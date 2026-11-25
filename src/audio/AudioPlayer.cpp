/**
 * @file AudioPlayer.cpp
 * @brief libzplay 音频播放封装
 * @author kaisawind <wind.kaisa@gmail.com>
 * @date 2013-03-24
 * @par 变更历史
 * - 2013-03-24 初版。
 * - 2026-11-25 重构、DPI、播放模式、测试与注释整理，详见 CHANGELOG.md。
 */

#include "stdafx.h"
#include "AudioPlayer.h"

// 私有静态数据成员初始化
ZPlay* CAudioPlayer::player = CreateZPlay();

/**
 * @brief 单例模式实现
 * @return CAudioPlayer*
 */
CAudioPlayer* CAudioPlayer::getInstance(void )
{
	static CAudioPlayer obj;
	return &obj;
}

/**
 * @brief 构造函数
 */
CAudioPlayer::CAudioPlayer()
{
	// 文件状态初始化
	FileStatus.nformat = UNKNOWN;
	lstrcpyW(FileStatus.szFileName, TEXT(""));
	lstrcpyW(FileStatus.szTotalTime, TEXT("00:00:000"));

	// 播放状态初始化
	PlayStatus.bFileOpen = false;
	PlayStatus.bPlay = false;
	lstrcpyW(PlayStatus.szCurrentTime, TEXT("00:00:000"));

	if (NULL != player)
	{
		player->GetPlayerVolume(&(PlayStatus.iLeftVolume), &(PlayStatus.iRightVolume));
	}
	else
	{
		PlayStatus.iLeftVolume = 0;
		PlayStatus.iRightVolume = 0;
	}
}

/**
 * @brief 打开音乐文件
 * @param[in] szFileName 要打开的文件名
 * @return TRUE : ALL OK
 * FALSE : 打开文件失败
 */
int CAudioPlayer::OpenFile(const TCHAR* szFileName)
{
	if (NULL == player)
	{
		return FALSE;
	}
	int iRet = FALSE;

	lstrcpynW(FileStatus.szFileName, szFileName, MAX_PATH);
	int nFormat = player->GetFileFormatW(szFileName);
	if(nFormat == sfUnknown)
	{
	  return FALSE;
	}

	iRet = player->OpenFileW(FileStatus.szFileName, (TStreamFormat)nFormat);

	if (TRUE == iRet)
	{
		PlayStatus.bFileOpen = true;
		FileStatus.nformat = (enumMusicFormat_em)nFormat;
		SetMusicTotalTime();
	}
	else
	{
		PlayStatus.bFileOpen = false;
	}
	return iRet;
}

/**
 * @brief 播放音乐文件
 * @return TRUE : ALL OK
 * FALSE : 音乐播放失败
 */
int CAudioPlayer::MusicPlay()
{
	if (NULL == player)
	{
		return FALSE;
	}

	// 循环10次，打不开就不打了
	int iCount = 0;
	while (false == PlayStatus.bFileOpen)
	{
		iCount++;
		if (OpenFile(FileStatus.szFileName))
		{
			break;
		}
		
		if (10 == iCount)
		{
			break;
		}
	}

	int iRet = FALSE;

	if (true == PlayStatus.bFileOpen)
	{
		iRet = player->Play();

		if (TRUE == iRet)
		{
			PlayStatus.bPlay = true;
		}
	}

	return iRet;
}

/**
 * @brief 暂停或继续播放
 * @return TRUE : ALL OK
 * FALSE : 音乐播放失败
 */
int CAudioPlayer::MusicPause()
{
	if (NULL == player)
	{
		return FALSE;
	}

	int iRet = FALSE;

	if (true == PlayStatus.bPlay)
	{
		iRet = player->Pause();
		if (TRUE == iRet)
		{
			PlayStatus.bPlay = false;
		}
	}
	else
	{
		iRet = MusicPlay();
	}	

	return iRet;

}

/**
 * @brief 停止播放音乐文件
 * @return TRUE : ALL OK
 * FALSE : 音乐停止播放失败
 */
int CAudioPlayer::MusicStop()
{
	// 文件没打开直接返回
	if ((false == PlayStatus.bFileOpen) || (NULL == player))
	{
		return FALSE;
	}

	int iRet = TRUE;

	iRet = player->Stop();

	if (TRUE == iRet)
	{
		PlayStatus.bPlay = false;
	}

	return iRet;
}

/**
 * @brief 获取播放状态
 * @return PlayStatus : 播放状态结构体
 */
tagPlayStatus_t* CAudioPlayer::GetPlayStatus()
{
	return &PlayStatus;
}

/**
 * @brief 获取文件状态
 * @return FileStatus : 文件状态结构体
 */
tagFileStatus_t* CAudioPlayer::GetFileStatus()
{
	return &FileStatus;
}

/**
 * @brief 关闭当前文件，复位打开/播放状态
 * @return TRUE : ALL OK
 * FALSE : 关闭失败
 */
