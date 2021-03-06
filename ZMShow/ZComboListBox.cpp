// ZComboListBox.cpp: 实现文件
//

#include "stdafx.h"
#include "ZComboListBox.h"


// ZComboListBox

IMPLEMENT_DYNAMIC(ZComboListBox, CListBox)

ZComboListBox::ZComboListBox()
{

}

ZComboListBox::~ZComboListBox()
{
}


BEGIN_MESSAGE_MAP(ZComboListBox, CListBox)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// ZComboListBox 消息处理程序




void ZComboListBox::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_tooltip.Create(this, TTS_ALWAYSTIP);
	m_tooltip.AddTool(this, _T(""));
	m_tooltip.SetDelayTime(TTDT_AUTOPOP, 3000);
	m_tooltip.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	CListBox::PreSubclassWindow();
}


BOOL ZComboListBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_tooltip.m_hWnd)
		m_tooltip.RelayEvent(pMsg);
	return CListBox::PreTranslateMessage(pMsg);
}


void ZComboListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static int nIndexLast = LB_ERR;
	int nIndex = LB_ERR;
	BOOL nIsOutside = TRUE;
	nIndex = ItemFromPoint(point, nIsOutside);
	if (!nIsOutside&&nIndex != LB_ERR)
	{
		CString str;
		GetText(nIndex, str);
		if (nIndex != nIndexLast)
		{
			m_tooltip.UpdateTipText(str, this);
			nIndexLast = nIndex;
		}
	}
	CListBox::OnMouseMove(nFlags, point);
}
