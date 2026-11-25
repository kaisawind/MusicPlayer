/*******************************************************************************
* @file		:	AudioPlayer.cpp
* @summary	:	libzplay 音频播放封装
* @author	:	Li Jiachun
* @time		:	2013-3-24 14:53:06
* @history	:	
********************************************************************************/

#include "stdafx.h"
#include "AudioPlayer.h"

// 私有静态数据成员初始化
ZPlay* CAudioPlayer::player = CreateZPlay();

/*****************************************************
*	@func	:	getInstance
*	@summary:	单例模式实现
*	@in		:	void
*	@out	:	none
*	@return	:	CAudioPlayer*
*	@author	:	DaPPer
*	@time	:	2013-3-24 15:08:21
*	@history:	
******************************************************/
CAudioPlayer* CAudioPlayer::getInstance(void )
{
	static CAudioPlayer obj;
	return &obj;
}

/*****************************************************
*	@func	:	CAudioPlayer
*	@summary:	构造函数
*	@in		:	void
*	@out	:	none
*	@return	:	none
*	@author	:	DaPPer
*	@time	:	2013-3-24 15:14:37
*	@history:	
******************************************************/
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

/*****************************************************
*	@func	:	OpenFile(const TCHAR*)
*	@summary:	打开音乐文件
*	@in		:	const TCHAR* szFileName	: 要打开的文件名
*	@out	:	none
*	@return	:	TRUE : ALL OK
*			:	FALSE : 打开文件失败
*	@author	:	DaPPer
*	@time	:	2013-3-24 15:46:31
*	@history:	
******************************************************/
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

/*****************************************************
*	@func	:	MusicPlay()
*	@summary:	播放音乐文件
*	@in		:	void
*	@out	:	none
*	@return	:	TRUE : ALL OK
*			:	FALSE : 音乐播放失败
*	@author	:	DaPPer
*	@time	:	2013-3-24 16:06:55
*	@history:	
******************************************************/
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

/*****************************************************
*	@func	:	MusicPause()
*	@summary:	暂停音乐文件播放
*	@in		:	void
*	@out	:	none
*	@return	:	TRUE : ALL OK
*			:	FALSE : 音乐播放失败
*	@author	:	DaPPer
*	@time	:	2013-3-24 16:06:55
*	@history:	
******************************************************/
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

/*****************************************************
*	@func	:	MusicStop()
*	@summary:	停止播放音乐文件
*	@in		:	void
*	@out	:	none
*	@return	:	TRUE : ALL OK
*			:	FALSE : 音乐停止播放失败
*	@author	:	DaPPer
*	@time	:	2013-3-24 19:11:03
*	@history:	
******************************************************/
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

/*****************************************************
*	@func	:	GetPlayStatus()
*	@summary:	获取播放状态
*	@in		:	void
*	@out	:	none
*	@return	:	PlayStatus : 播放状态结构体
*	@author	:	DaPPer
*	@time	:	2013-3-24 22:34:41
*	@history:	
******************************************************/
tagPlayStatus_t* CAudioPlayer::GetPlayStatus()
{
	return &PlayStatus;
}

/*****************************************************
*	@func	:	GetFileStatus()
*	@summary:	获取文件状态
*	@in		:	void
*	@out	:	none
*	@return	:	FileStatus : 文件状态结构体
*	@author	:	DaPPer
*	@time	:	2013-3-24 22:34:41
*	@history:	
******************************************************/
tagFileStatus_t* CAudioPlayer::GetFileStatus()
{
	return &FileStatus;
}

/*****************************************************
*	@func	:	MusicClose()
*	@summary:	关闭当前文件，复位打开/播放状态
*	@in		:	void
*	@out	:	none
*	@return	:	TRUE : ALL OK
*			:	FALSE : 关闭失败
*	@author	:	DaPPer
*	@time	:	2013-3-30 16:02:21
*	@history:	
******************************************************/
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

/*****************************************************
*	@func	:	NewMusicPlay
*	@summary:	打开并播放新文件；格式不支持时保留当前文件
*	@in		:	const TCHAR* szFileName	: 要播放的歌曲的名字
*	@out	:	none
*	@return	:	TRUE : ALL OK
*			:	FALSE : 音乐播放失败
*	@author	:	DaPPer
*	@time	:	2013-3-31 20:17:40
*	@history:	
******************************************************/
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

	int iRet = FALSE;
	if (MusicClose()			&&
		OpenFile(szFileName)	&&
		MusicPlay())
	{
		iRet = TRUE;
	}

	return iRet;
}

/*****************************************************
*	@func	:	SetMusicTotalTime
*	@summary:	设置音乐的总时间
*	@in		:	void
*	@out	:	none
*	@return	:	void
*	@author	:	DaPPer
*	@time	:	2013-4-3 22:03:40
*	@history:	
******************************************************/
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

/*****************************************************
*	@func	:	SetMusicCurrentTime
*	@summary:	设置音乐的当前时间
*	@in		:	void
*	@out	:	none
*	@return	:	void
*	@author	:	DaPPer
*	@time	:	2013-4-3 22:03:40
*	@history:	
******************************************************/
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

/*****************************************************
*	@func	:	GetStatus
*	@summary:	获取音乐的播放状态
*	@in		:	void
*	@out	:	none
*	@return	:	TRUE : 播放状态
*			:	FALSE : 播放完了
*	@author	:	DaPPer
*	@time	:	2013-4-3 22:03:40
*	@history:	
******************************************************/
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

/*****************************************************
*	@func	:	Seek
*	@summary:	快进快退处理
*	@in		:	int iTime >= 0 : 快进
*			:	int iTime <  0 : 快退
*	@out	:	none
*	@return	:	void
*	@author	:	DaPPer
*	@time	:	2013-4-3 22:03:40
*	@history:	
******************************************************/
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