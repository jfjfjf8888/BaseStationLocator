#include "KernelAlgorithm.h"

#include <cmath>
#include <iostream>
using namespace std;

CKernelAlgorithm::CKernelAlgorithm()
{
	m_pointList.clear();

	//核函数
	//m_kernelFuncList.push_back("Tricube");
	//m_kernelFuncList.push_back("Boxcar");
	//m_kernelFuncList.push_back("Gaussian");
	//m_kernelFuncList.push_back("Epanechnikov");

	//m_predictModel.push_back("线性回归");
	//m_predictModel.push_back("增加样本点-线性回归");
	//m_predictModel.push_back("增加样本点-去噪-线性回归");
	//m_predictModel.push_back("误差加权-线性回归");
	//m_predictModel.push_back("误差加权2-线性回归");
	m_windowShape = 1;//默认窗口类型为圆形
}


CKernelAlgorithm::~CKernelAlgorithm()
{
}

void CKernelAlgorithm::setXYNList(double X[], double Y[], double N[], int nCount)
{
	int nManIndex = -1;//最大点的索引
	int nMinIndex = -1;//最小点的索引

	double maxN = -99999;
	double minN = 99999;
	for (int i = 0; i < nCount; ++i)
	{
		SPoint sp;
		sp.x = X[i];
		sp.y = Y[i];
		sp.N = N[i];

		if (sp.N > maxN)
		{
			maxN = sp.N;
			nManIndex = i;
		}

		if (sp.N < minN)
		{
			minN = sp.N;
			nMinIndex = i;
		}
		m_pointList.push_back(sp);
	}

	//cout << "MAX:" << m_pointList[nManIndex].N << "   MIN:" << m_pointList[nMinIndex].N;
	//剔除最大点和最小点
	if (nManIndex > nMinIndex)
	{
		m_pointList.erase(m_pointList.begin() + nManIndex);
		m_pointList.erase(m_pointList.begin() + nMinIndex);
	}
	else
	{
		m_pointList.erase(m_pointList.begin() + nMinIndex);
		m_pointList.erase(m_pointList.begin() + nManIndex);
	}
}

void CKernelAlgorithm::setWindowShape(int windowShape)
{
	m_windowShape = windowShape;
}

double CKernelAlgorithm::kernelK2(double x, double y, int algorithm)
{
	double result;
	if (0 == m_windowShape)
	{//方形
		if (sqrt(pow(x, 2) + pow(y, 2)) > 1)
		{
			result = 0;
			return result;
		}
	}
	else if (1 == m_windowShape)
	{//圆形
		if (sqrtf(x*x + y*y) > 1)
		{
			result = 0;
			return result;
		}
	}
	switch (algorithm)
	{
	case Tricube:
		result = (1.0 - abs(x))*(1.0 - abs(y));
		break;
	case Boxcar:
		result = 1.0 / 4.0;
		break;
	case Gaussian:
		result = 1.0 / sqrt(2 * PI) * exp(-(pow(x, 2) + pow(y, 2)) / 2);
		break;
	case Epanechnikov:
		result = 9.0 / 16.0 * (1 - pow(x, 2)) * (1 - pow(y, 2));
		break;
	default:
		break;
	}
	return result;
}//double kernelK2(double x, double y, int flag)

QVector<double> & CKernelAlgorithm::kernelK2_KNear(QVector<double> & x_list, QVector<double> & y_list, int algorithm)
{
	m_result.clear();

	switch (algorithm)
	{
	case Tricube:
		fTricube(m_result, x_list, y_list);
		break;
	case Boxcar:
		fBoxcar(m_result, x_list, y_list);
		break;
	case Gaussian:
		fGaussian(m_result, x_list, y_list);
		break;
	case Epanechnikov:
		fEpanechnikov(m_result, x_list, y_list);
		break;
	default:
		break;
	}

	for (int i = 0; i < x_list.size(); ++i)
	{
		if (pow(x_list[i], 2) + pow(y_list[i], 2) > 1)
		{
			m_result[i] = 0;
		}
	}

	return m_result;
}

void CKernelAlgorithm::fTricube(QVector<double> & result, QVector<double> & x_list, QVector<double> & y_list)
{
	for (int i = 0; i < x_list.size(); ++i)
	{
		result.push_back((1.0 - abs(x_list[i]))*(1.0 - abs(y_list[i])));
	}
}

