// ZListEdit.cpp: 实现文件
//

#include "stdafx.h"
#include "ZListEdit.h"


// ZListEdit

IMPLEMENT_DYNAMIC(ZListEdit, CEdit)

ZListEdit::ZListEdit()
{

}

ZListEdit::~ZListEdit()
{
}


BEGIN_MESSAGE_MAP(ZListEdit, CEdit)
	ON_CONTROL_REFLECT(EN_SETFOCUS, &ZListEdit::OnEnSetfocus)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &ZListEdit::OnEnKillfocus)
END_MESSAGE_MAP()



// ZListEdit 消息处理程序




void ZListEdit::OnEnSetfocus()
{
	// TODO: 在此添加控件通知处理程序代码
	m_hIMC = ImmAssociateContext(m_hWnd, NULL);//禁用输入法
}


void ZListEdit::OnEnKillfocus()
{
	// TODO: 在此添加控件通知处理程序代码
	ImmAssociateContext(m_hWnd, m_hIMC);//启用输入法
	if (GetParent())
		GetParent()->PostMessage(WM_EDITKILLFOCUS, 0, 0);
}


BOOL ZListEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		if (::GetFocus() == m_hWnd)
		{
			CString str, strChar;
			if (::GetKeyState(VK_CONTROL)<0)
				str += _T("Ctrl+");
			if (::GetKeyState(VK_MENU)<0)
				str += _T("Alt+");
			if (::GetKeyState(VK_SHIFT)<0)
				str += _T("Shift+");
			UINT nKey = pMsg->wParam;
			if (nKey == VK_PRIOR || nKey == VK_NEXT || nKey == VK_SPACE || nKey == VK_RETURN || nKey == VK_LEFT || nKey == VK_RIGHT || nKey == VK_UP || nKey == VK_DOWN || (nKey >= '0'&&nKey <= '9') || (nKey >= 'A'&&nKey <= 'Z') || (nKey >= VK_NUMPAD0 && nKey <= VK_NUMPAD9))
			{
				if (nKey >= VK_NUMPAD0 && nKey <= VK_NUMPAD9)
				{
					strChar.Format(_T("小键盘%d"), nKey - VK_NUMPAD0);
				}
				else
				{
					switch (nKey)
					{
					case VK_PRIOR:
						strChar = _T("PageUp");
						break;
					case VK_NEXT:
						strChar = _T("PageDown");
						break;
					case VK_SPACE:
						strChar = _T("空格");
						break;
					case VK_RETURN:
						strChar = _T("回车");
						break;
					case VK_LEFT:
						strChar = _T("←");
						break;
					case VK_RIGHT:
						strChar = _T("→");
						break;
					case VK_UP:
						strChar = _T("↑");
						break;
					case VK_DOWN:
						strChar = _T("↓");
						break;
					default:
						strChar = (char)nKey;
					}
				}
				str += strChar;
				SetWindowText(str);
				SetSel(str.GetLength(), str.GetLength());
			}
			return TRUE;
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}
