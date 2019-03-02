// maindlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "maindlg.h"

#include "atlmisc.h"
#include <vector>
#include "../common/PGMFile.h"

#define MAX_TRAINING_ERROR	9999999.0

#define PIC_HEIGHT	30
#define PIC_WIDTH	32
#define PIC_PIXELS	PIC_HEIGHT*PIC_WIDTH
#define PIC_SCALE	255


BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	HWND hwnd=GetDlgItem(IDC_COMBO_TARGET_TYPE);

	::SendMessage(hwnd, CB_INSERTSTRING, 0, (LPARAM)TRAIN_NAME);
	::SendMessage(hwnd, CB_INSERTSTRING, 1, (LPARAM)TRAIN_DIRECTION);
	::SendMessage(hwnd, CB_INSERTSTRING, 2, (LPARAM)TRAIN_EMOTION);
	::SendMessage(hwnd, CB_INSERTSTRING, 3, (LPARAM)TRAIN_GLASSES);
	::SendMessage(hwnd, CB_SETCURSEL, 1, 0L);

	CAxWindow pgmView = GetDlgItem(IDC_PGMVIEW_FACE_TRAIN);
	
	HRESULT hr = pgmView.QueryControl ( &m_pPGMV );
	if(FAILED(hr))
		MessageBox(_T("Can not find the PGMView Control!"));

	HWND hwndIterateTime=GetDlgItem(IDC_EDIT_ITERATE_TIME);
	::SetWindowText(hwndIterateTime,_T("100000"));
	hwnd=GetDlgItem(IDC_EDIT_LEARNING_RATE);
	::SetWindowText(hwnd,_T("0.3"));
	hwnd=GetDlgItem(IDC_EDIT_MOMENTUM);
	::SetWindowText(hwnd,_T("0.3"));
	hwnd=GetDlgItem(IDC_EDIT_HIDDEN_UNITS);
	::SetWindowText(hwnd,_T("3"));


	HWND hwndStopError=GetDlgItem(IDC_EDIT_STOP_ERROR);
	::SetWindowText(hwndStopError,_T("0.003"));

	CEdit edSE(hwndStopError);
	CEdit edIT(hwndIterateTime);
	HWND hbtn=GetDlgItem(IDC_CHECK_STOP_ERROR);
	CButton btn(hbtn);
	btn.SetCheck(0);
	if(!btn.GetCheck())
	{
		edSE.SetReadOnly(TRUE);
		edIT.SetReadOnly(FALSE);
	}
	else
	{
		edSE.SetReadOnly(FALSE);
		edIT.SetReadOnly(TRUE);
	}
	
	m_bPrepared=FALSE;
	m_bTrained=FALSE;
	m_bSelectVarifyFile=FALSE;
	m_bSelectVarifyFile_Batch=FALSE;
	m_bBPLoadFromFile=FALSE;

	m_err=100.0;

	return TRUE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

LRESULT CMainDlg::OnClickedBtn_select_train(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CFolderDialog dlg;
	if(dlg.DoModal() != IDOK)
		return 0;

	CString sPath=dlg.GetFolderPath();
	CString sFoldName=dlg.GetFolderDisplayName();
	//MessageBox(sPath,sFoldName,MB_OK);

	if(m_TrainingSet.GetFileInfo(sPath))
	{
		const std::vector<CImage>& vecImage=m_TrainingSet.GetImageList();

		if ( m_pPGMV )
        {
			int n=vecImage.size();
			for(int i=0; i<n; i++)
			{
				const CImage* pImg=&(vecImage[i]);

				CComBSTR bstr=pImg->m_sFilePath;
				m_pPGMV->SetPGMFilePath(bstr,TRUE);

				m_bPrepared=TRUE;
				break;
			}
        }
		
	}

	return 0;
}

