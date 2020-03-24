
#include <windows.h>
#include "GameTimer.h"

GameTimer::GameTimer()
	: mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0),
	mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{
	__int64 countsPerSec;
	//QueryPerformanceFrequency函数通过它的参数返回当前时间值，该参数是一个64位整数。使用QueryPerformanceFrequency函数来获取性能计时器的频率（每秒的计数次数）：
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;              	//两个计数两次之间的的时间差值
}

//得到计时总时间
float GameTimer::TotalTime()const
{
	//如果当前是出于暂停状态，那么结果为：（暂停时的时间点 - 暂停的总时间 - 计时开始的起始点）
	if (mStopped)
	{
		return (float)(((mStopTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
	}

	//如果当前没有暂停：结果为：（当前时间 - 暂停的总时间 - 计时开始的时间点）
	else
	{
		return (float)(((mCurrTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
	}
}

// 距离上次次计时的时间差
float GameTimer::DeltaTime()const
{
	return (float)mDeltaTime;
}

//重置计时器
void GameTimer::Reset()
{
	//重置时，得到已经计数的次数
	//让开始计时的初始点为当前点
	//上次计时的时间点为现在
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mPausedTime = 0;	// 涉及到多次Reset的话需要将其归0
	mStopped = false;
}

//暂停后，再开始计时
void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (mStopped)
	{
		//更新暂停时间
		mPausedTime += (startTime - mStopTime);
		//更新上次计时时间点
		mPrevTime = startTime;
		mStopTime = 0;
		mStopped = false;
	}
}

//暂停定时
void GameTimer::Stop()
{
	if (!mStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mStopTime = currTime;
		mStopped = true;
	}
}

void GameTimer::Tick()
{
	if (mStopped)
	{
		mDeltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	// 两帧之间的时间差
	mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;

	//改变PrevTime，为记录下一帧时间做准备
	mPrevTime = mCurrTime;

	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}

