// lOpenGL.cpp: 구현 파일
//

#include "pch.h"
#include "MFC/lOpenGL.h"
#include "3d/dhMat.h"

// lOpenGL

IMPLEMENT_DYNAMIC(lOpenGL, CWnd)

lOpenGL::lOpenGL()
{

}

lOpenGL::~lOpenGL()
{
}


BEGIN_MESSAGE_MAP(lOpenGL, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_TIMER()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
//    ON_WM_NCLBUTTONUP()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_WM_MOUSELEAVE()
ON_WM_MBUTTONDOWN()
ON_WM_MBUTTONUP()
ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// lOpenGL 메시지 처리기




BOOL lOpenGL::CreateOpenGLContext()
{
    dhMat h;
    // Get device context
    int nPixelFormat;
    m_hDC = ::GetDC(m_hWnd);

    static PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0, 0,
        0, 0, 0, 0, 0,
        32,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    Cam.Initialization();
    int pixelFormat = ChoosePixelFormat(m_hDC, &pfd);
    SetPixelFormat(m_hDC, pixelFormat, &pfd);
    m_hRC = wglCreateContext(m_hDC);
    wglMakeCurrent(m_hDC, m_hRC);
    Light.SetLightColor(255, 255, 255, 255);
    Light.SetPoint(0, -10, -10);
    glewInit();
    LButtonClick = false;
    RButtonClick = false;
    glEnable(GL_DEPTH_TEST);
    anglex = 0;
    angley = 0;
    cTx = 0;
    cTy = 0;
    cT = -10;
    dhMat c;
    c = h.Trans(cTx, cTy, cT) * h.RotY(RAD(anglex)) * h.RotX(RAD(angley));
    Cam.T(c);
    o.Initialization();
    o.SetCamAndLight(&Cam, &Light);
    o.SetShader("D:\\Project\\glsl\\vertexShader.glsl", "D:\\Project\\glsl\\PhongShader.glsl");
    return TRUE;
}

void lOpenGL::GLResize(int cx, int cy)
{
    Cam.SETAspectRatioAndFOV(RAD(45), cx, cy);
    glViewport(0, 0, cx, cy);  // 뷰포트 설정
}

void lOpenGL::GLRenderScene()
{
    dhMat h;
}

void lOpenGL::GLObjectCreate()
{
    
}

void lOpenGL::GLObjectDelete()
{
}

void lOpenGL::GLDraw(CDC* pDC)
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLRenderScene();

    o.Draw();
    glFlush();
}

void lOpenGL::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
        wglMakeCurrent(m_hDC, m_hRC);
        GLDraw((CDC*)&dc);
        SwapBuffers(m_hDC);
        wglMakeCurrent(m_hDC, NULL);
}


int lOpenGL::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
    SetTimer(1, 33, NULL);
    CreateOpenGLContext();
    GLObjectCreate();
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void lOpenGL::OnDestroy()
{
	CWnd::OnDestroy();
    KillTimer(1);
    GLObjectDelete();
    o.Close();
    wglDeleteContext(m_hRC);
    ::ReleaseDC(m_hWnd, m_hDC);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL lOpenGL::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


BOOL lOpenGL::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWnd::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam);
}


void lOpenGL::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    VERIFY(wglMakeCurrent(m_hDC, m_hRC));
    this->GLResize(cx,cy);
    VERIFY(wglMakeCurrent(NULL, NULL));
    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void lOpenGL::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    Invalidate();
    CWnd::OnTimer(nIDEvent);
}


void lOpenGL::OnLButtonDown(UINT nFlags, CPoint point)
{
    LButtonClick = true;
    CWnd::OnLButtonDown(nFlags, point);
}


void lOpenGL::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    dhMat h, c;
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    int ex = point.x - oldCPoint.x;
    int ey = point.y - oldCPoint.y;
    if (RButtonClick) {
        if (ABS(ex) > ABS(ey)) {
            if (ex > 0)cTx += 0.03;
            else cTx -= 0.03;
        }
        else {
            if (ey > 0)cTy -= 0.03;
            else cTy += 0.03;
        }
    }
    if (LButtonClick) {
        if (ABS(ex) > ABS(ey)) {
            if (ex < 0)     anglex += 3;
            else              anglex -= 3;
        }
        else {
            if (ey < 0)     angley += 3;
            else              angley -= 3;
        }
        
    }
    oldCPoint = point;
    c = h.Trans(cTx, cTy, cT) * h.RotY(RAD(anglex)) * h.RotX(RAD(angley));
    Cam.T(c);
    CWnd::OnMouseMove(nFlags, point);
}

void lOpenGL::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    LButtonClick = false;
    CWnd::OnLButtonUp(nFlags, point);
}


void lOpenGL::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    
    CWnd::OnRButtonDown(nFlags, point);
}


void lOpenGL::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    
    CWnd::OnRButtonUp(nFlags, point);
}


void lOpenGL::OnMouseLeave()
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    LButtonClick = false;
    RButtonClick = false;
    CWnd::OnMouseLeave();
}


void lOpenGL::OnMButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    RButtonClick = true;
    CWnd::OnMButtonDown(nFlags, point);
}


void lOpenGL::OnMButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    RButtonClick = false;
    CWnd::OnMButtonUp(nFlags, point);
}


BOOL lOpenGL::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    if (zDelta < 0) { 
        cT -= 50;
        if (cT < -100000) {
            cT = -3000;
        }
    }
    else {
        cT += 50;
        if (cT > -5) {
            cT = -5;
        }
    } 
    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
