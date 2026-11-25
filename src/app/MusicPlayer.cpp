/*******************************************************************************
* @file		:	MusicPlayer.cpp
* @summary	:	MusicPlayer应用程序的入口点。
* @author	:	DaPPer
* @time		:	2013-3-24 13:27:27
* @history	:	
********************************************************************************/

#include "stdafx.h"
#include "MusicPlayer.h"

static HINSTANCE hInst;
static TCHAR szTitle[MAX_LOADSTRING];
static TCHAR szWindowClass[MAX_LOADSTRING];

static void SetWorkingDirectoryToExe()
{
	TCHAR szPath[MAX_PATH];
	if (0 != GetModuleFileName(NULL, szPath, MAX_PATH))
	{
		TCHAR* pSlash = _tcsrchr(szPath, TEXT('\\'));
		if (NULL != pSlash)
		{
			*pSlash = TEXT('\0');
			SetCurrentDirectory(szPath);
		}
	}
}

/*****************************************************************************
* @func		:	int WINAPI WinMain
* @summary	:	应用程序的入口函数.
* @in		:	HINSTANCE hInstance : 实例句柄
*			:	HINSTANCE hPreInstance : NULL
*			:	LPSTR lpCmdLine	: 运行程序的命令行
*			:	int iShowCmd	: 最大化、最小化、...
* @out		:	none
* @return	:	0
* @author	:	Li Jiachun
* @time		:	2013-3-24 13:30:46
* @history	:	
******************************************************************************/
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MUSICPLAYER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MUSICPLAYER));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

/*****************************************************************************
* @func		:	MyRegisterClass(
* @summary	:	注册窗口类。
* @in		:	HINSTANCE hInstance : 实例句柄
* @out		:	none
* @return	:	注册成功: 窗口类型的标识号
*			:	注册失败: 返回值为0
* @author	:	Li Jiachun
* @time		:	2013-3-24 13:33:41
* @history	:	
******************************************************************************/
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MUSICPLAYER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MUSICPLAYER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

/*****************************************************************************
* @func		:	InitInstance(HINSTANCE, int)
* @summary	:	保存实例句柄并创建主窗口.
* @in		:	HINSTANCE hInstance : 实例句柄
*			:	int iShowCmd	: 最大化、最小化、...
* @out		:	none
* @return	:	TRUE	: 窗口创建成功
*			:	FAULSE	: 窗口创建失败
* @author	:	Li Jiachun
* @time		:	2013-3-24 13:43:22
* @history	:	
******************************************************************************/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中
   SetWorkingDirectoryToExe();

   hWnd = CreateWindow(szWindowClass, 		// 主窗口类名
	   szTitle, 							// 标题栏文本
	   USER_MUSICPLAYERWINDOW, 				// 窗口样式
	   CW_USEDEFAULT, 						// x
	   CW_USEDEFAULT, 						// y
	   800,									// 宽
	   600, 								// 高
	   NULL, 								// 父窗口
	   NULL, 								// 菜单
	   hInstance, 							// 实例句柄
	   NULL);								// lpParam

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

