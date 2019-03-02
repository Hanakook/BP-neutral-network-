// TrainingSet.h: interface for the CTrainingSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEINFO_H__D896F3FF_B70C_44B1_8E6D_E18A93A23E9F__INCLUDED_)
#define AFX_FILEINFO_H__D896F3FF_B70C_44B1_8E6D_E18A93A23E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <map>
#include "../common/BPNN.h"

#define		TARGET_ONE		(double(0.9))
#define		TARGET_ZERO		(double(0.1))

#define		TRAIN_NAME			_T("Name")
#define		TRAIN_DIRECTION		_T("Direction")
#define		TRAIN_EMOTION		_T("Emotion")
#define		TRAIN_GLASSES		_T("Glasses")

typedef std::vector<double>	TargetType;

enum TARGET_VECTOR_TYPE
{
	ONE_OF_N,
	N_MINUS_ONE,
	VALUE
};

enum TRAINTYPE
{
	TRAINTYPE_NAME,
	TRAINTYPE_DIRECTION,
	TRAINTYPE_EMOTION,
	TRAINTYPE_GLASSES
};

class CImage
{
public:
	CString m_sFilePath;
	CString m_sName;
	CString m_sDirection;
	CString m_sEmotion;
	CString m_sGlasses;
};

//文件信息类
class CTrainingSet
{
public:
	CTrainingSet();
	virtual ~CTrainingSet();
public:
	BOOL GetFileInfo(CString sPath);
	const std::vector<CImage>& GetImageList();
	int GetTargetVectorLength(CString sTarget);
	const TargetType& GetTarget(const CImage* pImg);
	BOOL SetTrainType(CString sTrainType);
	TRAINTYPE GetTrainType();
	CString GetTargetName(int nIndex);
	void GetTargetTypeData(target_type* tat);
	void SetTargetTypeData(const target_type* tat);
protected:
	void ClearHistory();
	void GetClassInfo(CString sFilePath,CString sFileName);
	void SetTrainType(TRAINTYPE enType);
	void GenerateTarget();
	BOOL IsExistString(std::vector<CString>& vec,CString& str);
	void GenTargetVectors(const std::vector<CString>& stdString, std::map<CString,TargetType>& m_mapTarget, TARGET_VECTOR_TYPE enType=ONE_OF_N);
	BOOL IsEqualTT(TargetType& tt1,TargetType& tt2);
protected:

	std::vector<CImage>	m_vecImage;
	std::map<CString,TargetType> m_mapNameTarget;
	std::map<CString,TargetType> m_mapDirectionTarget;
	std::map<CString,TargetType> m_mapEmotionTarget;
	std::map<CString,TargetType> m_mapGlassesTarget;
	//////////////////////////////////////////////////////////////////////////
	//类别
	std::vector<CString> m_vecName;
	std::vector<CString> m_vecDirection;
	std::vector<CString> m_vecEmotion;
	std::vector<CString> m_vecGlasses;

	TRAINTYPE	m_enType;
};

class CTestingSet : public CTrainingSet
{
public:
	CTestingSet(){};
public:
	//只取一层目录内的文件
	BOOL GetFileInfo(CString sPath);

	BOOL IsTestRight(TRAINTYPE enTrainType,CString sTestTarget,const CImage& img,CString& sTrueTarget);
};

#endif // !defined(AFX_TRAININGSET_H__D896F3FF_B70C_44B1_8E6D_E18A93A23E9F__INCLUDED_)
