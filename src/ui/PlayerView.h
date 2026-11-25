/********************************************************************************
* @file		:	PlayerView.h
* @summary	:	MusicPlayer 主窗口视图控制类。
* @author	:	Li Jiachun
* @time		:	2013-3-29 20:48:05
* @history	:
********************************************************************************/
#ifndef __CPLAYERVIEW_H__
#define __CPLAYERVIEW_H__

#include "Common.h"

// 子窗口ID
enum enumWindowID_em
{
	BUTTON_REW_ID = 1,
	BUTTON_FF_ID,
	BUTTON_PLAYPAUSE_ID,
	BUTTON_STOP_ID,
	BUTTON_PRESONG_ID,
	BUTTON_NEXTSONG_ID,
	SCROLL_LIST_ID,
	SCROLL_VOLUME_ID,
	SCROLL_TIME_ID,
	LIST_TABLE_ID,
	STATIC_LISTPATH_ID
};

class CPlayerView
{
public:
	CPlayerView(HINSTANCE, HWND);
	void ShowPlayOrPause(bool);
	void InitListTable();
	int InitWindows();
	int GetSongsCount();
	int GetListTopIndex();
	void SetListTopIndex(int);
	void InitScrollList();
	void UpdateScrollList(WPARAM, LPARAM);
	int GetCurrentSelectListName(TCHAR*, int);
	LRESULT UpdateScrollListByMouse(int, bool, UINT, WPARAM, LPARAM);
	void UpdateRectBeginTime(HDC, const TCHAR[]);
	void UpdateRectTotalTime(HDC, const TCHAR[]);
	void UpdateRectCurreTime(HDC, const TCHAR[]);
	void InvalidateRectCurrentTime();
	void InvalidateRectTotalTime();
	void InitScrollTime(int);
	int GetPreviousSongName(TCHAR*, int);
	int GetNextSongName(TCHAR*, int);
	void SetPreviousIndex();
	void SetNextIndex();
	void InitScrollVolume(unsigned int);
	void UpdateScrollVolume(unsigned int&, WPARAM, LPARAM);
	void UpdateScrollTime(unsigned int);

private:
	int CreateButtonPreSong();
	int CreateButtonNextSong();
	int CreateButtonREW();
	int CreateButtonFF();
	int CreateButtonStop();
	int CreateButtonPlayPause();
	int CreateScrollList();
	int CreateScrollTime();
	int CreateScrollVolume();
	int CreateListTable();
	int CreateListPath();

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;
};
#endif // __CPLAYERVIEW_H__
