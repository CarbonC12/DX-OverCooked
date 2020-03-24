#ifndef GAMETIMER_H
#define GAMETIMER_H

class GameTimer
{
public:
	GameTimer();

	float TotalTime()const;		// �ܼ�ʱʱ��
	float DeltaTime()const;		// ֡���ʱ��

	void Reset(); // ����Ϣѭ��֮ǰ����
	void Start(); // ��ȡ����ͣ��ʱ�����
	void Stop();  // ����ͣ��ʱ�����
	void Tick();  // ��ʱһ�Σ���ÿһ֡������һ��

private:
	double mSecondsPerCount;  //ϵͳ��أ�ϵͳ���μ���֮���ʱ���Ƕ�ʱ����ʱ��ת��Ϊs�Ļ�׼
	double mDeltaTime;        //�����ϴμ�ʱ��ʱ���

	__int64 mBaseTime;        //��ʱ����ʼ������ʱ���
	__int64 mPausedTime;      //��ͣ����ʱ��
	__int64 mStopTime;        //��ͣ�Ŀ�ʼʱ��
	__int64 mPrevTime;        //�ϴμ�ʱ��ʱ���
	__int64 mCurrTime;        //���μ�ʱ��ʱ���


	bool mStopped;            //�Ƿ�����ͣ״̬
};

#endif // GAMETIMER_H