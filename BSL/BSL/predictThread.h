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
	void locate(const QVector<CPointInfo *> & predictResult, Rect & rect);
signals:
	void predictStart();
	void predictEnd(QVector<CPointInfo *> * );
	void locateStart();
	void locateRect(QVector<Rect> * locateResult);
	void locateEnd();
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

	void myQuickSortByFeild(QVector<CPointInfo *> &vec, int low, int high);
	void myQuickSortByX(QVector<CPointInfo *> &vec, int low, int high);
	void myQuickSortByY(QVector<CPointInfo *> &vec, int low, int high);

	//��ȡ���ΰ��������е�
	void foundRectPointList(const QVector<CPointInfo *> & __INPUT__ predictResult, QVector<CPointInfo *> & __OUTPUT__ rectPointList, Rect & rect);
	//ͳ��Rect�г����ٽ�ֵ��ĸ���
	int rectPointListFeildMaxThancriticalCount(QVector<CPointInfo *> & rectPointList, double criticalField);
private:
	predictParameter m_pp;
	vector<SPoint> m_pointList;
	float m_lenth;
	float m_width;
	float m_r;

	QVector<CPointInfo *> pointList;//Ԥ�����б�
	QVector<CPointInfo *> result;//K���ڷǲ����˻ع�ר�ã�����
	
	int rectIndex;
	QVector<Rect> locateResult;
};

