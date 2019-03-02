// TrainingSet.cpp: implementation of the CTrainingSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TrainingSet.h"


#define GET_VECTOR(mp) \
for(it=mp.begin(); it!=mp.end(); it++) \
{ \
	memset(chTemp,0,sizeof(TCHAR)*100); \
	CString str=(*it).first; \
	for(i=0; i<str.GetLength(); i++) \
		chTemp[i]=str[i]; \
	target_name=T2A(chTemp); \
	target_vector=(*it).second; \
	tat->push_back(std::pair<std::string,std::vector<double> > (target_name,target_vector)); \
}

#define SET_VECTOR(mp,v) \
mp.clear(); \
for(i=0; i<tat->size(); i++) \
{ \
	sTargetName.Format("%s",((*tat)[i]).first.c_str()); \
	v.push_back(sTargetName); \
	mp[sTargetName]=(*tat)[i].second; \
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrainingSet::CTrainingSet()
{
	m_enType=TRAINTYPE_DIRECTION;
}

CTrainingSet::~CTrainingSet()
{

}

void CTrainingSet::SetTrainType(TRAINTYPE enType)
{
	m_enType=enType;
}

BOOL CTrainingSet::SetTrainType(CString sTrainType)
{
	if(sTrainType == TRAIN_NAME)
	{
		SetTrainType(TRAINTYPE_NAME);
		return TRUE;
	}
	if(sTrainType == TRAIN_DIRECTION)
	{
		SetTrainType(TRAINTYPE_DIRECTION);
		return TRUE;
	}
	if(sTrainType == TRAIN_EMOTION)
	{
		SetTrainType(TRAINTYPE_EMOTION);
		return TRUE;
	}
	if(sTrainType == TRAIN_GLASSES)
	{
		SetTrainType(TRAINTYPE_GLASSES);
		return TRUE;
	}
	return FALSE;
}

TRAINTYPE CTrainingSet::GetTrainType()
{
	return m_enType;
}

BOOL CTrainingSet::GetFileInfo(CString sPath)
{
	ClearHistory();
	
	CString sPathFile=sPath;
	CString sFile;
	WIN32_FIND_DATA FindFileData;
	sPathFile+=_T("\\*.*");
	HANDLE hFind=::FindFirstFile(sPathFile,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
		return FALSE;
	while(TRUE)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//文件夹
			if(FindFileData.cFileName[0]!=_TCHAR('.'))
			{
				CString sDictory=sPath+_T("\\")+FindFileData.cFileName;
				
				CString sPathFile2=sDictory+_T("\\*.*");
				
				WIN32_FIND_DATA FindFileData2;
				HANDLE hFind2=::FindFirstFile(sPathFile2,&FindFileData2);
				
				if(INVALID_HANDLE_VALUE == hFind2)
					return FALSE;
				
				while(TRUE)//读人脸文件名
				{
					if(FindFileData2.cFileName[0]!=_TCHAR('.'))
					{
						CString sFile=sDictory+_T("\\")+FindFileData2.cFileName;
						if(!(FindFileData2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))//是文件
						{
							CString sTemp=FindFileData2.cFileName;
							if(
								(sTemp.GetAt(sTemp.GetLength()-1) == _TCHAR('m')) || (sTemp.GetAt(sTemp.GetLength()-1) == _TCHAR('M')) &&
								(sTemp.GetAt(sTemp.GetLength()-2) == _TCHAR('g')) || (sTemp.GetAt(sTemp.GetLength()-2) == _TCHAR('G')) &&
								(sTemp.GetAt(sTemp.GetLength()-3) == _TCHAR('p')) || (sTemp.GetAt(sTemp.GetLength()-3) == _TCHAR('P')) 
								)
								GetClassInfo(sDictory+_T("\\")+FindFileData2.cFileName,FindFileData2.cFileName);
							else//当读到不是人脸文件时,略过
							{
								if(0)
								{
									ClearHistory();
									return FALSE;
								}
							}
						}
						
					}
					
					if(!FindNextFile(hFind2,&FindFileData2))
						break;
				}
				FindClose(hFind2);
			}
		}
		else
		{
			//文件
			//MessageBox(FindFileData.cFileName);
		}
		if(!FindNextFile(hFind,&FindFileData))
			break;
	}
	
	FindClose(hFind);

	//////////////////////////////////////////////////////////////////////////
	//生成目标向量
	GenerateTarget();
	
	return TRUE;
}

