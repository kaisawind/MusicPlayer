#pragma once

// 把 value 夹到 [minValue, maxValue] 区间内。
inline int ClampInt(int value, int minValue, int maxValue)
{
	if (value < minValue)
	{
		return minValue;
	}
	if (value > maxValue)
	{
		return maxValue;
	}
	return value;
}

// 返回下一首的索引；空列表返回 -1，末尾保持最后一项。
inline int NextIndex(int current, int count)
{
	if (count <= 0)
	{
		return -1;
	}
	if (current < 0)
	{
		return 0;
	}
	return (current + 1 < count) ? (current + 1) : (count - 1);
}

// 返回上一首的索引；空列表返回 -1，开头保持第一项。
inline int PrevIndex(int current, int count)
{
	if (count <= 0)
	{
		return -1;
	}
	if (current >= count)
	{
		current = count - 1;
	}
	if (current <= 0)
	{
		return 0;
	}
	return current - 1;
}

enum PlayMode
{
	PlayModeSequential = 0,
	PlayModeRepeatOne,
	PlayModeRepeatAll,
	PlayModeShuffle
};

// 按播放模式返回下一首索引；顺序模式到末尾返回 -1，空列表返回 -1。
// randomValue 由调用者传入 rand()，便于测试随机分支。
inline int NextIndexForMode(int current, int count, PlayMode mode, unsigned int randomValue)
{
	if (count <= 0)
	{
		return -1;
	}
	if (current < 0)
	{
		current = 0;
	}
	else if (current >= count)
	{
		current = count - 1;
	}

	switch (mode)
	{
	case PlayModeSequential:
		return (current + 1 < count) ? (current + 1) : -1;
	case PlayModeRepeatOne:
		return current;
	case PlayModeRepeatAll:
		return (current + 1) % count;
	case PlayModeShuffle:
		if (1 == count)
		{
			return 0;
		}
		{
			int next = (int)(randomValue % (unsigned int)count);
			if (next == current)
			{
				next = (next + 1) % count;
			}
			return next;
		}
	default:
		return -1;
	}
}