void CKernelAlgorithm::fBoxcar(QVector<double> & result, QVector<double> & x_list, QVector<double> & y_list)
{
	for (int i = 0; i < x_list.size(); ++i)
	{
		result.push_back(0.25);
	}
}

void CKernelAlgorithm::fGaussian(QVector<double> & result, QVector<double> & x_list, QVector<double> & y_list)
{
	for (int i = 0; i < x_list.size(); ++i)
	{
		result.push_back(1.0 / sqrt(2 * PI) * exp(-(pow(x_list[i], 2) + pow(y_list[i], 2)) / 2));
	}
}

void CKernelAlgorithm::fEpanechnikov(QVector<double> & result, QVector<double> & x_list, QVector<double> & y_list)
{
	for (int i = 0; i < x_list.size(); ++i)
	{
		result.push_back(9.0 / 16.0 * (1 - pow(x_list[i], 2)) * (1 - pow(y_list[i], 2)));
	}
}

//vector<string> & CKernelAlgorithm::getKernelFuncList()
//{
//	return m_kernelFuncList;
//}

double CKernelAlgorithm::originalKernelRegress(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType)
{
	//1.计算权值，并求和
	double dbTotalK = 0;
	QString tmp;
	for (int nPointIndex = 0; nPointIndex < originalPointList.size(); ++nPointIndex)
	{//
		double dbTempK = 0;
		dbTempK = kernelK2((originalPointList[nPointIndex].x - spPredict.x) / h, (originalPointList[nPointIndex].y - spPredict.y) / h, kernelFuncType);
		//cout << dbTempK << endl;
		dbTotalK += dbTempK;
		//cout << originalPointList[nPointIndex].y << endl;
		tmp += QString::number(dbTempK) + " ";
	}
	//cout << "------------------------------------" << endl;

	//2.权值全部为0，说明在该窗口的范围内没有点，则增大窗宽，直到权值不为0为止
	double temp_h = h;
	while (IS_DOUBLE_ZERO(dbTotalK))
	{//
		temp_h += 0.5;
		dbTotalK = 0;
		for (int nPointIndex = 0; nPointIndex < originalPointList.size(); ++nPointIndex)
		{
			//K = W1 + W2 + ... + Wn
			//排除正在预测的点
			dbTotalK += kernelK2((originalPointList[nPointIndex].x - spPredict.x) / temp_h, (originalPointList[nPointIndex].y - spPredict.y) / temp_h, kernelFuncType);
		}
	}

	//3.N_predict = W1 * N1 + W2 * N2 + ... + Wn * Nn
	double sum_k_z = 0;
	
	for (int nPointIndex = 0; nPointIndex < originalPointList.size(); ++nPointIndex)
	{
		//排除正在预测的点
		double temp_z = 0;
		temp_z = kernelK2((originalPointList[nPointIndex].x - spPredict.x) / temp_h, (originalPointList[nPointIndex].y - spPredict.y) / temp_h, kernelFuncType) * originalPointList[nPointIndex].N;
		sum_k_z += temp_z;
		//cout << temp_z << endl;
		
	}

	//cout << "------------------------------------" << endl;

	//4.计算预测结果
	spPredict.N_predict = sum_k_z / dbTotalK;
	spPredict.errors = fabs(spPredict.N - spPredict.N_predict);

	return spPredict.N_predict;
}

double CKernelAlgorithm::increaseSampleKernelRegress(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType)
{
	vector<SPoint> newOriginalPointList = originalPointList;
	//1.找窗内样本点
	vector<SPoint> samplePoint;
	for (int nPointIndex = 0; nPointIndex < originalPointList.size(); ++nPointIndex)
	{//
	 //排除正在预测的点
		if (!(IS_DOUBLE_ZERO(originalPointList[nPointIndex].x - spPredict.x) && IS_DOUBLE_ZERO(originalPointList[nPointIndex].y - spPredict.y)))
		{
			double currentK = kernelK2((originalPointList[nPointIndex].x - spPredict.x) / h, (originalPointList[nPointIndex].y - spPredict.y) / h, kernelFuncType);

			if (!IS_DOUBLE_ZERO(currentK))
			{//该点对应的权值不为零，说明该点在窗内
				//保存找到的这些点，作为样本点
				SPoint spNew = originalPointList[nPointIndex];
				//spNew.x = fabs(spPredict.x + originalPointList[nPointIndex].x) / 2;
				//spNew.y = fabs(spPredict.y + originalPointList[nPointIndex].y) / 2;
				//spNew.N = 0;
				samplePoint.push_back(spNew);
				//cout << originalPointList[nPointIndex].y << endl;
			}
		}
	}

	//2.增加样本点
	for (int nIndex = 0; nIndex < samplePoint.size(); ++nIndex)
	{
		SPoint sp;
		sp.x = fabs((samplePoint[nIndex].x + spPredict.x)) / 2;
		sp.y = fabs((samplePoint[nIndex].y + spPredict.y)) / 2;
		sp.N = originalKernelRegress(originalPointList, sp, h, kernelFuncType);//计算样本点的场强值

		//cout << sp.N << endl;
		newOriginalPointList.push_back(sp);//将计算得到的样本点添加到原始数据后面，准备开始计算样本点的场强值
	}

	//3.计算预测点的场强值
	return originalKernelRegress(newOriginalPointList, spPredict, h, kernelFuncType);
}

