
#include <windows.h>
#include "GameTimer.h"

GameTimer::GameTimer()
	: mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0),
	mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{
	__int64 countsPerSec;
	//QueryPerformanceFrequency����ͨ�����Ĳ������ص�ǰʱ��ֵ���ò�����һ��64λ������ʹ��QueryPerformanceFrequency��������ȡ���ܼ�ʱ����Ƶ�ʣ�ÿ��ļ�����������
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;              	//������������֮��ĵ�ʱ���ֵ
}

//�õ���ʱ��ʱ��
float GameTimer::TotalTime()const
{
	//�����ǰ�ǳ�����ͣ״̬����ô���Ϊ������ͣʱ��ʱ��� - ��ͣ����ʱ�� - ��ʱ��ʼ����ʼ�㣩
	if (mStopped)
	{
		return (float)(((mStopTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
	}

	//�����ǰû����ͣ�����Ϊ������ǰʱ�� - ��ͣ����ʱ�� - ��ʱ��ʼ��ʱ��㣩
	else
	{
		return (float)(((mCurrTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
	}
}

// �����ϴδμ�ʱ��ʱ���
float GameTimer::DeltaTime()const
{
	return (float)mDeltaTime;
}

//���ü�ʱ��
void GameTimer::Reset()
{
	//����ʱ���õ��Ѿ������Ĵ���
	//�ÿ�ʼ��ʱ�ĳ�ʼ��Ϊ��ǰ��
	//�ϴμ�ʱ��ʱ���Ϊ����
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mPausedTime = 0;	// �漰�����Reset�Ļ���Ҫ�����0
	mStopped = false;
}

//��ͣ���ٿ�ʼ��ʱ
void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (mStopped)
	{
		//������ͣʱ��
		mPausedTime += (startTime - mStopTime);
		//�����ϴμ�ʱʱ���
		mPrevTime = startTime;
		mStopTime = 0;
		mStopped = false;
	}
}

//��ͣ��ʱ
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

	// ��֮֡���ʱ���
	mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;

	//�ı�PrevTime��Ϊ��¼��һ֡ʱ����׼��
	mPrevTime = mCurrTime;

	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}