/*****************************************************************************
* @func		:	WndProc(HWND, UINT, WPARAM, LPARAM)
* @summary	:	处理主窗口的消息。
* @message	:	WM_CREATE	- 窗口创建
*			:	WM_COMMAND	- 处理应用程序菜单
*			:	WM_PAINT	- 绘制主窗口
*			:	WM_DESTROY	- 发送退出消息并返回
* @in		:	HWND hwnd : 窗口句柄
*			:	UINT iMessage : 消息
*			:	WPARAM wParam : wParam
*			:	LPARAM lParam : lParam
* @out		:	none
* @return	:	0 : 一般情况
*			:	DefWindowProc(hwnd, iMessage, wParam, lParam) : 未处理的消息
* @author	:	Li Jiachun
* @time		:	2013-3-24 13:45:03
* @history	:	
******************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static int iDeltaPerLine = 0;	// 鼠标滚动每行需要的幅度
	unsigned int iVolume = 0;

	CPlayerView obj(hInst, hWnd);
	switch (message)
	{
	case WM_CREATE:
		SetTimer(hWnd, TIMER_TIMERPROC_ID, 1000, TimerProc);
		// 窗口布局
		obj.InitWindows();
		obj.InitListTable();
		obj.InitScrollList();
		obj.InitScrollTime(AudioPlayer->GetTotalMilliSecond());
		obj.InitScrollVolume(AudioPlayer->GetPlayStatus()->iRightVolume);
		// 获取鼠标滚动每行需要的幅度
		iDeltaPerLine = GetMouseDeltaPerLine();
		break;
	case WM_SETTINGCHANGE:
		iDeltaPerLine = GetMouseDeltaPerLine();
		break;
	case USER_MOUSEHWHEEL:
	case WM_MOUSEHWHEEL:
	{
		LRESULT lResult = obj.UpdateScrollListByMouse(iDeltaPerLine, message, wParam, lParam);
		if (NULL != lResult)
		{
			return lResult;
		}
		break;
	}
	case WM_SIZE:
		break;
	case WM_VSCROLL:
		// 垂直滚动条部品更新
		obj.UpdateScrollList(wParam, lParam);
		// 声音滚动条的更新
		iVolume = AudioPlayer->GetPlayStatus()->iRightVolume;
		obj.UpdateScrollVolume(iVolume, wParam, lParam);
		AudioPlayer->SetPlayerVolume(iVolume);
		break;
	case WM_HSCROLL:
		break;
	case WM_SETFOCUS:
		// TODO: 在此添加设置焦点的代码...
		//hwndChild = GetDlgItem(hWnd, SCROLL_LIST_ID);
		//SetFocus(hwndChild);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// 分析菜单选择:
		if (NULL != MenuProc(hWnd, message, wParam, lParam))
		{
			return MenuProc(hWnd, message, wParam, lParam);
		}
		// 分析列表框
		if (NULL != ListBoxProc(hWnd, message, wParam, lParam))
		{
			return ListBoxProc(hWnd, message, wParam, lParam);
		}
		// 分析按钮
		if (NULL != ButtonProc(hWnd, message, wParam, lParam))
		{
			return ButtonProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...		
		obj.UpdateRectCurreTime(AudioPlayer->GetPlayStatus()->szCurrentTime);
		obj.UpdateRectBeginTime(TEXT("00:00:000"));
		obj.UpdateRectTotalTime(AudioPlayer->GetFileStatus()->szTotalTime);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, TIMER_TIMERPROC_ID);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/*****************************************************************************
* @func		:	About(HWND , UINT , WPARAM , LPARAM )
* @summary	:	“关于”框的消息处理程序。
* @message	:	WM_COMMAND	- 处理应用程序菜单
*			:	WM_PAINT	- 绘制主窗口
*			:	WM_DESTROY	- 发送退出消息并返回
* @in		:	HWND hDlg : 窗口句柄
*			:	UINT message : 消息
*			:	WPARAM wParam : wParam
*			:	LPARAM lParam : lParam
* @out		:	none
* @return	:	FALSE	: 其他情况
*			:	TRUE	: 处理WM_INITDIALOG
* @author	:	Li Jiachun
* @time		:	2013-3-24 13:49:11
* @history	:	
******************************************************************************/
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	default:
		break;
	}
	return (INT_PTR)FALSE;
}

