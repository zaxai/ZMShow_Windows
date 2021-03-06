// ZStatic.cpp: 实现文件
//

#include "stdafx.h"
#include "ZMShow.h"
#include "ZStatic.h"


// ZStatic

IMPLEMENT_DYNAMIC(ZStatic, CStatic)

ZStatic::ZStatic()
{

}

ZStatic::~ZStatic()
{
}


BEGIN_MESSAGE_MAP(ZStatic, CStatic)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// ZStatic 消息处理程序




BOOL ZStatic::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_BKGND);
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap * p_bmpOld = dcMem.SelectObject(&bmp);
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);
	CRect rect;
	GetClientRect(&rect);
	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,SRCCOPY);
	pDC->SetStretchBltMode(HALFTONE);
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	dcMem.SelectObject(p_bmpOld);
	return TRUE;
	return CStatic::OnEraseBkgnd(pDC);
}


void ZStatic::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{

	// TODO:  添加您的代码以绘制指定项
}
