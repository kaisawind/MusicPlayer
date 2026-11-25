/********************************************************************************
* @file		:	PlayerView.h
* @summary	:	MusicPlayer视图控制类的实现。
* @author	:	Li Jiachun
* @time		:	2013-3-29 20:48:05
* @history	:	
********************************************************************************/
#include "stdafx.h"
#include "PlayerView.h"
#include "Range.h"
#include <algorithm>

namespace
{
const TCHAR szButtonClass[] = TEXT("button");
const TCHAR szScrollBarClass[] = TEXT("scrollbar");
const TCHAR szListBox[] = TEXT("listbox");
const TCHAR szStaticClass[] = TEXT("static");

const TCHAR szREWTitle[] = TEXT("快退");
const TCHAR szFFTitle[] = TEXT("快进");
const TCHAR szPlayPauseTitle[2][3] = {TEXT("播放"), TEXT("暂停")};
const TCHAR szStopTitle[] = TEXT("停止");
const TCHAR szUPTitle[] = TEXT("上一曲");
const TCHAR szDOWNTitle[] = TEXT("下一曲");

const RECT rectBegin = {110, 410, 190, 430};
const RECT rectEnd = {610, 410, 690, 430};
const RECT rectCurrent = {360, 410, 440, 430};

const DWORD LBS_USER = LBS_NOTIFY | LBS_SORT | WS_BORDER;

// 安全读取列表文本：先查长度，失败或超出缓冲区返回 LB_ERR。
int GetListText(HWND hwndList, int iIndex, TCHAR* szText, int cchText)
{
	if ((NULL == hwndList) || (NULL == szText) || (cchText <= 0))
	{
		return LB_ERR;
	}

	int iCount = (int)SendMessage(hwndList, LB_GETCOUNT, 0, 0);
	if ((iIndex < 0) || (iIndex >= iCount))
	{
		return LB_ERR;
	}

	int iLength = (int)SendMessage(hwndList, LB_GETTEXTLEN, iIndex, 0);
	if ((LB_ERR == iLength) || (iLength >= cchText))
	{
		return LB_ERR;
	}

	return (int)SendMessage(hwndList, LB_GETTEXT, iIndex, (LPARAM)szText);
}
}

// 只保存实例和主窗口句柄；子控件通过 ID 查找。
CPlayerView::CPlayerView(HINSTANCE hInst, HWND hWnd)
{
	this->m_hInst = hInst;
	this->m_hWnd = hWnd;
}

/*****************************************************************************
* @func		:	CreateButtonPreSong
* @summary	:	创建上一曲的Button
* @in		:	void
* @out		:	none
* @return	:	TRUE	: 创建成功
*			:	FALSE	: 创建失败
* @author	:	Li Jiachun
* @time		:	2013-3-29 20:36:36
* @history	:	
******************************************************************************/
int CPlayerView::CreateButtonPreSong()
{
	int iRet = FALSE;
	HWND hwndPreSong = CreateWindow(szButtonClass, 	// Button类名
	   szUPTitle, 									// 标题栏文本
	   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 		// 窗口样式
	   150,			 								// x
	   450, 										// y
	   100,											// 宽
	   30, 											// 高
	   m_hWnd, 										// 父窗口
	   (HMENU)BUTTON_PRESONG_ID, 					// 菜单
	   m_hInst, 									// 实例句柄
	   NULL);
	if (NULL != hwndPreSong)
	{
		iRet = TRUE;
	}
	return iRet;
}

/*****************************************************************************
* @func		:	CreateButtonNextSong
* @summary	:	创建下一曲的Button
* @in		:	void
* @out		:	none
* @return	:	TRUE	: 创建成功
*			:	FALSE	: 创建失败
* @author	:	Li Jiachun
* @time		:	2013-3-29 20:36:36
* @history	:	
******************************************************************************/
int CPlayerView::CreateButtonNextSong()
{
	int iRet = FALSE;
	HWND hwndNextSong = CreateWindow(szButtonClass, // Button类名
	   szDOWNTitle, 								// 标题栏文本
	   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 		// 窗口样式
	   550,			 								// x
	   450, 										// y
	   100,											// 宽
	   30, 											// 高
	   m_hWnd, 										// 父窗口
	   (HMENU)BUTTON_NEXTSONG_ID, 					// 菜单
	   m_hInst, 									// 实例句柄
	   NULL);										// lpParam
	if (NULL != hwndNextSong)
	{
		iRet = TRUE;
	}
	return iRet;
}