//获得分类信息,并生成目标向量,最终生成所有的训练样本
void CTrainingSet::GetClassInfo(CString sFilePath,CString sFileName)
{
	CImage img;

	TCHAR ch=TCHAR('_');
	int nlen=sFileName.GetLength();
	int i=0;
	int n=sFileName.Find(ch,i);

	//文件路径
	img.m_sFilePath=sFilePath;

	//姓名
	img.m_sName=sFileName.Mid(0,n);
	//MessageBox(NULL,sName,_T(""),MB_OK);
	
	//朝向
	i=n+1;
	n=sFileName.Find(ch,i);
	img.m_sDirection=sFileName.Mid(i,n-i);
	//MessageBox(NULL,img.m_sDirection,_T(""),MB_OK);

	//心情
	i=n+1;
	n=sFileName.Find(ch,i);
	img.m_sEmotion=sFileName.Mid(i,n-i);
	//MessageBox(NULL,sEmotion,_T(""),MB_OK);

	//太阳镜
	i=n+1;
	n=sFileName.Find(ch,i);
	img.m_sGlasses=sFileName.Mid(i,n-i);
	//MessageBox(NULL,sGlass,_T(""),MB_OK);

	m_vecImage.push_back(img);
}

void CTrainingSet::GenerateTarget()
{
	int n=m_vecImage.size();
	CImage* pImage=NULL;

	m_vecName.clear();
	m_vecDirection.clear();
	m_vecEmotion.clear();
	m_vecGlasses.clear();

	for(int i=0; i<n; i++)
	{
		pImage=&(m_vecImage[i]);

		if(!IsExistString(m_vecName,pImage->m_sName))
			m_vecName.push_back(pImage->m_sName);

		if(!IsExistString(m_vecDirection,pImage->m_sDirection))
			m_vecDirection.push_back(pImage->m_sDirection);
		
		if(!IsExistString(m_vecEmotion,pImage->m_sEmotion))
			m_vecEmotion.push_back(pImage->m_sEmotion);

		if(!IsExistString(m_vecGlasses,pImage->m_sGlasses))
			m_vecGlasses.push_back(pImage->m_sGlasses);
	}


	//Name
	GenTargetVectors(m_vecName,m_mapNameTarget);
	//Direction
	GenTargetVectors(m_vecDirection,m_mapDirectionTarget);
	//CString str;
	//str.Format("Direction:%d",m_vecDirection.size());
	//MessageBox(NULL,str,_T(""),MB_OK);
	//Emotion
	GenTargetVectors(m_vecEmotion,m_mapEmotionTarget);
	
	//str.Format("Emotion:%d",m_vecDirection.size());
	//MessageBox(NULL,str,_T(""),MB_OK);
	//Glasses
	GenTargetVectors(m_vecGlasses,m_mapGlassesTarget);

	//str.Format("Glasses:%d",m_vecDirection.size());
	//MessageBox(NULL,str,_T(""),MB_OK);

}

BOOL CTrainingSet::IsExistString(std::vector<CString>& vec,CString& str)
{
	for(int i=0; i<vec.size(); i++)
	{
		if(str == vec[i])
			return TRUE;
	}
	return FALSE;
}

void CTrainingSet::GenTargetVectors(const std::vector<CString>& stdString, std::map<CString,TargetType>& m_mapTarget, TARGET_VECTOR_TYPE enType)
{
	TargetType vecTarget;

	switch(enType)
	{
	case ONE_OF_N:
		{
			int n=stdString.size();
			for(int i=0; i<n; i++)
			{
				vecTarget.clear();
				
				for(int j=0; j<n; j++)
				{
					if(j == i)
						vecTarget.push_back(TARGET_ONE);
					else
						vecTarget.push_back(TARGET_ZERO);
				}
				
				m_mapTarget[stdString[i]]=vecTarget;

// 				char aa[100];
// 				memset(aa,0,100);
// 				CString sTemp,str;
// 				for(int k=0; k<vecTarget.size(); k++)
// 				{
// 					sprintf(aa,"%d=%g\n",k,vecTarget[k]);
// 					sTemp=aa;
// 					str+=sTemp;
// 				}
// 				MessageBox(NULL,str,_T(""),MB_OK);
			}
		}
		break;
	case N_MINUS_ONE:
		break;
	case VALUE:
		break;
	default:
		break;
	}
}

