#ifndef GAMETIMER_H
#define GAMETIMER_H

class GameTimer
{
public:
	GameTimer();

	float TotalTime()const;		// 总计时时间
	float DeltaTime()const;		// 帧间隔时间

	void Reset(); // 在消息循环之前调用
	void Start(); // 在取消暂停的时候调用
	void Stop();  // 在暂停的时候调用
	void Tick();  // 计时一次，在每一帧都调用一次

private:
	double mSecondsPerCount;  //系统相关，系统两次计数之间的时间差，是定时器把时间转换为s的基准
	double mDeltaTime;        //距离上次计时的时间差

	__int64 mBaseTime;        //定时器开始工作的时间点
	__int64 mPausedTime;      //暂停的总时间
	__int64 mStopTime;        //暂停的开始时间
	__int64 mPrevTime;        //上次计时的时间点
	__int64 mCurrTime;        //本次计时的时间点


	bool mStopped;            //是否处于暂停状态
};

#endif // GAMETIMER_H