/*****************************************************************************
* @func		:	CreateButtonREW
* @summary	:	创建快退的Button
* @in		:	void
* @out		:	none
* @return	:	TRUE	: 创建成功
*			:	FALSE	: 创建失败
* @author	:	Li Jiachun
* @time		:	2013-3-29 20:36:36
* @history	:	
******************************************************************************/
int CPlayerView::CreateButtonREW()
{
	int iRet = FALSE;
	HWND hwndREW = CreateWindow(szButtonClass, 		// Button类名
	   szREWTitle, 									// 标题栏文本
	   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 		// 窗口样式
	   150,			 								// x
	   510, 										// y
	   100,											// 宽
	   30, 											// 高
	   m_hWnd, 										// 父窗口
	   (HMENU)BUTTON_REW_ID, 						// 菜单
	   m_hInst, 									// 实例句柄
	   NULL);										// lpParam

	if (NULL != hwndREW)
	{
		iRet = TRUE;
	}
	return iRet;
}

/*****************************************************************************
* @func		:	CreateButtonFF
* @summary	:	创建快进的Button
* @in		:	void
* @out		:	none
* @return	:	TRUE	: 创建成功
*			:	FALSE	: 创建失败
* @author	:	Li Jiachun
* @time		:	2013-3-29 20:36:36
* @history	:	
******************************************************************************/
int CPlayerView::CreateButtonFF()
{
	int iRet = FALSE;
	HWND hwndFF = CreateWindow(szButtonClass, 		// Button类名
	   szFFTitle, 									// 标题栏文本
	   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 		// 窗口样式
	   550,			 								// x
	   510, 										// y
	   100,											// 宽
	   30, 											// 高
	   m_hWnd, 										// 父窗口
	   (HMENU)BUTTON_FF_ID, 						// 菜单
	   m_hInst, 									// 实例句柄
	   NULL);

	if (NULL != hwndFF)
	{
		iRet = TRUE;
	}
	return iRet;
}

/*****************************************************************************
* @func		:	CreateButtonStop
* @summary	:	创建停止的Button
* @in		:	void
* @out		:	none
* @return	:	TRUE	: 创建成功
*			:	FALSE	: 创建失败
* @author	:	Li Jiachun
* @time		:	2013-3-29 20:36:36
* @history	:	
******************************************************************************/
int CPlayerView::CreateButtonStop()
{
	int iRet = FALSE;
	HWND hwndStop = CreateWindow(szButtonClass, 	// Button类名
	   szStopTitle, 								// 标题栏文本
	   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 		// 窗口样式
	   350,			 								// x
	   510, 										// y
	   100,											// 宽
	   30, 											// 高
	   m_hWnd, 										// 父窗口
	   (HMENU)BUTTON_STOP_ID, 						// 菜单
	   m_hInst, 									// 实例句柄
	   NULL);										// lpParam

	if (NULL != hwndStop)
	{
		iRet = TRUE;
	}

	return iRet;
}

/*****************************************************************************
* @func		:	CreateButtonPlayPause
* @summary	:	创建播放暂停的Button
* @in		:	void
* @out		:	none
* @return	:	TRUE	: 创建成功
*			:	FALSE	: 创建失败
* @author	:	Li Jiachun
* @time		:	2013-3-29 20:36:36
* @history	:	
******************************************************************************/
int CPlayerView::CreateButtonPlayPause()
{
	int iRet = FALSE;
	HWND hwndPlayPause = CreateWindow(szButtonClass,// Button类名
	   szPlayPauseTitle[0], 						// 标题栏文本
	   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 		// 窗口样式
	   350,			 								// x
	   450, 										// y
	   100,											// 宽
	   30, 											// 高
	   m_hWnd, 										// 父窗口
	   (HMENU)BUTTON_PLAYPAUSE_ID, 					// 菜单
	   m_hInst, 									// 实例句柄
	   NULL);										// lpParam

	if (NULL != hwndPlayPause)
	{
		iRet = TRUE;
	}

	return iRet;
}