const std::vector<CImage>& CTrainingSet::GetImageList()
{
	return m_vecImage;
}

int CTrainingSet::GetTargetVectorLength(CString sTarget)
{
	if(sTarget == TRAIN_NAME)
		return m_vecName.size();

	if(sTarget == TRAIN_DIRECTION)
		return m_vecDirection.size();

	if(sTarget == TRAIN_EMOTION)
		return m_vecEmotion.size();

	if(sTarget == TRAIN_GLASSES)
		return m_vecGlasses.size();

	return 0;
}

void CTrainingSet::ClearHistory()
{
//	m_vecDictory.clear();//文件夹全路径
//	m_vecFile.clear();//每个文件夹下对应的文件全路径
	
	m_vecImage.clear();
	m_mapNameTarget.clear();
	m_mapDirectionTarget.clear();
	m_mapEmotionTarget.clear();
	m_mapGlassesTarget.clear();

	m_vecName.clear();
	m_vecDirection.clear();
	m_vecEmotion.clear();
	m_vecGlasses.clear();
}


const TargetType& CTrainingSet::GetTarget(const CImage* pImg)
{
	switch(m_enType)
	{
	case TRAINTYPE_NAME:
		return m_mapNameTarget[pImg->m_sName];
	case TRAINTYPE_DIRECTION:
		return m_mapDirectionTarget[pImg->m_sDirection];
	case TRAINTYPE_EMOTION:
		return m_mapEmotionTarget[pImg->m_sEmotion];
	case TRAINTYPE_GLASSES:
		return m_mapGlassesTarget[pImg->m_sGlasses];
	}
	return m_mapGlassesTarget[pImg->m_sGlasses];
}

CString CTrainingSet::GetTargetName(int nIndex)
{
	TargetType tt;
	int i=0,n=0;
	TargetType* ptt=NULL;
	switch(m_enType)
	{
	case TRAINTYPE_NAME:
		{
			n=m_vecName.size();
			for(i=0; i<n; i++)
				if(i == nIndex)
					tt.push_back(TARGET_ONE);
				else
					tt.push_back(TARGET_ZERO);

			for(i=0; i<n; i++)
			{
				ptt=&(m_mapNameTarget[m_vecName[i]]);
				if(IsEqualTT(tt,*ptt))
					return m_vecName[i];
			}
		}
		break;
	case TRAINTYPE_DIRECTION:
		{
			n=m_vecDirection.size();
			for(i=0; i<n; i++)
				if(i == nIndex)
					tt.push_back(TARGET_ONE);
				else
					tt.push_back(TARGET_ZERO);
				
			for(i=0; i<n; i++)
			{
					ptt=&(m_mapDirectionTarget[m_vecDirection[i]]);
					if(IsEqualTT(tt,*ptt))
						return m_vecDirection[i];
			}
		}
		break;
	case TRAINTYPE_EMOTION:
		{
			n=m_vecEmotion.size();
			for(i=0; i<n; i++)
				if(i == nIndex)
					tt.push_back(TARGET_ONE);
				else
					tt.push_back(TARGET_ZERO);
				
			for(i=0; i<n; i++)
			{
				ptt=&(m_mapEmotionTarget[m_vecEmotion[i]]);
				if(IsEqualTT(tt,*ptt))
					return m_vecEmotion[i];
			}
		}
		break;
	case TRAINTYPE_GLASSES:
		{
			n=m_vecGlasses.size();
			for(i=0; i<n; i++)
				if(i == nIndex)
					tt.push_back(TARGET_ONE);
				else
					tt.push_back(TARGET_ZERO);
				
			for(i=0; i<n; i++)
			{
				ptt=&(m_mapGlassesTarget[m_vecGlasses[i]]);
				if(IsEqualTT(tt,*ptt))
					return m_vecGlasses[i];
			}
		}
		break;
	}

	return _T("NONE,Error");
}

