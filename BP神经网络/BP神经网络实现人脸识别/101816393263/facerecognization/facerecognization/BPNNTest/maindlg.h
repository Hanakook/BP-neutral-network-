// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINDLG_H__A06E136F_E25F_4BFA_9CB4_D9638BB87CBF__INCLUDED_)
#define AFX_MAINDLG_H__A06E136F_E25F_4BFA_9CB4_D9638BB87CBF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "TrainingSet.h"
#include "../facerecognization.h"
#include "../common/BPNN.h"

class CMainDlg : public CAxDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_BTN_SELECT_TRAIN, BN_CLICKED, OnClickedBtn_select_train)
		COMMAND_HANDLER(IDC_BTN_TRAIN, BN_CLICKED, OnClickedBtn_train)
		COMMAND_HANDLER(IDC_BTN_SELECT_VARIFY, BN_CLICKED, OnClickedBtn_select_varify)
		COMMAND_HANDLER(IDC_BTN_VARIFY, BN_CLICKED, OnClickedBtn_varify)
		COMMAND_HANDLER(IDC_CHECK_STOP_ERROR, BN_CLICKED, OnClickedCheck_stop_error)
		COMMAND_HANDLER(IDC_BTN_SAVE, BN_CLICKED, OnClickedBtn_save)
		COMMAND_HANDLER(IDC_BTN_SELECT_VARIFY_BATCH, BN_CLICKED, OnClickedBtn_select_varify_batch)
		COMMAND_HANDLER(IDC_BTN_LOAD, BN_CLICKED, OnClickedBtn_load)
		COMMAND_HANDLER(IDC_BTN_VARIFY_BATCH, BN_CLICKED, OnClickedBtn_varify_batch)
	END_MSG_MAP()

	CTrainingSet		m_TrainingSet;
	CTestingSet			m_TestingSet;
	BOOL				m_bPrepared;
	BOOL				m_bTrained;
	CComPtr<IPGMView>	m_pPGMV;
	CBPNN				m_BPNN;

	BOOL				m_bSelectVarifyFile;
	BOOL				m_bSelectVarifyFile_Batch;
	BOOL				m_bBPLoadFromFile;
	CString				m_sVarifyFilePath;

	double m_err;
	//CButton				m_bCheckError;

	CString GetFileName(CString sFilePath);
	
// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void CloseDialog(int nVal);
	LRESULT OnClickedBtn_select_train(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedBtn_train(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedBtn_select_varify(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedBtn_varify(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCheck_stop_error(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedBtn_save(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedBtn_select_varify_batch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedBtn_load(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedBtn_varify_batch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__A06E136F_E25F_4BFA_9CB4_D9638BB87CBF__INCLUDED_)