/*****************************************************************************
* @func		:	CreateScrollList
* @summary	:	创建歌曲列表的滚动条
* @in		:	void
* @out		:	none
* @return	:	TRUE	: 创建成功
*			:	FALSE	: 创建失败
* @author	:	Li Jiachun
* @time		:	2013-3-29 20:36:36
* @history	:	
******************************************************************************/
int CPlayerView::CreateScrollList()
{
	int iRet = FALSE;
	HWND hwndScrollList = CreateWindow(szScrollBarClass,	// Button类名
		NULL,												// 标题栏文本
		WS_CHILD | WS_VISIBLE | SBS_VERT,					// 窗口样式
		60,													// x
		30,													// y
		40,													// 宽
		340,												// 高
		m_hWnd,												// 父窗口
		(HMENU)SCROLL_LIST_ID,								// 菜单
		m_hInst,											// 实例句柄
		NULL);												// lpParam

	if (NULL != hwndScrollList)
	{
		iRet = TRUE;
	}
	return iRet;
}

/*****************************************************************************
* @func		:	CreateScrollTime
* @summary	:	创建歌曲播放时间进度条
* @in		:	void
* @out		:	none
* @return	:	TRUE	: 创建成功
*			:	FALSE	: 创建失败
* @author	:	Li Jiachun
* @time		:	2013-3-29 20:36:36
* @history	:	
******************************************************************************/
int CPlayerView::CreateScrollTime()
{
	int iRet = FALSE;
	HWND hwndScrollTime = CreateWindow(szScrollBarClass,	// Button类名
		NULL,												// 标题栏文本
		WS_CHILD | WS_VISIBLE | SBS_HORZ,					// 窗口样式
		110,												// x
		390,												// y
		570,												// 宽
		10,													// 高
		m_hWnd,												// 父窗口
		(HMENU)SCROLL_TIME_ID,								// 菜单
		m_hInst,											// 实例句柄
		NULL												// lpParam
		);
	if (NULL != hwndScrollTime)
	{
		iRet = TRUE;
	}

	//EnableScrollBar(hwndScrollTime, SB_CTL, ESB_DISABLE_BOTH);
	return iRet;
}

/*****************************************************************************
* @func		:	CreateScrollVolume
* @summary	:	创建声音大小的滚动条
* @in		:	void
* @out		:	none
* @return	:	TRUE	: 创建成功
*			:	FALSE	: 创建失败
* @author	:	Li Jiachun
* @time		:	2013-3-29 20:36:36
* @history	:	
******************************************************************************/
int CPlayerView::CreateScrollVolume()
{
	int iRet = FALSE;
	HWND hwndScrollVolume = CreateWindow(szScrollBarClass,	// Button类名
		NULL,												// 标题栏文本
		WS_CHILD | WS_VISIBLE | SBS_VERT,					// 窗口样式
		700,												// x
		30,													// y
		40,													// 宽
		340,												// 高
		m_hWnd,												// 父窗口
		(HMENU)SCROLL_VOLUME_ID,							// 菜单
		m_hInst,											// 实例句柄
		NULL												// lpParam
		);

	if (NULL != hwndScrollVolume)
	{
		iRet = TRUE;
	}
	return iRet;
}

/*****************************************************************************
* @func		:	CreateListTable
* @summary	:	创建歌曲播放列表
* @in		:	void
* @out		:	none
* @return	:	TRUE	: 创建成功
*			:	FALSE	: 创建失败
* @author	:	Li Jiachun
* @time		:	2013-3-29 20:36:36
* @history	:	
******************************************************************************/
int CPlayerView::CreateListTable()
{
	int iRet = FALSE;

	HWND hwndListTable = CreateWindow(szListBox,			// Button类名
		NULL,												// 标题栏文本
		WS_CHILD | WS_VISIBLE | LBS_USER,					// 窗口样式
		120,												// x
		30,													// y
		550,												// 宽
		340,												// 高
		m_hWnd,												// 父窗口
		(HMENU)LIST_TABLE_ID,								// 菜单
		m_hInst,											// 实例句柄
		NULL												// lpParam
		);

	if (NULL != hwndListTable)
	{
		iRet = TRUE;
	}
	return iRet;
}

