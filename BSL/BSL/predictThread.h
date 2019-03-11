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

	void myQuickSortByFeild(QVector<CPointInfo *> &vec, int low, int high);
	void myQuickSortByX(QVector<CPointInfo *> &vec, int low, int high);
	void myQuickSortByY(QVector<CPointInfo *> &vec, int low, int high);

	//获取矩形包含的所有点
	void foundRectPointList(const QVector<CPointInfo *> & __INPUT__ predictResult, QVector<CPointInfo *> & __OUTPUT__ rectPointList, Rect & rect);
	//统计Rect中超过临界值点的个数
	int rectPointListFeildMaxThancriticalCount(QVector<CPointInfo *> & rectPointList, double criticalField);
private:
	predictParameter m_pp;
	vector<SPoint> m_pointList;
	float m_lenth;
	float m_width;
	float m_r;

	QVector<CPointInfo *> pointList;//预测结果列表
	QVector<CPointInfo *> result;//K近邻非参数核回归专用！！！
	
	int rectIndex;
	QVector<Rect> locateResult;
};

