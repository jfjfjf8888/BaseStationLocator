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
	//获取预测函数ID
	int getFuncId();
	//获取核函数ID
	int getCoreFuncId();

	bool isPointExist(SPoint & sp, int & index);

	void kNear(CKernelAlgorithm & ka);

	//根据传入坐标查找预测是查找预测值,flag 0表示屏蔽前，1表示屏蔽后
	double foundPredictValue(double x, double y, QVector<CPointInfo *> & pl, int flag);

	//判断传入坐标是否在原始数据中，存在返回true，不存在返回false
	bool isPointExist(float x, float y);
private:
	predictParameter m_pp;
	vector<SPoint> m_pointList;
	float m_lenth;
	float m_width;
	float m_r;

	QVector<CPointInfo *> pointList;//预测结果列表
	QVector<CPointInfo *> result;//K近邻非参数核回归专用！！！
	
	//单点预测时使用
};