/*****************************************************************************
* @func		:	CreateListPath
* @summary	:	创建显示当前播放的歌曲的路径
* @in		:	void
* @out		:	none
* @return	:	TRUE	: 创建成功
*			:	FALSE	: 创建失败
* @author	:	Li Jiachun
* @time		:	2013-3-29 20:36:36
* @history	:	
******************************************************************************/
int CPlayerView::CreateListPath()
{
	int iRet = FALSE;

	HWND hwndListPath = CreateWindow(szStaticClass,			// Button类名
		NULL,												// 标题栏文本
		WS_CHILD | WS_VISIBLE | SS_LEFT,					// 窗口样式
		120,												// x
		10,													// y
		550,												// 宽
		20,													// 高
		m_hWnd,												// 父窗口
		(HMENU)STATIC_LISTPATH_ID,							// 菜单
		m_hInst,											// 实例句柄
		NULL												// lpParam
		);

	if (NULL != hwndListPath)
	{
		iRet = TRUE;
	}
	return iRet;
}

/*****************************************************************************
* @func		:	ShowPlayOrPause
* @summary	:	显示播放暂停的文言
* @in		:	bool bPlay = true : 暂停
*			:	bool bPlay = false : 播放
* @out		:	none
* @return	:	void
* @author	:	Li Jiachun
* @time		:	2013-3-29 20:43:03
* @history	:	
******************************************************************************/
void CPlayerView::ShowPlayOrPause(bool bPlay)
{
	HWND hwndChild = GetDlgItem(m_hWnd, BUTTON_PLAYPAUSE_ID);

	if (bPlay)
	{
		SetWindowText(hwndChild, szPlayPauseTitle[1]);
	}
	else
	{
		SetWindowText(hwndChild, szPlayPauseTitle[0]);
	}

	return ;
}

/*****************************************************************************
* @func		:	InitListTable
* @summary	:	初始化歌曲播放列表
* @in		:	void
* @out		:	none
* @return	:	void
* @author	:	Li Jiachun
* @time		:	2013-3-29 20:36:36
* @history	:	
******************************************************************************/
void CPlayerView::InitListTable()
{
	TCHAR szBuffer[MAX_LOADSTRING + 1];
	HWND hwndListPath = GetDlgItem(m_hWnd, STATIC_LISTPATH_ID);
	GetCurrentDirectory(MAX_LOADSTRING, szBuffer);
	SetWindowText(hwndListPath, szBuffer);

	HWND hwndChild = GetDlgItem(m_hWnd, LIST_TABLE_ID);
	SendMessage(hwndChild, LB_RESETCONTENT, 0, 0);
	SendMessage(hwndChild, LB_DIR, DDL_READWRITE, (LPARAM)TEXT("*.MP3"));
	SendMessage(hwndChild, LB_DIR, DDL_READWRITE, (LPARAM)TEXT("*.WAV"));
	SendMessage(hwndChild, LB_SETCURSEL, 0, TRUE);

	return ;
}

// 按文件名查找并选中列表项；找不到返回 LB_ERR。
int CPlayerView::SelectListName(const TCHAR* szName)
{
	HWND hwndListTable = GetDlgItem(m_hWnd, LIST_TABLE_ID);
	int iIndex = (int)SendMessage(hwndListTable, LB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)szName);
	if (LB_ERR != iIndex)
	{
		SendMessage(hwndListTable, LB_SETCURSEL, iIndex, 0);
	}
	return iIndex;
}

/*****************************************************************************
* @func		:	InitWindows
* @summary	:	窗口初始化。
* @in		:	void
* @out		:	none
* @return	:	TRUE	: 各窗口创建成功
*			:	FALSE	: 窗口创建不成功
* @author	:	Li Jiachun
* @time		:	2013-3-24 21:02:57
* @history	:	
******************************************************************************/
int CPlayerView::InitWindows()
{
	if (CreateButtonFF()		&& 
		CreateButtonNextSong()	&& 
		CreateButtonPlayPause() && 
		CreateButtonPreSong()	&& 
		CreateButtonREW()		&& 
		CreateButtonStop()		&& 
		CreateListTable()		&& 
		CreateScrollList()		&& 
		CreateScrollTime()		&& 
		CreateScrollVolume()	&&
		CreateListPath())
	{
		return TRUE;
	}

	return FALSE;
}

/*****************************************************************************
* @func		:	GetSongsCount
* @summary	:	获取列表中所有歌曲的数目
* @in		:	void
* @out		:	none
* @return	:	int iNo	: 歌曲的数目
* @author	:	Li Jiachun
* @time		:	2013-3-30 11:30:04
* @history	:	
******************************************************************************/
int CPlayerView::GetSongsCount()
{
	int iNo = 0;
	HWND hwndListTable = GetDlgItem(m_hWnd, LIST_TABLE_ID);
	iNo = SendMessage(hwndListTable, LB_GETCOUNT, 0, 0);
	return iNo;
}