LRESULT CMainDlg::OnClickedBtn_train(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(!m_bPrepared)
	{
		MessageBox(_T("Please, select the training example set first!"),_T("Warning"),MB_OK|MB_ICONWARNING);
		return 0;
	}

	__int64 n64StartTime;
	__int64 n64EndTime;
	__int64 n64Freq = 0;
	QueryPerformanceCounter((LARGE_INTEGER *)&n64StartTime);
	QueryPerformanceFrequency((LARGE_INTEGER *)&n64Freq);
	double fInvTicksPerMs = (double)1000/n64Freq;


	TCHAR str[100];
	HWND hwnd=GetDlgItem(IDC_COMBO_TARGET_TYPE);
	::GetWindowText(hwnd,str,100);
	CString sTrainType=str;
	m_TrainingSet.SetTrainType(sTrainType);//设置训练类型(人名,朝向,心情,太阳镜)

	hwnd=GetDlgItem(IDC_EDIT_ITERATE_TIME);
	::GetWindowText(hwnd,str,100);
	int nIterateTime=_ttoi(str);
	if(nIterateTime <= 0)
	{
		MessageBox(_T("Please input a number >0 !"),_T("Warning"),MB_OK|MB_ICONWARNING);
		return 0;
	}

	hwnd=GetDlgItem(IDC_EDIT_LEARNING_RATE);
	::GetWindowText(hwnd,str,100);
	double eta;
	sscanf(str,"%lf", &eta);

	hwnd=GetDlgItem(IDC_EDIT_MOMENTUM);
	::GetWindowText(hwnd,str,100);
	double momentum;
	sscanf(str,"%lf", &momentum);

	hwnd=GetDlgItem(IDC_EDIT_HIDDEN_UNITS);
	::GetWindowText(hwnd,str,100);
	int nHiddenUnits=_ttoi(str);

	HWND hwnd_list_error=GetDlgItem(IDC_LIST_ERROR);
	::SendMessage(hwnd_list_error, LB_RESETCONTENT, 0, 0L);
	int q=0;
	::SendMessage(hwnd_list_error, LB_INSERTSTRING, q++, (LPARAM)(LPCTSTR)_T("-------------------------------------------------------------------------------------"));
	
	const std::vector<CImage>& vecImage=m_TrainingSet.GetImageList();
	const CImage* pImg=NULL;

	CPGMFile pgm;
	double data[PIC_PIXELS];
	USES_CONVERSION;

	m_BPNN.InitBPNN(PIC_PIXELS,m_TrainingSet.GetTargetVectorLength(sTrainType),nHiddenUnits);


	FILE* fp=fopen("bpnn_error_list.txt","w");

	int nTime=0;
	char aa[100];
	CString sError;

	double err;
	double total_err=MAX_TRAINING_ERROR;
	double stop_err;
	HWND hwndStopError=GetDlgItem(IDC_EDIT_STOP_ERROR);
	::GetWindowText(hwndStopError,str,100);
	sscanf(T2A(str),"%lf", &stop_err);

	HWND hwndFileName=GetDlgItem(IDC_STATIC_FILE_NAME);

	CButton btnCheck(GetDlgItem(IDC_CHECK_STOP_ERROR));
	BOOL bUseIterateTime=TRUE;
	if(btnCheck.GetCheck())
		bUseIterateTime=FALSE;
	else
		bUseIterateTime=TRUE;

	int nImgCount=vecImage.size();

	HWND hwnd_static_status=GetDlgItem(IDC_STATIC_STATUS);
	CString sTrainTime;

	while(bUseIterateTime ? (nTime < nIterateTime) : (total_err/nImgCount >= stop_err))
	{
		total_err=0.0;

		for(int i=0; i<nImgCount; i++)
		{
			pImg=&(vecImage[i]);

			::SendMessage(hwndFileName,WM_SETTEXT,0,(LPARAM)((LPCTSTR)(pImg->m_sFilePath)));

			CComBSTR bstr=pImg->m_sFilePath;
			m_pPGMV->SetPGMFilePath(bstr,TRUE);


			LPTSTR pfilepath=(LPSTR)((LPCTSTR)(pImg->m_sFilePath));
			pgm.openPGMFile(T2A(pfilepath));
			
			memset(data,0,sizeof(double)*PIC_PIXELS);
			const unsigned char* ch_data=pgm.getData();
			for(int m=0; m<PIC_PIXELS; m++)
				data[m]=double(ch_data[m]);
			
			m_BPNN.LoadInputUnit(data,PIC_PIXELS,PIC_SCALE);
			
			const TargetType& vecTT=m_TrainingSet.GetTarget(pImg);
			double* t=new double[vecTT.size()];
			for(int j=0; j<vecTT.size(); j++)
			{
				t[j]=vecTT[j];
			}

			m_BPNN.LoadTarget(t,vecTT.size());
			
			delete[] t;
			
			err=m_BPNN.Train(eta,momentum);
			total_err+=err;
			m_err=err;

			memset(aa,0,100);
			sprintf(aa,"T_it=%d\t,L_it=%d, err_o=%g\t%s\n",nTime+1,i,err,T2A(pfilepath));
			fputs(aa,fp);
			
			if(nTime == 0)
			{
				memset(aa,0,100);
				sprintf(aa,"time=%d            err=%g\n ",nTime+1,err);
				sError = aa;
				::SendMessage(hwnd_list_error, LB_INSERTSTRING, q++, (LPARAM)(LPCTSTR)sError);
			}
			nTime++;

			sTrainTime.Format(_T("%d"),nTime+1);
			::SendMessage(hwnd_static_status,WM_SETTEXT,0,(LPARAM)((LPCTSTR)sTrainTime));
		}

		memset(aa,0,100);
		sprintf(aa,"time=%d       cur_err=%g      avg_err=%g\n ",nTime+1,err,total_err/i);
		CString sError=aa;
		::SendMessage(hwnd_list_error, LB_INSERTSTRING, q++, (LPARAM)(LPCTSTR)sError);
	}

	fclose(fp);

	QueryPerformanceCounter((LARGE_INTEGER *)&n64EndTime);
	unsigned int unMs = (unsigned int)((n64EndTime - n64StartTime) * fInvTicksPerMs);
	CString sTimeConsumed;
	sTimeConsumed.Format(_T("Consumed Time: %d.%d Seconds"),int(unMs/1000),unMs-int(unMs/1000)*1000);
	::SendMessage(hwnd_list_error, LB_INSERTSTRING, 0, (LPARAM)(LPCTSTR)sTimeConsumed);

	//MessageBox(_T("OK!"));

	m_bTrained=TRUE;

	return 0;
}

