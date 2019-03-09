#pragma once
#include "define.h"
#include <vector>
#include <QVector>
using namespace std;

//#ifdef SERVERDLL_EXPORTS 
//#define SERVERDLL_API __declspec(dllexport)  
//#else  
//#define SERVERDLL_API __declspec(dllimport)  
//#endif  

class /*SERVERDLL_API*/ CKernelAlgorithm
{
public:
	CKernelAlgorithm();
	~CKernelAlgorithm();
	void setXYNList(double X[], double Y[], double N[], int nCount);
	inline void setPointList(vector<SPoint> pointList)
	{	m_pointList = pointList;  }

	//方法一：原始预测函数，没有经过任何预处理
	double originalKernelRegress(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType);

	//方法二：增加样本点
	double increaseSampleKernelRegress(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType);

	//方法三：增加样本点，剔除噪点
	double increaseSampleKernelRegress_cutBadPoint(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType);

	//方法四：
	double errorsWeightKernelRegress(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType);

	//方法五：
	double logErrorsWeightKernelRegress(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType);

	//funcId 1 - 6 代表方法一到方法六
	double predict(int funcId, vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType);

	//KNear专用预测函数
	double predict(vector<SPoint> & pointList, SPoint & spPredict, double h, int kernelFuncType);

	vector<SPoint> & getPointList();

	//设置窗口类型
	void setWindowShape(int windowShape);
private:
	double kernelK2(double x, double y, int algorithm);

	double sum(QVector<double> & K);

	QVector<double> & kernelK2_KNear(QVector<double> & x_list, QVector<double> & y_list, int algorithm);

	void fTricube(QVector<double> & result, QVector<double> & x_list, QVector<double> & y_list);
	void fBoxcar(QVector<double> & result, QVector<double> & x_list, QVector<double> & y_list);
	void fGaussian(QVector<double> & result, QVector<double> & x_list, QVector<double> & y_list);
	void fEpanechnikov(QVector<double> & result, QVector<double> & x_list, QVector<double> & y_list);
private:
	vector<SPoint> m_pointList;//点列表
	int m_windowShape;
	QVector<double> m_result;
};

