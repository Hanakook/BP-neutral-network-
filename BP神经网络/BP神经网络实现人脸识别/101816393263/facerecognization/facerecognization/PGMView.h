// PGMView.h : Declaration of the CPGMView

#ifndef __PGMVIEW_H_
#define __PGMVIEW_H_

#include "resource.h"       // main symbols
#include <atlctl.h>

#include "common/PGMFile.h"
#include "DIB.h"
#include <comdef.h>

#define MAX_PATH_CHAR	200

/////////////////////////////////////////////////////////////////////////////
// CPGMView
class ATL_NO_VTABLE CPGMView : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IPGMView, &IID_IPGMView, &LIBID_FACERECOGNIZATIONLib>,
	public CComControl<CPGMView>,
	public IPersistStreamInitImpl<CPGMView>,
	public IOleControlImpl<CPGMView>,
	public IOleObjectImpl<CPGMView>,
	public IOleInPlaceActiveObjectImpl<CPGMView>,
	public IViewObjectExImpl<CPGMView>,
	public IOleInPlaceObjectWindowlessImpl<CPGMView>,
	public IPersistStorageImpl<CPGMView>,
	public ISpecifyPropertyPagesImpl<CPGMView>,
	public IQuickActivateImpl<CPGMView>,
	public IDataObjectImpl<CPGMView>,
	public IProvideClassInfo2Impl<&CLSID_PGMView, NULL, &LIBID_FACERECOGNIZATIONLib>,
	public CComCoClass<CPGMView, &CLSID_PGMView>
{
public:
	CPGMView();

DECLARE_REGISTRY_RESOURCEID(IDR_PGMVIEW)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPGMView)
	COM_INTERFACE_ENTRY(IPGMView)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_PROP_MAP(CPGMView)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_MSG_MAP(CPGMView)
	CHAIN_MSG_MAP(CComControl<CPGMView>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);



// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IPGMView
public:

	HRESULT OnDraw(ATL_DRAWINFO& di);
private:
	//TCHAR		m_sFilePath[MAX_PATH_CHAR];
	_bstr_t		m_sFilePath;
	CPGMFile	m_pgm;
	CDIB		m_Dib;
	int			m_nScaleTimes;
	int			m_nOffsetBrightness;
public:
	STDMETHOD(SetPGMFilePath)(BSTR sFilePath,VARIANT_BOOL bRefresh);
	STDMETHOD(Refresh)(void);
	STDMETHOD(GetWidth)(LONG* nWidth);
	STDMETHOD(GetHeight)(LONG* nHeight);
	STDMETHOD(IncreaseBrightness)(CHAR nQuantity);
	STDMETHOD(DecreaseBrightness)(CHAR nQuantity);
};

#endif //__PGMVIEW_H_