double CKernelAlgorithm::increaseSampleKernelRegress_cutBadPoint(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType)
{
	vector<SPoint> tempPointList(originalPointList.begin(), originalPointList.end());

	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//找出原始数据中误差大的点
		originalKernelRegress(tempPointList, *it, h, kernelFuncType);
		//每预测一个点，根据预测结果判断误差，若误差>4 db,则认定该点为噪点
		if ((*it).errors > 4)
		{//当前点为噪点
			if (!((spPredict.x == (*it).x) && (spPredict.y == (*it).y)))
			{//不是当前点，从原始数据中剔除噪点
				it = tempPointList.erase(it);
				--it;
			}
			//是当前点，什么也不做(保留)
		}
	}
	//增加样本点预测
	return increaseSampleKernelRegress(tempPointList, spPredict, h, kernelFuncType);
}

double CKernelAlgorithm::errorsWeightKernelRegress(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType)
{
	vector<SPoint> tempPointList(originalPointList.begin(), originalPointList.end());//避免修改原始数据

	double dbReciprocalTotalErrors = 0;//误差倒数和
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//计算各各点的误差,h在这里可能被更新
		originalKernelRegress(tempPointList, *it, h, kernelFuncType);
		dbReciprocalTotalErrors += (1.0 / (*it).errors);
		//cout << (*it).errors << endl;
	}

	//误差归一化
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//计算各个点的误差权值
		(*it).errors_weight = (1.0 / (*it).errors) / dbReciprocalTotalErrors;
		//cout << (*it).errors_weight << endl;
	}


	double dbTotalK = 0;
	for (int nPointIndex = 0; nPointIndex < tempPointList.size(); ++nPointIndex)
	{//求各个点的权值
		double currentK = kernelK2((tempPointList[nPointIndex].x - spPredict.x) / h, (tempPointList[nPointIndex].y - spPredict.y) / h, kernelFuncType);
		tempPointList[nPointIndex].weight = currentK;
		//cout << currentK << endl;

		if (IS_DOUBLE_ZERO(currentK))//权值为0的点，误差权值也置为0
			tempPointList[nPointIndex].errors_weight = 0;
		else
			dbTotalK += currentK;
	}

	//权值归一化
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//计算各个点归一化权值
		(*it).weight_normalization = (*it).weight / dbTotalK;
		//cout << (*it).weight_normalization << endl;
	}

	//计算新的权值
	dbTotalK = 0;
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//计算各个点归一化权值
		(*it).weight = ((*it).weight_normalization - 0.9 * (*it).errors_weight);
		dbTotalK += (*it).weight;
		//cout << (*it).weight << endl;//显示新的权重
	}

	//3.N_predict = W1 * N1 + W2 * N2 + ... + Wn * Nn
	double sum_k_z = 0;
	for (int nPointIndex = 0; nPointIndex < tempPointList.size(); ++nPointIndex)
	{
		sum_k_z += tempPointList[nPointIndex].weight * tempPointList[nPointIndex].N;
	}

	//4.计算预测结果
	spPredict.N_predict = sum_k_z / dbTotalK;
	spPredict.errors = fabs(spPredict.N - spPredict.N_predict);
	return spPredict.N_predict;
}