/*****************************************************************************
* @func		:	GetListTopIndex
* @summary	:	获取列表中第一条的索引值
* @in		:	void
* @out		:	none
* @return	:	int iIndex	: 列表中第一条的索引值
* @author	:	Li Jiachun
* @time		:	2013-3-30 11:30:04
* @history	:	
******************************************************************************/
int CPlayerView::GetListTopIndex()
{
	int iIndex = 0;
	HWND hwndListTable = GetDlgItem(m_hWnd, LIST_TABLE_ID);
	iIndex = SendMessage(hwndListTable, LB_GETTOPINDEX, 0, 0);
	return iIndex;
}

/*****************************************************************************
* @func		:	SetListTopIndex
* @summary	:	设置列表中第一条的索引值
* @in		:	int iIndex : 将要设置的列表中第一条的索引值
* @out		:	none
* @return	:	void
* @author	:	Li Jiachun
* @time		:	2013-3-30 11:30:04
* @history	:	
******************************************************************************/
void CPlayerView::SetListTopIndex(int iIndex)
{
	HWND hwndListTable = GetDlgItem(m_hWnd, LIST_TABLE_ID);
	SendMessage(hwndListTable, LB_SETTOPINDEX, iIndex, 0);
	return ;
}

/*****************************************************************************
* @func		:	InitScrollList
* @summary	:	初始化列表滚动条
* @in		:	void
* @out		:	none
* @return	:	void
* @author	:	Li Jiachun
* @time		:	2013-3-30 11:30:04
* @history	:	
******************************************************************************/
void CPlayerView::InitScrollList()
{
	TEXTMETRIC tm;
	RECT rect;
	HWND hwndScrollList = GetDlgItem(m_hWnd, SCROLL_LIST_ID);
	HDC hdc = GetDC(hwndScrollList);
	if (NULL == hdc)
	{
		return ;
	}

	if (!GetWindowRect(hwndScrollList, &rect) || !GetTextMetrics(hdc, &tm) || (0 == tm.tmHeight))
	{
		ReleaseDC(hwndScrollList, hdc);
		return ;
	}

	int iyChar = tm.tmHeight;
	int iMax = GetSongsCount() - (rect.bottom - rect.top) / iyChar;

	iMax = std::max(0, iMax);

	SetScrollRange(hwndScrollList, SB_CTL, 0, iMax, FALSE);
	SetScrollPos(hwndScrollList, SB_CTL, GetListTopIndex(), TRUE);
	ReleaseDC(hwndScrollList, hdc);
	return ;
}

/*****************************************************************************
* @func		:	UpdateScrollList
* @summary	:	按滚动条消息更新列表位置
*			:	WndProcのWM_VSCROLL消息处用
* @in		:	WPARAM wParam : 滚动条消息
*			:	LPARAM lParam : 滚动条句柄
* @out		:	none
* @return	:	void
* @author	:	Li Jiachun
* @time		:	2013-3-30 11:30:04
* @history	:	
******************************************************************************/
void CPlayerView::UpdateScrollList(WPARAM wParam, LPARAM lParam)
{
	if (SCROLL_LIST_ID != GetWindowLong((HWND)lParam, GWL_ID))
	{
		return ;
	}
	int iMin = 0;
	int iMax = 0;
	int iIndex = (int)GetScrollPos((HWND)lParam, SB_CTL);
	GetScrollRange((HWND)lParam, SB_CTL, &iMin, &iMax);

	switch(LOWORD(wParam))
	{
	case SB_PAGEDOWN:
		iIndex += 10;
		break;
	case SB_PAGEUP:
		iIndex -= 10;
		break;
	case SB_LINEDOWN:
		iIndex++;
		break;
	case SB_LINEUP:
		iIndex--;
		break;
	case SB_TOP:
		iIndex = iMin;
		break;
	case SB_BOTTOM:
		iIndex = iMax;
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		iIndex = HIWORD(wParam);
		break;
	default:
		break;
	}

	iIndex = ClampInt(iIndex, iMin, iMax);

	SetScrollPos((HWND)lParam, SB_CTL, iIndex, TRUE);
	SetListTopIndex(iIndex);
	return ;
}