LRESULT CMainDlg::OnClickedBtn_select_varify(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	TCHAR szFilter[] = _T( "PGM Files *.PGM\0");

	CFileDialog dlg(TRUE,_T("pgm"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);
	if(dlg.DoModal() != IDOK)
		return 0;

	//MessageBox(dlg.m_szFileName);

	CAxWindow pgmView = GetDlgItem(IDC_PGMVIEW_FACE_VALIFY);
	CComPtr<IPGMView> pPGMV;
	HRESULT hr = pgmView.QueryControl (&pPGMV);
	if(FAILED(hr))
	{
		MessageBox(_T("Can not find the PGMView Control!"));
		return 0;
	}

	m_sVarifyFilePath=dlg.m_szFileName;

	HWND hwndFileName=GetDlgItem(IDC_STATIC_FILE_NAME2);
	::SendMessage(hwndFileName,WM_SETTEXT,0,(LPARAM)((LPCTSTR)m_sVarifyFilePath));
	CComBSTR bstr=m_sVarifyFilePath;
	pPGMV->SetPGMFilePath(bstr,TRUE);


	m_bSelectVarifyFile=TRUE;

	return 0;
}

LRESULT CMainDlg::OnClickedBtn_varify(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(!(m_bTrained && m_bSelectVarifyFile) && !m_bBPLoadFromFile)
	{
		MessageBox(_T("Train the BPNN(or load from a save file) and select a varify file first!"),_T("Warning"),MB_OK|MB_ICONWARNING);
		return 0;
	}

	HWND hwnd_list_varify=GetDlgItem(IDC_LIST_VARIFY_RESULT);
	::SendMessage(hwnd_list_varify, LB_RESETCONTENT, 0, 0L);

	std::vector<double> vecOutput;
	CPGMFile pgm;
	double data[PIC_PIXELS];
	USES_CONVERSION;

	LPTSTR p=(LPSTR)((LPCTSTR)(m_sVarifyFilePath));
	pgm.openPGMFile(T2A(p));
	//MessageBox(p);
	memset(data,0,sizeof(double)*PIC_PIXELS);
	const unsigned char* ch_data=pgm.getData();
	for(int m=0; m<PIC_PIXELS; m++)
		data[m]=double(ch_data[m]);

	m_BPNN.Test(data,PIC_PIXELS,PIC_SCALE,vecOutput);

	CString str;
	CString sTemp;
	char aa[100];
	int q=0;
	int nMaxIndex=-1;
	double fMax=0.0;
	for(int i=0; i<vecOutput.size(); i++)
	{
		if(vecOutput[i]>fMax)
		{
			fMax=vecOutput[i];
			nMaxIndex=i;
		}

		memset(aa,0,100);
		sprintf(aa,"%d=%g\n",i+1,vecOutput[i]);
		sTemp.Format(_T("%s"),aa);
		::SendMessage(hwnd_list_varify, LB_INSERTSTRING, q++, (LPARAM)(LPCTSTR)sTemp);
	}

	sTemp=_T("-----------------------");
	::SendMessage(hwnd_list_varify, LB_INSERTSTRING, q++, (LPARAM)(LPCTSTR)sTemp);
	
	sTemp=m_TrainingSet.GetTargetName(nMaxIndex);
	::SendMessage(hwnd_list_varify, LB_INSERTSTRING, q++, (LPARAM)(LPCTSTR)sTemp);

	//MessageBox(str);
	return 0;
}

LRESULT CMainDlg::OnClickedCheck_stop_error(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CEdit edIT(GetDlgItem(IDC_EDIT_ITERATE_TIME));
	CEdit edSE(GetDlgItem(IDC_EDIT_STOP_ERROR));
	
	CButton btnCheck(hWndCtl);
	if(!btnCheck.GetCheck())
	{
		edSE.SetReadOnly(TRUE);
		edIT.SetReadOnly(FALSE);
	}
	else
	{
		edSE.SetReadOnly(FALSE);
		edIT.SetReadOnly(TRUE);
	}

	return 0;
}

LRESULT CMainDlg::OnClickedBtn_save(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	TCHAR szFilter[] = _T( "BPNN Files *.bp\0");
	CFileDialog dlg(FALSE,_T("bp"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);
	if(dlg.DoModal() != IDOK)
		return 0;
	
	TCHAR sTrainType[100];
	TCHAR sTemp[100];
	HWND hwnd=GetDlgItem(IDC_COMBO_TARGET_TYPE);
	::GetWindowText(hwnd,sTrainType,100);

	hwnd=GetDlgItem(IDC_EDIT_ITERATE_TIME);
	::GetWindowText(hwnd,sTemp,100);
	int nIT=_ttoi(sTemp);

	hwnd=GetDlgItem(IDC_EDIT_STOP_ERROR);
	::GetWindowText(hwnd,sTemp,100);
	double fStopError;
	sscanf(sTemp,"%lf", &fStopError);

	target_type tat;
	m_TrainingSet.GetTargetTypeData(&tat);

	bool bRes=m_BPNN.SaveBPNNFile(T2A(dlg.m_szFileName),T2A(sTrainType),nIT,fStopError,&tat);
	if(bRes)
		MessageBox(_T("Save OK!"),_T("Success"),MB_OK|MB_ICONINFORMATION);
	else
		MessageBox(_T("Save Error!"),_T("Error"),MB_OK|MB_ICONERROR);
	
	return 0;
}

LRESULT CMainDlg::OnClickedBtn_load(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;
	
	TCHAR szFilter[] = _T( "BPNN Files *.bp\0");
	CFileDialog dlg(TRUE,_T("bp"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);
	if(dlg.DoModal() != IDOK)
		return 0;

	CString sFilePath=dlg.m_szFileName;
	std::vector<std::pair<std::string,std::vector<double> > > target;

	bool b=m_BPNN.LoadBPNNFile(sFilePath,&target);
	if(!b)
		MessageBox(_T("Can not open the bpnn file!"),_T("Error"),MB_OK|MB_ICONERROR);

	m_TrainingSet.SetTargetTypeData(&target);

	HWND hwnd_static_status=GetDlgItem(IDC_STATIC_STATUS);
	CString sStatus;
	sStatus.Format(_T("Load from file:%s"),sFilePath);
	::SendMessage(hwnd_static_status,WM_SETTEXT,0,(LPARAM)((LPCTSTR)sStatus));

	m_bBPLoadFromFile=TRUE;

	return 0;
}

LRESULT CMainDlg::OnClickedBtn_select_varify_batch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CFolderDialog dlg;
	if(dlg.DoModal() != IDOK)
		return 0;
	
	CString sPath=dlg.GetFolderPath();
	CString sFoldName=dlg.GetFolderDisplayName();
	
	if(m_TestingSet.GetFileInfo(sPath))
	{
		const std::vector<CImage>& vecImage=m_TestingSet.GetImageList();
		CAxWindow pgmView = GetDlgItem(IDC_PGMVIEW_FACE_VALIFY2);
		CComPtr<IPGMView> pPGMV;
		HRESULT hr = pgmView.QueryControl (&pPGMV);
		if(FAILED(hr))
		{
			MessageBox(_T("Can not find the PGMView Control!"));
			return 0;
		}
		
		if ( pPGMV )
        {
			int n=vecImage.size();
			for(int i=0; i<n; i++)
			{
				const CImage* pImg=&(vecImage[i]);
				
				CComBSTR bstr=pImg->m_sFilePath;
				pPGMV->SetPGMFilePath(bstr,TRUE);
				
				m_bPrepared=TRUE;
				break;
			}
        }
		
	}

	m_bSelectVarifyFile_Batch=TRUE;
	
	return 0;
}

LRESULT CMainDlg::OnClickedBtn_varify_batch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(!(m_bTrained && m_bSelectVarifyFile_Batch) && !m_bBPLoadFromFile)
	{
		MessageBox(_T("Train the BPNN(or load from a save file) and select a varify FOLDER first!"),_T("Warning"),MB_OK|MB_ICONWARNING);
		return 0;
	}
	
	HWND hwnd_list_varify=GetDlgItem(IDC_LIST_VARIFY_RESULT);
	::SendMessage(hwnd_list_varify, LB_RESETCONTENT, 0, 0L);
	
	std::vector<double> vecOutput;
	CPGMFile pgm;
	double data[PIC_PIXELS];
	CString sTemp;
	char aa[100];

	USES_CONVERSION;
	
	int i=0;
	const std::vector<CImage>& vecImage=m_TestingSet.GetImageList();
	const CImage* pImg=NULL;
	int nRightCount=0;
	CString sTestTarget;
	CString sTrueTarget;
	int nMaxIndex;
	double fMax=0.0;
	int q=0;

	for(i=0; i<vecImage.size(); i++)
	{
		pImg=&(vecImage[i]);
		LPTSTR p=(LPSTR)((LPCTSTR)(pImg->m_sFilePath));
		pgm.openPGMFile(T2A(p));

		memset(data,0,sizeof(double)*PIC_PIXELS);
		const unsigned char* ch_data=pgm.getData();
		for(int m=0; m<PIC_PIXELS; m++)
		data[m]=double(ch_data[m]);

		vecOutput.clear();
		m_BPNN.Test(data,PIC_PIXELS,PIC_SCALE,vecOutput);

		::SendMessage(GetDlgItem(IDC_STATIC_FILE_NAME3), WM_SETTEXT, 0, (LPARAM)(LPCTSTR)(pImg->m_sFilePath));
		::SendMessage(hwnd_list_varify, LB_INSERTSTRING, q++, (LPARAM)(LPCTSTR)(GetFileName(pImg->m_sFilePath)));

		fMax=0.0;

		for(int i=0; i<vecOutput.size(); i++)
		{
			if(vecOutput[i]>fMax)
			{
				fMax=vecOutput[i];
				nMaxIndex=i;
			}

		memset(aa,0,100);
		sprintf(aa,"%d=%g\n",i+1,vecOutput[i]);
		sTemp.Format(_T("%s"),aa);
		::SendMessage(hwnd_list_varify, LB_INSERTSTRING, q++, (LPARAM)(LPCTSTR)sTemp);

		}

		sTestTarget=m_TrainingSet.GetTargetName(nMaxIndex);
		BOOL bRight=m_TestingSet.IsTestRight(m_TrainingSet.GetTrainType(),sTestTarget,*pImg,sTrueTarget);
		if(bRight)
			nRightCount++;

		sTemp=_T("-----------------------");
		::SendMessage(hwnd_list_varify, LB_INSERTSTRING, q++, (LPARAM)(LPCTSTR)sTemp);
				
		CString sTestRes;
		sTestRes.Format("%s<-->%s....................%s",sTestTarget,sTrueTarget,bRight?_T("RIGHT"):_T("WRONG"));
		::SendMessage(hwnd_list_varify, LB_INSERTSTRING, q++, (LPARAM)(LPCTSTR)sTestRes);

		::SendMessage(hwnd_list_varify, LB_INSERTSTRING, q++, (LPARAM)(LPCTSTR)sTemp);
	}

	sTemp=_T("-----------------------------------------------------------------------");
	::SendMessage(hwnd_list_varify, LB_INSERTSTRING, q++, (LPARAM)(LPCTSTR)sTemp);
	sprintf(aa,"%0.2f%%",nRightCount*1.0/vecImage.size()*100);
	sTemp.Format(_T("Accuracy: %s"),aa);
	::SendMessage(hwnd_list_varify, LB_INSERTSTRING, q++, (LPARAM)(LPCTSTR)sTemp);
	
	return 0;
}

CString CMainDlg::GetFileName(CString sFilePath)
{
	int n=sFilePath.ReverseFind(_TCHAR('\\'));
	CString str;
	str=sFilePath.Mid(n+1);
	return str;
}