/*****************************************************************************
* @func		:	MenuProc(HWND , UINT , WPARAM , LPARAM )
* @summary	:	“菜单”栏的消息处理程序。
* @message	:	IDM_PLAY	- 播放菜单
*			:	IDM_PAUSE	- 暂停菜单
*			:	IDM_STOP	- 停止菜单
*			:	IDM_ABOUT	- 关于菜单
*			:	IDM_EXIT	- 退出菜单
* @in		:	HWND hWnd : 窗口句柄
*			:	UINT message : 消息
*			:	WPARAM wParam : wParam
*			:	LPARAM lParam : lParam
* @out		:	none
* @return	:	NULL	: 有消息处理の情况
*			:	DefWindowProc	: 没有消息对应的处理
* @author	:	Li Jiachun
* @time		:	2013-3-24 19:52:10
* @history	:	
******************************************************************************/
LRESULT MenuProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId    = LOWORD(wParam);
	CPlayerView obj(hInst, hWnd);
	TCHAR szFileName[MAX_LOADSTRING + 1] = L"";

	switch (wmId)
	{
	case IDM_PLAY:
		obj.GetCurrentSelectListName(szFileName);		
		if (0 != lstrcmp(szFileName, AudioPlayer->GetFileStatus()->szFileName))
		{
			AudioPlayer->NewMusicPlay(szFileName);
			obj.InvalidateRectTotalTime();
			obj.InitScrollTime(AudioPlayer->GetTotalMilliSecond());
		}
		else
		{
			AudioPlayer->MusicPlay();
		}
		obj.ShowPlayOrPause(AudioPlayer->GetPlayStatus()->bPlay);
		break;
	case IDM_PAUSE:
		obj.GetCurrentSelectListName(szFileName);		
		if (0 != lstrcmp(szFileName, AudioPlayer->GetFileStatus()->szFileName))
		{
			AudioPlayer->NewMusicPlay(szFileName);
			obj.InvalidateRectTotalTime();
			obj.InitScrollTime(AudioPlayer->GetTotalMilliSecond());
		}
		else
		{
			AudioPlayer->MusicPause();
		}
		obj.ShowPlayOrPause(AudioPlayer->GetPlayStatus()->bPlay);
		break;
	case IDM_STOP:
		AudioPlayer->MusicStop();
		// obj.InvalidateRectTotalTime();		// 目测停止播放时不需要更新文件播放时间
		obj.ShowPlayOrPause(AudioPlayer->GetPlayStatus()->bPlay);
		break;
	case IDM_PRESONG:
		obj.GetPreviousSongName(szFileName);
		AudioPlayer->NewMusicPlay(szFileName);
		obj.SetPreviousIndex();
		obj.InvalidateRectTotalTime();
		obj.InitScrollTime(AudioPlayer->GetTotalMilliSecond());
		obj.ShowPlayOrPause(AudioPlayer->GetPlayStatus()->bPlay);
		break;
	case IDM_NEXTSONG:
		obj.GetNextSongName(szFileName);
		AudioPlayer->NewMusicPlay(szFileName);
		obj.SetNextIndex();
		obj.InvalidateRectTotalTime();
		obj.InitScrollTime(AudioPlayer->GetTotalMilliSecond());
		obj.ShowPlayOrPause(AudioPlayer->GetPlayStatus()->bPlay);
		break;
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return NULL;
}

/*****************************************************************************
* @func		:	ButtonProc(HWND , UINT , WPARAM , LPARAM )
* @summary	:	按钮的消息处理程序。
* @wmId		:	BUTTON_PRESONG_ID	- 上一首歌按钮
*			:	BUTTON_PLAYPAUSE_ID	- 播放暂停按钮
*			:	BUTTON_NEXTSONG_ID	- 下一首歌按钮
*			:	BUTTON_FF_ID	- 快进按钮
*			:	BUTTON_REW_ID	- 快退按钮
*			:	BUTTON_STOP_ID	- 停止按钮
* @in		:	HWND hWnd : 窗口句柄
*			:	UINT message : 消息
*			:	WPARAM wParam : wParam
*			:	LPARAM lParam : lParam
* @out		:	none
* @return	:	NULL	: 有消息处理の情况
*			:	DefWindowProc	: 没有消息对应的处理
* @author	:	Li Jiachun
* @time		:	2013-3-31 19:55:48
* @history	:	
******************************************************************************/
LRESULT ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	int wmEvent = HIWORD(wParam);
	CPlayerView obj(hInst, hWnd);
	TCHAR szFileName[MAX_LOADSTRING + 1];

	switch (wmId)
	{
	case BUTTON_PRESONG_ID:
		obj.GetPreviousSongName(szFileName);
		AudioPlayer->NewMusicPlay(szFileName);
		obj.SetPreviousIndex();
		obj.InvalidateRectTotalTime();
		obj.InitScrollTime(AudioPlayer->GetTotalMilliSecond());
		obj.ShowPlayOrPause(AudioPlayer->GetPlayStatus()->bPlay);
		break;
	case BUTTON_PLAYPAUSE_ID:
		obj.GetCurrentSelectListName(szFileName);		
		if (0 != lstrcmp(szFileName, AudioPlayer->GetFileStatus()->szFileName))
		{
			AudioPlayer->NewMusicPlay(szFileName);
			obj.InvalidateRectTotalTime();
			obj.InitScrollTime(AudioPlayer->GetTotalMilliSecond());
		}
		else
		{
			AudioPlayer->MusicPause();
		}
		obj.ShowPlayOrPause(AudioPlayer->GetPlayStatus()->bPlay);
		break;
	case BUTTON_NEXTSONG_ID:
		obj.GetNextSongName(szFileName);
		AudioPlayer->NewMusicPlay(szFileName);
		obj.SetNextIndex();
		obj.InvalidateRectTotalTime();
		obj.InitScrollTime(AudioPlayer->GetTotalMilliSecond());
		obj.ShowPlayOrPause(AudioPlayer->GetPlayStatus()->bPlay);
		break;
	case BUTTON_FF_ID:
		AudioPlayer->Seek(5);
		break;
	case BUTTON_REW_ID:
		AudioPlayer->Seek(-5);
		break;
	case BUTTON_STOP_ID:
		AudioPlayer->MusicStop();
		obj.ShowPlayOrPause(AudioPlayer->GetPlayStatus()->bPlay);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return NULL;
}