/*****************************************************************************
* @func		:	GetCurrentSelectListName
* @summary	:	获取当前选中的歌曲名
* @in		:	TCHAR* szCurrentSelectName : 输出缓冲区
*			:	int cchCurrentSelectName : 缓冲区长度
* @out		:	TCHAR* szCurrentSelectName : 当前选中的歌曲名
* @return	:	int : 名称长度，失败返回 LB_ERR
* @author	:	Li Jiachun
* @time		:	2013-3-31 19:37:18
* @history	:	
******************************************************************************/
int CPlayerView::GetCurrentSelectListName(TCHAR* szCurrentSelectName, int cchCurrentSelectName)
{
	HWND hwndListTable = GetDlgItem(m_hWnd, LIST_TABLE_ID);
	int iCurIndex = (int)SendMessage(hwndListTable, LB_GETCURSEL, 0, 0);
	return GetListText(hwndListTable, iCurIndex, szCurrentSelectName, cchCurrentSelectName);
}

/*****************************************************************************
* @func		:	UpdateScrollListByMouse
* @summary	:	鼠标滚轮更新播放列表
* @in		:	int iDeltaPerLine : 滚轮每格对应的滚动量
*			:	bool bPageScroll : 是否按页滚动
*			:	UINT message : 原始鼠标消息
*			:	WPARAM wParam : 原始 wParam
*			:	LPARAM lParam : 原始 lParam
* @out		:	none
* @return	:	NULL	: 已处理
*			:	DefWindowProc	: 未处理
* @author	:	Li Jiachun
* @time		:	2013-3-31 19:39:06
* @history	:	
******************************************************************************/
LRESULT CPlayerView::UpdateScrollListByMouse(int iDeltaPerLine, bool bPageScroll, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (0 == iDeltaPerLine)
	{
		return DefWindowProc(m_hWnd, message, wParam, lParam);
	}

	HWND hwndScrollList = GetDlgItem(m_hWnd, SCROLL_LIST_ID);
	WPARAM wUp = bPageScroll ? SB_PAGEUP : SB_LINEUP;
	WPARAM wDown = bPageScroll ? SB_PAGEDOWN : SB_LINEDOWN;

	int iAccumDelta = (short)HIWORD(wParam);

	// 向下滚动鼠标
	while (iAccumDelta >= iDeltaPerLine)
	{
		SendMessage(m_hWnd, WM_VSCROLL, wUp, (LPARAM)hwndScrollList);
		iAccumDelta -= iDeltaPerLine;
	}

	// 向上滚动鼠标
	while(iAccumDelta <= -iDeltaPerLine)
	{
		SendMessage(m_hWnd, WM_VSCROLL, wDown, (LPARAM)hwndScrollList);
		iAccumDelta += iDeltaPerLine;
	}
	return NULL;
}

/*****************************************************************************
* @func		:	UpdateRectBeginTime
* @summary	:	更新开始时间区域的时间显示
* @in		:	HDC hdc : 绘制设备上下文
*			:	const TCHAR szBeginTime[] : 开始时间
* @out		:	none
* @return	:	void
* @author	:	Li Jiachun
* @time		:	2013-4-3 21:53:53
* @history	:	
******************************************************************************/
void CPlayerView::UpdateRectBeginTime(HDC hdc, const TCHAR szBeginTime[])
{
	if (NULL == hdc)
	{
		return ;
	}

	TextOut(hdc, rectBegin.left, rectBegin.top, szBeginTime, lstrlen(szBeginTime));
	return ;
}

/*****************************************************************************
* @func		:	UpdateRectTotalTime
* @summary	:	更新总时间的显示
* @in		:	HDC hdc : 绘制设备上下文
*			:	const TCHAR szTotalTime[] : 总时间
* @out		:	none
* @return	:	void
* @author	:	Li Jiachun
* @time		:	2013-4-3 21:53:53
* @history	:	
******************************************************************************/
void CPlayerView::UpdateRectTotalTime(HDC hdc, const TCHAR szTotalTime[])
{
	if (NULL == hdc)
	{
		return ;
	}

	TextOut(hdc, rectEnd.left, rectEnd.top, szTotalTime, lstrlen(szTotalTime));
	return ;
}

