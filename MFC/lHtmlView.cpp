#include "pch.h"
#include "lHtmlView.h"
#include "lToken.h"

bool lHtmlView::URL_Check(LPCTSTR lpszURL)
{
	
	lString url = lpszURL, name;
	lToken tok;
	tok.Split(url, "\\");
	
	name = tok.Search(tok.GetSize() - 1); 
	if (htmlData.In(name)) {
		return false;
	}
	else {
		Out = name;
		return true;
	}
	
	return false;
}


lString lHtmlView::GetURLData()
{
	lString out;
	out = Out;
	out.Temp();
	return out;
}

void lHtmlView::DeleteOutData()
{
	Out.Delete();
}

void lHtmlView::SetHtml(const char* name)
{
	lString html;
	html.Read(name);
	htmlData.Split(html, "\r\n");
}
void lHtmlView::navigate(lString url)
{
	Navigate2(CString(url.GetC()));
}
void lHtmlView::jsFunction(lString Format, ...)
{
	va_list args;
	CComVariant* pArgs;
	lToken tok,pvtok;
	lString pv; 
	HRESULT hr;
	DISPID dispid;
	CComPtr<IDispatch> spDisp;
	CComPtr<IHTMLDocument2> spHtmlDoc;
	CComPtr<IDispatch> spScript;

	int i,n;
	if (pBrowserApp == NULL)return;
	pBrowserApp->get_Document(&spDisp);
	spDisp->QueryInterface(IID_IHTMLDocument2, (void**)&spHtmlDoc);
	
	spHtmlDoc->get_Script(&spScript);
	tok.Split(Format, "()");
	CComBSTR bstrFunctionName(tok.Search(0).GetC());
	hr = spScript->GetIDsOfNames(IID_NULL, &bstrFunctionName, 1, LOCALE_USER_DEFAULT, &dispid);
	if (FAILED(hr)){//Function Name 
		return;
	}
	pvtok.Split(tok.Search(1), ",");
	n = pvtok.GetSize();
	va_start(args,Format);
	pArgs = new CComVariant[pvtok.GetSize()];
	for (i = 0; i < pvtok.GetSize(); i++) {
		if (pvtok.Search(i).In("d")) {
			int a = va_arg(args, int);
			pArgs[pvtok.GetSize() - 1 - i] = (CComVariant)a;
		}
		if (pvtok.Search(i).In("f")) {	
			double a = va_arg(args, double);
			pArgs[pvtok.GetSize() - 1 - i] = (CComVariant)a;
		}
		if (pvtok.Search(i).In("c")) { 
			char a = va_arg(args, char);
			pArgs[pvtok.GetSize() - 1 - i] = (CComVariant)a;
		}
		if (pvtok.Search(i).In("s")) { 
			char* a = va_arg(args, char*);
			pArgs[pvtok.GetSize() - 1 - i] = (CComVariant)a;
		}
	}
	DISPPARAMS params = { pArgs, NULL, static_cast<UINT>(n), 0 };
	hr = spScript->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, nullptr, nullptr, nullptr);
	if (FAILED(hr)) {
		MessageBox(L"JavaScript �Լ� ȣ�� ����", L"Error", MB_OK | MB_ICONERROR);
	}
	va_end(args);
	delete[](pArgs);
	Format.NoTemp();
}
void lHtmlView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	*pbCancel = URL_Check(lpszURL);
	if (*pbCancel) return;
	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}


void lHtmlView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();
	PostMessage(WM_USER + 1);
	//IUnknown* pUnk = GetControlUnknown();
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}

BEGIN_MESSAGE_MAP(lHtmlView, CHtmlView)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()



void lHtmlView::OnDocumentComplete(LPCTSTR lpszURL)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	pBrowserApp = m_pBrowserApp;
	if (pBrowserApp) {
		BrowserAppRaed = true;
	}
	CHtmlView::OnDocumentComplete(lpszURL);
}


void lHtmlView::OnDestroy()
{
	CHtmlView::OnDestroy();
	int i;
	htmlData.Delete();
	KillTimer(1);
	
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void lHtmlView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nIDEvent == 1) {
	}
	CHtmlView::OnTimer(nIDEvent);
}


int lHtmlView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CHtmlView::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetTimer(1, 100, NULL);
	pBrowserApp = NULL;
	BrowserAppRaed = false;
	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return 0;
}
