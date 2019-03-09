#pragma once
#include <QThread>
#include "KernelAlgorithm.h"
#include "define.h"
#include <QVector>

class predictThread :
	public QThread
{
	Q_OBJECT
public:
	predictThread();
	~predictThread();

	void setPredictData(predictParameter & pp,
		vector<SPoint> & pointList);
	void run() override;
	void setScenesInfo(float lenth, float width, float r);
	void locate();
signals:
	void predictStart();
	void predictEnd(QVector<CPointInfo *> * );

private:
	//��ȡԤ�⺯��ID
	int getFuncId();
	//��ȡ�˺���ID
	int getCoreFuncId();

	bool isPointExist(SPoint & sp, int & index);

	void kNear(CKernelAlgorithm & ka);

	//���ݴ����������Ԥ���ǲ���Ԥ��ֵ,flag 0��ʾ����ǰ��1��ʾ���κ�
	double foundPredictValue(double x, double y, QVector<CPointInfo *> & pl, int flag);

	//�жϴ��������Ƿ���ԭʼ�����У����ڷ���true�������ڷ���false
	bool isPointExist(float x, float y);
private:
	predictParameter m_pp;
	vector<SPoint> m_pointList;
	float m_lenth;
	float m_width;
	float m_r;

	QVector<CPointInfo *> pointList;//Ԥ�����б�
	QVector<CPointInfo *> result;//K���ڷǲ����˻ع�ר�ã�����
	
	//����Ԥ��ʱʹ��
};