/*****************************************************************************
* @func		:	UpdateRectCurreTime
* @summary	:	更新当前时间的显示
* @in		:	HDC hdc : 绘制设备上下文
*			:	const TCHAR szCurrentTime[] : 当前时间
* @out		:	none
* @return	:	void
* @author	:	Li Jiachun
* @time		:	2013-4-3 21:53:53
* @history	:	
******************************************************************************/
void CPlayerView::UpdateRectCurreTime(HDC hdc, const TCHAR szCurrentTime[])
{
	if (NULL == hdc)
	{
		return ;
	}

	TextOut(hdc, rectCurrent.left, rectCurrent.top, szCurrentTime, lstrlen(szCurrentTime));
	return ;
}

/*****************************************************************************
* @func		:	InvalidateRectCurrentTime
* @summary	:	是当前时间区域块无效
* @in		:	void
* @out		:	none
* @return	:	void
* @author	:	Li Jiachun
* @time		:	2013-4-3 21:53:53
* @history	:	
******************************************************************************/
void CPlayerView::InvalidateRectCurrentTime()
{
	InvalidateRect(m_hWnd, &rectCurrent, TRUE);
	return ;
}

/*****************************************************************************
* @func		:	InvalidateRectTotalTime
* @summary	:	使总时间区域块无效
* @in		:	void
* @out		:	none
* @return	:	void
* @author	:	Li Jiachun
* @time		:	2013-4-3 21:53:53
* @history	:	
******************************************************************************/
void CPlayerView::InvalidateRectTotalTime()
{
	InvalidateRectCurrentTime();
	InvalidateRect(m_hWnd, &rectEnd, TRUE);
	return ;
}

/*****************************************************************************
* @func		:	InitScrollTime
* @summary	:	初始化时间滚动条
* @in		:	void
* @out		:	none
* @return	:	void
* @author	:	Li Jiachun
* @time		:	2013-4-3 21:53:53
* @history	:	
******************************************************************************/
void CPlayerView::InitScrollTime(int iTotalTime)
{
	HWND hwndScrollTime = GetDlgItem(m_hWnd, SCROLL_TIME_ID);

	SetScrollRange(hwndScrollTime, SB_CTL, 0, iTotalTime, FALSE);
	SetScrollPos(hwndScrollTime, SB_CTL, 0, TRUE);
	return ;
}

/*****************************************************************************
* @func		:	GetPreviousSongName
* @summary	:	获取上一首歌的名字
* @in		:	TCHAR* szPreSongName : 输出缓冲区
*			:	int cchPreSongName : 缓冲区长度
* @out		:	TCHAR* szPreSongName : 上一首歌的名字
* @return	:	int : 名称长度，失败返回 LB_ERR
* @author	:	Li Jiachun
* @time		:	2013-4-3 21:53:53
* @history	:	
******************************************************************************/
int CPlayerView::GetPreviousSongName(TCHAR* szPreSongName, int cchPreSongName)
{
	HWND hwndListTable = GetDlgItem(m_hWnd, LIST_TABLE_ID);
	int iCount = (int)SendMessage(hwndListTable, LB_GETCOUNT, 0, 0);
	if (iCount <= 0)
	{
		return LB_ERR;
	}

	int iCurIndex = (int)SendMessage(hwndListTable, LB_GETCURSEL, 0, 0);
	if (LB_ERR == iCurIndex)
	{
		iCurIndex = 0;
	}

	int iPreSongIndex = PrevIndex(iCurIndex, iCount);
	return GetListText(hwndListTable, iPreSongIndex, szPreSongName, cchPreSongName);
}

/*****************************************************************************
* @func		:	GetNextSongName
* @summary	:	获取下一首歌的名字
* @in		:	TCHAR* szNextName : 输出缓冲区
*			:	int cchNextName : 缓冲区长度
* @out		:	TCHAR* szNextName : 下一首歌的名字
* @return	:	int : 名称长度，失败返回 LB_ERR
* @author	:	Li Jiachun
* @time		:	2013-4-3 21:53:53
* @history	:	
******************************************************************************/
int CPlayerView::GetNextSongName(TCHAR* szNextName, int cchNextName)
{
	HWND hwndListTable = GetDlgItem(m_hWnd, LIST_TABLE_ID);
	int iCount = (int)SendMessage(hwndListTable, LB_GETCOUNT, 0, 0);
	if (iCount <= 0)
	{
		return LB_ERR;
	}

	int iCurIndex = (int)SendMessage(hwndListTable, LB_GETCURSEL, 0, 0);
	int iNextSongIndex = NextIndex(iCurIndex, iCount);
	return GetListText(hwndListTable, iNextSongIndex, szNextName, cchNextName);
}

