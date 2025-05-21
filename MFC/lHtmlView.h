#pragma once
#include <afxhtml.h>
#include <stdarg.h>
#include "lString.h"
#include "lToken.h"
class lHtmlView :
    public CHtmlView
{
private:
    bool URL_Check(LPCTSTR);
protected:
private:
    lToken htmlData;
    lString Out;
public:
    IWebBrowser2* pBrowserApp;
    bool BrowserAppRaed;
public:
    lString GetURLData();
    void DeleteOutData();
    void SetHtml(const char*);
    void navigate(lString);
    void jsFunction(lString,...);
public:
    void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
    void OnInitialUpdate();
    DECLARE_MESSAGE_MAP()
    void OnDocumentComplete(LPCTSTR lpszURL);
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

