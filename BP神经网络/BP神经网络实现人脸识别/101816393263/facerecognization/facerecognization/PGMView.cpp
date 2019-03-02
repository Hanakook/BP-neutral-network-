// PGMView.cpp : Implementation of CPGMView

#include "stdafx.h"
#include "Facerecognization.h"
#include "PGMView.h"
#include "stdio.h"

/////////////////////////////////////////////////////////////////////////////
// CPGMView

CPGMView::CPGMView()
{
	//memset(m_sFilePath,sizeof(TCHAR)*MAX_PATH_CHAR,0);

	m_nScaleTimes=1;
	
	m_nOffsetBrightness=0;	
}
/*
HRESULT CPGMView::OnDraw(ATL_DRAWINFO& di)
{
	RECT& rc = *(RECT*)di.prcBounds;
	Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
	
	SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
	LPCTSTR pszText = _T("ATL 3.0 : PGMView");
	TextOut(di.hdcDraw, 
		(rc.left + rc.right) / 2, 
		(rc.top + rc.bottom) / 2, 
		pszText, 
		lstrlen(pszText));
	
	return S_OK;
}
*/

STDMETHODIMP CPGMView::SetPGMFilePath(BSTR sFilePath,VARIANT_BOOL bRefresh)
{
	// TODO: Add your implementation code here

	m_sFilePath=sFilePath;

	USES_CONVERSION;

	_bstr_t bs=sFilePath;
	OPERATOR_CODE opc=m_pgm.openPGMFile(/*T2A(sFilePath)*/(char*)bs);

	if(opc != FILE_OK)
	{
		TCHAR strMsg[100];
		_stprintf(strMsg,_T("Cann't open the file: %s"),m_sFilePath);
		
		MessageBox(strMsg,_T("ERROR"),MB_ICONERROR);

		return S_FALSE;
	}

  	if(bRefresh)
 	{
		FireViewChange();
	}

	return S_OK;
}

STDMETHODIMP CPGMView::Refresh(void)
{
	// TODO: Add your implementation code here


	FireViewChange();
	return S_OK;
}


HRESULT CPGMView::OnDraw(ATL_DRAWINFO& di)
{
	RECT& rc = *(RECT*)di.prcBounds;

	const unsigned char* data=m_pgm.getData();
	if(!data)
		return S_OK;

	m_Dib.Create(m_pgm.getCols()*m_nScaleTimes,m_pgm.getRows()*m_nScaleTimes);

	int nDibWidth=m_Dib.Width();
	int nDibHeight=m_Dib.Height();
	int r,g,b;
	int i=0,k=0;
	for(i=0; i<nDibHeight; i++)
	{
		unsigned char* p=m_Dib.GetLinePtr(i);
		for(int j=0; j<nDibWidth; j++,k++,p+=3)
		{
			r=g=b=min(255,max(0,data[k]+m_nOffsetBrightness));
			p[0]=b;//B
			p[1]=g;//G
			p[2]=r;//R
		}
	}
	m_Dib.BitBlt(di.hdcDraw,rc.left,rc.top,rc.right,rc.bottom,0,0);

	return S_OK;
}
STDMETHODIMP CPGMView::GetWidth(LONG* nWidth)
{
	// TODO: Add your implementation code here

	*nWidth=m_pgm.getCols();
	return S_OK;
}

STDMETHODIMP CPGMView::GetHeight(LONG* nHeight)
{
	// TODO: Add your implementation code here
	*nHeight=m_pgm.getRows();
	return S_OK;
}

STDMETHODIMP CPGMView::IncreaseBrightness(CHAR nQuantity)
{
	// TODO: Add your implementation code here
	m_nOffsetBrightness+=nQuantity;
	Invalidate();
	return S_OK;
}

STDMETHODIMP CPGMView::DecreaseBrightness(CHAR nQuantity)
{
	// TODO: Add your implementation code here
	m_nOffsetBrightness-=nQuantity;
	Invalidate();
	return S_OK;
}