/*****************************************************************************
* @func		:	SetPreviousIndex
* @summary	:	将当前的歌曲选择项设为上一首歌
* @in		:	void
* @out		:	none
* @return	:	void
* @author	:	Li Jiachun
* @time		:	2013-4-3 21:53:53
* @history	:	
******************************************************************************/
void CPlayerView::SetPreviousIndex()
{
	HWND hwndListTable = GetDlgItem(m_hWnd, LIST_TABLE_ID);
	int iCount = (int)SendMessage(hwndListTable, LB_GETCOUNT, 0, 0);
	if (iCount <= 0)
	{
		return ;
	}

	int iCurIndex = (int)SendMessage(hwndListTable, LB_GETCURSEL, 0, 0);
	if (LB_ERR == iCurIndex)
	{
		iCurIndex = 0;
	}

	int iIndex = PrevIndex(iCurIndex, iCount);
	SendMessage(hwndListTable, LB_SETCURSEL, iIndex, 0);
	return ;
}

/*****************************************************************************
* @func		:	SetNextIndex
* @summary	:	将当前的歌曲选择项设为下一首歌
* @in		:	void
* @out		:	none
* @return	:	void
* @author	:	Li Jiachun
* @time		:	2013-4-3 21:53:53
* @history	:	
******************************************************************************/
void CPlayerView::SetNextIndex()
{
	HWND hwndListTable = GetDlgItem(m_hWnd, LIST_TABLE_ID);
	int iCount = (int)SendMessage(hwndListTable, LB_GETCOUNT, 0, 0);
	if (iCount <= 0)
	{
		return ;
	}

	int iCurIndex = (int)SendMessage(hwndListTable, LB_GETCURSEL, 0, 0);
	int iIndex = NextIndex(iCurIndex, iCount);
	SendMessage(hwndListTable, LB_SETCURSEL, iIndex, 0);
	return ;
}

// 初始化音量滚动条位置（0-100）。
void CPlayerView::InitScrollVolume(unsigned int iVolume)
{
	iVolume = (unsigned int)ClampInt((int)iVolume, 0, MAX_PLAYER_VOLUME);

	HWND hwndScrollVolume = GetDlgItem(m_hWnd, SCROLL_VOLUME_ID);
	SetScrollRange(hwndScrollVolume, SB_CTL, 0, MAX_PLAYER_VOLUME, FALSE);
	SetScrollPos(hwndScrollVolume, SB_CTL, MAX_PLAYER_VOLUME - iVolume, TRUE);
	return ;
}

// 按滚动条消息更新音量，结果夹在 0-100。
void CPlayerView::UpdateScrollVolume(unsigned int& iVolume, WPARAM wParam, LPARAM lParam)
{
	if (SCROLL_VOLUME_ID != GetWindowLong((HWND)lParam, GWL_ID))
	{
		return ;
	}

	iVolume = (unsigned int)ClampInt((int)iVolume, 0, MAX_PLAYER_VOLUME);

	int iIndex = MAX_PLAYER_VOLUME - (int)GetScrollPos((HWND)lParam, SB_CTL);
	switch(LOWORD(wParam))
	{
	case SB_PAGEDOWN:
		iIndex--;
		break;
	case SB_PAGEUP:
		iIndex++;
		break;
	case SB_LINEDOWN:
		iIndex--;
		break;
	case SB_LINEUP:
		iIndex++;
		break;
	case SB_TOP:
		iIndex = MAX_PLAYER_VOLUME;
		break;
	case SB_BOTTOM:
		iIndex = 0;
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		iIndex = MAX_PLAYER_VOLUME - HIWORD(wParam);
		break;
	default:
		break;
	}

	iIndex = ClampInt(iIndex, 0, MAX_PLAYER_VOLUME);

	SetScrollPos((HWND)lParam, SB_CTL, MAX_PLAYER_VOLUME - iIndex, TRUE);
	iVolume = (unsigned int)iIndex;
	return ;
}

// 更新进度条位置（毫秒）。
void CPlayerView::UpdateScrollTime(unsigned int iTime)
{
	HWND hwndScrollTime = GetDlgItem(m_hWnd, SCROLL_TIME_ID);
	SetScrollPos(hwndScrollTime, SB_CTL, iTime, TRUE);
	return ;
}