double CKernelAlgorithm::logErrorsWeightKernelRegress(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType)
{
	vector<SPoint> tempPointList(originalPointList.begin(), originalPointList.end());//避免修改原始数据

	double dbReciprocalTotalErrors = 0;//误差倒数和
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//计算各各点的误差,h在这里可能被更新
		originalKernelRegress(tempPointList, *it, h, kernelFuncType);
		dbReciprocalTotalErrors += (1.0 / (*it).errors);
	}

	//误差归一化
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//计算各个点的误差权值
		(*it).errors_weight = (1.0 / (*it).errors) / dbReciprocalTotalErrors;
	}

	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//修正各个点的误差权值
		(*it).errors_weight = 0.5 * log((1 - (*it).errors_weight)/((*it).errors_weight));
	}

	double dbTotalK = 0;
	for (int nPointIndex = 0; nPointIndex < tempPointList.size(); ++nPointIndex)
	{//求各个点的权值
		double currentK = kernelK2((tempPointList[nPointIndex].x - spPredict.x) / h, (tempPointList[nPointIndex].y - spPredict.y) / h, kernelFuncType);
		tempPointList[nPointIndex].weight = currentK;

		if (IS_DOUBLE_ZERO(currentK))//权值为0的点，误差权值也置为0
			tempPointList[nPointIndex].errors_weight = 0;
		else
			dbTotalK += currentK;
	}

	//权值归一化
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//计算各个点归一化权值
		(*it).weight_normalization = (*it).weight / dbTotalK;
	}

	//计算新的权值
	dbTotalK = 0;
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//计算各个点归一化权值
		(*it).weight = ((*it).weight_normalization * (*it).errors_weight);
		dbTotalK += (*it).weight;
		//cout << (*it).weight << endl;//显示新的权重
	}

	//3.N_predict = W1 * N1 + W2 * N2 + ... + Wn * Nn
	double sum_k_z = 0;
	for (int nPointIndex = 0; nPointIndex < tempPointList.size(); ++nPointIndex)
	{
		sum_k_z += tempPointList[nPointIndex].weight * tempPointList[nPointIndex].N;
	}

	//4.计算预测结果
	spPredict.N_predict = sum_k_z / dbTotalK;
	spPredict.errors = fabs(spPredict.N - spPredict.N_predict);
	return spPredict.N_predict;
}

vector<SPoint> & CKernelAlgorithm::getPointList() 
{
	return m_pointList;
}

//vector<string> & CKernelAlgorithm::getPredictModelList()
//{
//	return m_predictModel;
//}

double CKernelAlgorithm::predict(int funcId, vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType)
{
	switch (funcId)
	{
	case 1:
		return originalKernelRegress(originalPointList, spPredict, h, kernelFuncType);
		break;
	case 2:
		return increaseSampleKernelRegress(originalPointList, spPredict, h, kernelFuncType);
		break;
	case 3 :
		return increaseSampleKernelRegress_cutBadPoint(originalPointList, spPredict, h, kernelFuncType);
		break;
	case 4:
		return errorsWeightKernelRegress(originalPointList, spPredict, h, kernelFuncType);
		break;
	case 5:
		return logErrorsWeightKernelRegress(originalPointList, spPredict, h, kernelFuncType);
		break;
	//case 6:
	//	return kNearKernelRegress(originalPointList, spPredict, h, kernelFuncType);
	//	break;
	default:
		break;
	}
}

double CKernelAlgorithm::predict(vector<SPoint> & pointList, SPoint & spPredict, double h, int kernelFuncType)
{
	QVector<double> X, Y;
	X.clear();
	Y.clear();

	for (int i = 0; i < pointList.size(); ++i)
	{
		X.push_back((pointList[i].x - spPredict.x) / h);
		Y.push_back((pointList[i].y - spPredict.y) / h);
	}
	QVector<double> & K = kernelK2_KNear(X, Y, kernelFuncType);

	double sumK = sum(K);
	if (IS_DOUBLE_ZERO(sumK))
	{
		return 0;
	}

	double sumKN = 0;
	for (int i = 0; i < pointList.size(); ++i)
	{
		sumKN += K[i] * pointList[i].N;
	}
	spPredict.N_predict =  sumKN / sumK;
	spPredict.errors = fabs(spPredict.N - spPredict.N_predict);
	return spPredict.N_predict;
}

double CKernelAlgorithm::sum(QVector<double> & K)
{
	double sumK = 0;
	for (int i = 0; i < K.size(); ++i)
	{
		sumK += K[i];
	}
	return sumK;
}