int CAudioPlayer::MusicClose()
{
	if (NULL == player)
	{
		return FALSE;
	}

	int iRet = FALSE;
	iRet = player->Close();

	if (iRet)
	{
		PlayStatus.bFileOpen = false;
		PlayStatus.bPlay = false;
	}
	return iRet;
}

/**
 * @brief 打开并播放新文件；失败时尝试恢复当前文件和位置
 * @param[in] szFileName 要播放的歌曲的名字
 * @return TRUE : ALL OK
 * FALSE : 音乐播放失败
 */
int CAudioPlayer::NewMusicPlay(const TCHAR* szFileName)
{
	if ((NULL == player) || (NULL == szFileName))
	{
		return FALSE;
	}

	// 先确认格式；不支持的格式直接失败，避免关闭当前正在播放的文件
	if (sfUnknown == player->GetFileFormatW(szFileName))
	{
		return FALSE;
	}

	// 保存当前文件和位置；新文件打开/播放失败时恢复
	TCHAR szOldFile[MAX_PATH] = L"";
	lstrcpynW(szOldFile, FileStatus.szFileName, MAX_PATH);
	bool bOldOpen = PlayStatus.bFileOpen;
	bool bOldPlay = PlayStatus.bPlay;
	unsigned int uiOldPosition = bOldOpen ? GetCurrentMilliSecond() : 0;

	if (MusicClose()			&&
		OpenFile(szFileName)	&&
		MusicPlay())
	{
		return TRUE;
	}

	if (bOldOpen && (TEXT('\0') != szOldFile[0]))
	{
		if (MusicClose() && OpenFile(szOldFile))
		{
			SeekToMilliSecond(uiOldPosition);
			if (bOldPlay)
			{
				MusicPlay();
			}
		}
	}

	return FALSE;
}

/**
 * @brief 设置音乐的总时间
 */
void CAudioPlayer::SetMusicTotalTime()
{
	if (NULL == player)
	{
		return ;
	}

	TStreamInfo pInfo;
	player->GetStreamInfo(&pInfo);
	wsprintf(FileStatus.szTotalTime,
		TEXT("%02u:%02u:%03u"),
		pInfo.Length.hms.minute,
		pInfo.Length.hms.second,
		pInfo.Length.hms.millisecond);
	return ;
}

/**
 * @brief 设置音乐的当前时间
 */
void CAudioPlayer::SetMusicCurrentTime()
{
	if (NULL == player)
	{
		return ;
	}

	TStreamTime pInfo;
	player->GetPosition(&pInfo);
	wsprintf(PlayStatus.szCurrentTime,
		TEXT("%02u:%02u:%03u"),
		pInfo.hms.minute,
		pInfo.hms.second,
		pInfo.hms.millisecond);
	return ;
}

/**
 * @brief 获取音乐的播放状态
 * @return int : 非 0 表示正在播放，0 表示已停止/结束
 */
int CAudioPlayer::GetStatus()
{
	if (NULL == player)
	{
		return FALSE;
	}

	TStreamStatus status;
	player->GetStatus(&status);
	return status.fPlay;
}

/**
 * @brief 快进快退处理
 * @param[in] iTime 秒数；>=0 快进，<0 快退
 */
void CAudioPlayer::Seek(int iTime)
{
	if ((NULL == player) || (false == PlayStatus.bFileOpen))
	{
		return ;
	}

	TStreamTime pTime;
	if (0 <= iTime)
	{
		pTime.sec = iTime;
		player->Seek(tfSecond, &pTime, smFromCurrentForward);
	}
	else
	{
		pTime.sec = -iTime;
		player->Seek(tfSecond, &pTime, smFromCurrentBackward);
	}
}

// 跳转到指定毫秒位置。
void CAudioPlayer::SeekToMilliSecond(unsigned int iMilliSecond)
{
	if ((NULL == player) || (false == PlayStatus.bFileOpen))
	{
		return ;
	}

	TStreamTime pTime;
	pTime.ms = iMilliSecond;
	player->Seek(tfMillisecond, &pTime, smFromBeginning);
	return ;
}

// 设置左右声道音量（0-100）。
void CAudioPlayer::SetPlayerVolume(int iVolume)
{
	if (NULL == player)
	{
		return ;
	}

	if (iVolume < 0)
	{
		iVolume = 0;
	}
	else if (iVolume > MAX_PLAYER_VOLUME)
	{
		iVolume = MAX_PLAYER_VOLUME;
	}

	player->SetPlayerVolume(iVolume, iVolume);
	player->GetPlayerVolume(&(PlayStatus.iLeftVolume), &(PlayStatus.iRightVolume));
	return ;
}

// 返回当前播放位置（毫秒）。
unsigned int CAudioPlayer::GetCurrentMilliSecond()
{
	if (NULL == player)
	{
		return 0;
	}

	TStreamTime pTime;
	player->GetPosition(&pTime);
	return pTime.ms;
}

// 返回总时长（毫秒）。
unsigned int CAudioPlayer::GetTotalMilliSecond()
{
	if (NULL == player)
	{
		return 0;
	}

	TStreamInfo pInfo;
	player->GetStreamInfo(&pInfo);
	return pInfo.Length.ms;
}
