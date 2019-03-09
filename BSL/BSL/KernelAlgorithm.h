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

	//����һ��ԭʼԤ�⺯����û�о����κ�Ԥ����
	double originalKernelRegress(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType);

	//������������������
	double increaseSampleKernelRegress(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType);

	//�����������������㣬�޳����
	double increaseSampleKernelRegress_cutBadPoint(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType);

	//�����ģ�
	double errorsWeightKernelRegress(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType);

	//�����壺
	double logErrorsWeightKernelRegress(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType);

	//funcId 1 - 6 ������һ��������
	double predict(int funcId, vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType);

	//KNearר��Ԥ�⺯��
	double predict(vector<SPoint> & pointList, SPoint & spPredict, double h, int kernelFuncType);

	vector<SPoint> & getPointList();

	//���ô�������
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
	vector<SPoint> m_pointList;//���б�
	int m_windowShape;
	QVector<double> m_result;
};