/*****************************************************************************
* @func		:	ListBoxProc(HWND , UINT , WPARAM , LPARAM )
* @summary	:	列表框消息处理程序。
* @wmEvent	:	LBN_DBLCLK : 鼠标双击列表框条目
* @in		:	HWND hWnd : 窗口句柄
*			:	UINT message : 消息
*			:	WPARAM wParam : wParam
*			:	LPARAM lParam : lParam
* @out		:	none
* @return	:	NULL	: 有消息处理の情况
*			:	DefWindowProc	: 没有消息对应的处理
* @author	:	Li Jiachun
* @time		:	2013-3-24 19:52:10
* @history	:	
******************************************************************************/
LRESULT ListBoxProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (LIST_TABLE_ID != LOWORD(wParam))
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	int wmEvent = HIWORD(wParam);
	CPlayerView obj(hInst, hWnd);
	TCHAR szFileName[MAX_LOADSTRING + 1];

	switch (wmEvent)
	{
	case LBN_DBLCLK:
		obj.GetCurrentSelectListName(szFileName);
		AudioPlayer->NewMusicPlay(szFileName);
		obj.InvalidateRectTotalTime();
		obj.InitScrollTime(AudioPlayer->GetTotalMilliSecond());
		obj.ShowPlayOrPause(true);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return NULL;
}

/*****************************************************************************
* @func		:	GetMouseDeltaPerLine
* @summary	:	获取鼠标每滚动一下时的滚轮滚动大小
* @in		:	void
* @out		:	none
* @return	:	int iDeltaPerLine : 鼠标每滚动一下时的滚轮滚动大小
* @author	:	Li Jiachun
* @time		:	2013-3-31 19:41:08
* @history	:	
******************************************************************************/
int GetMouseDeltaPerLine()
{
	ULONG ulScrollLines = 0;
	int iDeltaPerLine = 0;
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ulScrollLines, 0);

	if (ulScrollLines)
	{
		iDeltaPerLine = WHEEL_DELTA / ulScrollLines;
	}
	else
	{
		iDeltaPerLine  = 0;
	}
	return iDeltaPerLine;
}

/*****************************************************************************
* @func		:	TimerProc
* @summary	:	时间处理的回调函数，每秒处理一次
* @in		:	HWND hWnd : 窗口句柄
*			:	UINT message : 消息
*			:	WPARAM wParam : wParam
*			:	LPARAM lParam : lParam
* @out		:	none
* @return	:	void
* @author	:	Li Jiachun
* @time		:	2013-4-3 21:52:38
* @history	:	
******************************************************************************/
void CALLBACK TimerProc(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	CPlayerView obj(hInst, hWnd);
	if (AudioPlayer->GetPlayStatus()->bPlay)
	{
		if (!AudioPlayer->GetStatus())
		{
			AudioPlayer->MusicStop();
			obj.ShowPlayOrPause(AudioPlayer->GetPlayStatus()->bPlay);
		}

		AudioPlayer->SetMusicCurrentTime();
		obj.InvalidateRectCurrentTime();
		obj.UpdateScrollTime(AudioPlayer->GetCurrentMilliSecond());
	}
	return ;
}