/**
 * @file PlayerView.h
 * @brief MusicPlayer 主窗口视图控制类。
 * @author kaisawind <wind.kaisa@gmail.com>
 * @date 2013-03-29
 * @par 变更历史
 * - 2013-03-29 初版。
 * - 2026-11-25 重构、DPI、播放模式、测试与注释整理，详见 CHANGELOG.md。
 */
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

// 主窗口视图辅助类：创建控件、读取列表、更新滚动条和时间文本。
class CPlayerView
{
public:
	CPlayerView(HINSTANCE, HWND);
	void ShowPlayOrPause(bool);
	void InitListTable();
	int SelectListName(const TCHAR*);
	int InitWindows();
	void LayoutControls();
	int GetSongsCount();
	int GetListTopIndex();
	void SetListTopIndex(int);
	void InitScrollList();
	void UpdateScrollList(WPARAM, LPARAM);
	int GetCurrentIndex();
	int GetSongNameByIndex(int, TCHAR*, int);
	void SetListIndex(int);
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
	RECT m_rectBegin;
	RECT m_rectEnd;
	RECT m_rectCurrent;
};
#endif // __CPLAYERVIEW_H__
