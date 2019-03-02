// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	m_wndMailLink.SubclassWindow(GetDlgItem(IDC_STATIC_MAIL_LINK));
	m_wndMailLink.SetHyperLink(_T("mailto:c_gao@163.net") );
	
	m_wndBlogLink.SubclassWindow(GetDlgItem(IDC_STATIC_BLOG_LINK));
	m_wndBlogLink.SetHyperLink(_T("http://cgao.csai.cn"));

	return TRUE;
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