BOOL CTrainingSet::IsEqualTT(TargetType& tt1,TargetType& tt2)
{
	int n=tt1.size();
	if(n != tt2.size())
		return FALSE;

	for(int i=0; i<n; i++)
		if(tt1[i] != tt2[i])
			return FALSE;

	return TRUE;
}

void CTrainingSet::GetTargetTypeData(target_type* tat)
{
	int i=0;
	tat->clear();
	std::string target_name;
	std::vector<double> target_vector;
	TCHAR chTemp[100];
	std::map<CString,TargetType>::iterator it;
	USES_CONVERSION;
	switch(m_enType)
	{
	case TRAINTYPE_NAME:
		GET_VECTOR(m_mapNameTarget);
		break;
	case TRAINTYPE_DIRECTION:
		GET_VECTOR(m_mapDirectionTarget);
		break;
	case TRAINTYPE_EMOTION:
		GET_VECTOR(m_mapEmotionTarget);
		break;
	case TRAINTYPE_GLASSES:
		GET_VECTOR(m_mapGlassesTarget);
		break;
	}
}

void CTrainingSet::SetTargetTypeData(const target_type* tat)
{
	int i=0;
	CString sTargetName;
	switch(m_enType)
	{
	case TRAINTYPE_NAME:
		SET_VECTOR(m_mapNameTarget,m_vecName);
		break;
	case TRAINTYPE_DIRECTION:
		SET_VECTOR(m_mapDirectionTarget,m_vecDirection);
		break;
	case TRAINTYPE_EMOTION:
		SET_VECTOR(m_mapEmotionTarget,m_vecEmotion);
		break;
	case TRAINTYPE_GLASSES:
		SET_VECTOR(m_mapGlassesTarget,m_vecGlasses);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////

BOOL CTestingSet::GetFileInfo(CString sPath)
{
	ClearHistory();

	CString sPathFile=sPath;
	CString sFile;
	WIN32_FIND_DATA FindFileData;
	sPathFile+=_T("\\*.*");
	HANDLE hFind=::FindFirstFile(sPathFile,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
		return FALSE;
	while(TRUE)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			;//文件夹
		else
		{
			//文件
			if(FindFileData.cFileName[0]!=_TCHAR('.'))
			{
				CString sFile=sPath+_T("\\")+FindFileData.cFileName;
				CString sTemp=FindFileData.cFileName;
				//MessageBox(sFile);
				
				if(
					(sTemp.GetAt(sTemp.GetLength()-1) == _TCHAR('m')) || (sTemp.GetAt(sTemp.GetLength()-1) == _TCHAR('M')) &&
					(sTemp.GetAt(sTemp.GetLength()-2) == _TCHAR('g')) || (sTemp.GetAt(sTemp.GetLength()-2) == _TCHAR('G')) &&
					(sTemp.GetAt(sTemp.GetLength()-3) == _TCHAR('p')) || (sTemp.GetAt(sTemp.GetLength()-3) == _TCHAR('P')) 
					)
					GetClassInfo(sFile,sTemp);
				else//当读到不是人脸文件时,略过
				{
					if(0)
					{
						ClearHistory();
						return FALSE;
					}
				}

			}
			
		}
		if(!FindNextFile(hFind,&FindFileData))
			break;
	}
	
	FindClose(hFind);
	
	//////////////////////////////////////////////////////////////////////////
	//生成目标向量
	GenerateTarget();
	
	return TRUE;
}

BOOL CTestingSet::IsTestRight(TRAINTYPE enTrainType,CString sTestTarget,const CImage& img,CString& sTrueTarget)
{
	switch(enTrainType)
	{
	case TRAINTYPE_NAME:
		sTrueTarget=img.m_sName;
		if(sTrueTarget == sTestTarget)
			return TRUE;
		break;
	case TRAINTYPE_DIRECTION:
		sTrueTarget=img.m_sDirection;
		if(sTrueTarget == sTestTarget)
			return TRUE;
		break;
	case TRAINTYPE_EMOTION:
		sTrueTarget=img.m_sEmotion;
		if(sTrueTarget == sTestTarget)
			return TRUE;
		break;
	case TRAINTYPE_GLASSES:
		sTrueTarget=img.m_sGlasses;
		if(sTrueTarget == sTestTarget)
			return TRUE;
		break;
	}

	return FALSE;
}