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
