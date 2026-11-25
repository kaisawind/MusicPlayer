/********************************************************************************
* @file		:	MusicPlayer.h
* @summary	:	应用层头文件：窗口、菜单、定时器声明
* @author	:	Li Jiachun
* @time		:	2013-3-24 13:26:03
* @history	:
********************************************************************************/
#ifndef __MUSICPLAYER_H__
#define __MUSICPLAYER_H__
#include "resource.h"
#include "AudioPlayer.h"
#include "PlayerView.h"

// 全局音频播放器实例
#define AudioPlayer CAudioPlayer::getInstance()

enum enum_Timer_em
{
	TIMER_TIMERPROC_ID = 1
};

/********************************************************************************
* USER_MUSICPLAYERWINDOW : 自定义MusicPlayer窗口格式
* WS_OVERLAPPED : 标题条和边框
* WS_CAPTION : 标题框
* WS_SYSMENU : 标题条上带有窗口菜单
* WS_MINIMIZEBOX : 允许最小化
* WS_MAXIMIZEBOX : 允许最大化
* WS_THICKFRAME : 允许拖动边框调整大小
********************************************************************************/
#define USER_MUSICPLAYERWINDOW (WS_OVERLAPPED		| \
								WS_CAPTION			| \
								WS_SYSMENU			| \
								WS_MINIMIZEBOX		| \
								WS_MAXIMIZEBOX		| \
								WS_THICKFRAME)

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT MenuProc(HWND, UINT, WPARAM, LPARAM);
LRESULT ButtonProc(HWND, UINT, WPARAM, LPARAM);
LRESULT ListBoxProc(HWND, UINT, WPARAM, LPARAM);
int GetMouseDeltaPerLine();
void CALLBACK TimerProc(HWND, UINT, UINT, DWORD);

#endif // __MUSICPLAYER_H__
