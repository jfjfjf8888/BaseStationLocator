#include "KernelAlgorithm.h"

#include <cmath>
#include <iostream>
using namespace std;

CKernelAlgorithm::CKernelAlgorithm()
{
	m_pointList.clear();

	//�˺���
	//m_kernelFuncList.push_back("Tricube");
	//m_kernelFuncList.push_back("Boxcar");
	//m_kernelFuncList.push_back("Gaussian");
	//m_kernelFuncList.push_back("Epanechnikov");

	//m_predictModel.push_back("���Իع�");
	//m_predictModel.push_back("����������-���Իع�");
	//m_predictModel.push_back("����������-ȥ��-���Իع�");
	//m_predictModel.push_back("����Ȩ-���Իع�");
	//m_predictModel.push_back("����Ȩ2-���Իع�");
	m_windowShape = 1;//Ĭ�ϴ�������ΪԲ��
}


CKernelAlgorithm::~CKernelAlgorithm()
{
}

void CKernelAlgorithm::setXYNList(double X[], double Y[], double N[], int nCount)
{
	int nManIndex = -1;//���������
	int nMinIndex = -1;//��С�������

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
	//�޳��������С��
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
	{//����
		if (sqrt(pow(x, 2) + pow(y, 2)) > 1)
		{
			result = 0;
			return result;
		}
	}
	else if (1 == m_windowShape)
	{//Բ��
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
	//1.����Ȩֵ�������
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

	//2.Ȩֵȫ��Ϊ0��˵���ڸô��ڵķ�Χ��û�е㣬�����󴰿�ֱ��Ȩֵ��Ϊ0Ϊֹ
	double temp_h = h;
	while (IS_DOUBLE_ZERO(dbTotalK))
	{//
		temp_h += 0.5;
		dbTotalK = 0;
		for (int nPointIndex = 0; nPointIndex < originalPointList.size(); ++nPointIndex)
		{
			//K = W1 + W2 + ... + Wn
			//�ų�����Ԥ��ĵ�
			dbTotalK += kernelK2((originalPointList[nPointIndex].x - spPredict.x) / temp_h, (originalPointList[nPointIndex].y - spPredict.y) / temp_h, kernelFuncType);
		}
	}

	//3.N_predict = W1 * N1 + W2 * N2 + ... + Wn * Nn
	double sum_k_z = 0;
	
	for (int nPointIndex = 0; nPointIndex < originalPointList.size(); ++nPointIndex)
	{
		//�ų�����Ԥ��ĵ�
		double temp_z = 0;
		temp_z = kernelK2((originalPointList[nPointIndex].x - spPredict.x) / temp_h, (originalPointList[nPointIndex].y - spPredict.y) / temp_h, kernelFuncType) * originalPointList[nPointIndex].N;
		sum_k_z += temp_z;
		//cout << temp_z << endl;
		
	}

	//cout << "------------------------------------" << endl;

	//4.����Ԥ����
	spPredict.N_predict = sum_k_z / dbTotalK;
	spPredict.errors = fabs(spPredict.N - spPredict.N_predict);

	return spPredict.N_predict;
}

double CKernelAlgorithm::increaseSampleKernelRegress(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType)
{
	vector<SPoint> newOriginalPointList = originalPointList;
	//1.�Ҵ���������
	vector<SPoint> samplePoint;
	for (int nPointIndex = 0; nPointIndex < originalPointList.size(); ++nPointIndex)
	{//
	 //�ų�����Ԥ��ĵ�
		if (!(IS_DOUBLE_ZERO(originalPointList[nPointIndex].x - spPredict.x) && IS_DOUBLE_ZERO(originalPointList[nPointIndex].y - spPredict.y)))
		{
			double currentK = kernelK2((originalPointList[nPointIndex].x - spPredict.x) / h, (originalPointList[nPointIndex].y - spPredict.y) / h, kernelFuncType);

			if (!IS_DOUBLE_ZERO(currentK))
			{//�õ��Ӧ��Ȩֵ��Ϊ�㣬˵���õ��ڴ���
				//�����ҵ�����Щ�㣬��Ϊ������
				SPoint spNew = originalPointList[nPointIndex];
				//spNew.x = fabs(spPredict.x + originalPointList[nPointIndex].x) / 2;
				//spNew.y = fabs(spPredict.y + originalPointList[nPointIndex].y) / 2;
				//spNew.N = 0;
				samplePoint.push_back(spNew);
				//cout << originalPointList[nPointIndex].y << endl;
			}
		}
	}

	//2.����������
	for (int nIndex = 0; nIndex < samplePoint.size(); ++nIndex)
	{
		SPoint sp;
		sp.x = fabs((samplePoint[nIndex].x + spPredict.x)) / 2;
		sp.y = fabs((samplePoint[nIndex].y + spPredict.y)) / 2;
		sp.N = originalKernelRegress(originalPointList, sp, h, kernelFuncType);//����������ĳ�ǿֵ

		//cout << sp.N << endl;
		newOriginalPointList.push_back(sp);//������õ�����������ӵ�ԭʼ���ݺ��棬׼����ʼ����������ĳ�ǿֵ
	}

	//3.����Ԥ���ĳ�ǿֵ
	return originalKernelRegress(newOriginalPointList, spPredict, h, kernelFuncType);
}

double CKernelAlgorithm::increaseSampleKernelRegress_cutBadPoint(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType)
{
	vector<SPoint> tempPointList(originalPointList.begin(), originalPointList.end());

	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//�ҳ�ԭʼ����������ĵ�
		originalKernelRegress(tempPointList, *it, h, kernelFuncType);
		//ÿԤ��һ���㣬����Ԥ�����ж��������>4 db,���϶��õ�Ϊ���
		if ((*it).errors > 4)
		{//��ǰ��Ϊ���
			if (!((spPredict.x == (*it).x) && (spPredict.y == (*it).y)))
			{//���ǵ�ǰ�㣬��ԭʼ�������޳����
				it = tempPointList.erase(it);
				--it;
			}
			//�ǵ�ǰ�㣬ʲôҲ����(����)
		}
	}
	//����������Ԥ��
	return increaseSampleKernelRegress(tempPointList, spPredict, h, kernelFuncType);
}

double CKernelAlgorithm::errorsWeightKernelRegress(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType)
{
	vector<SPoint> tempPointList(originalPointList.begin(), originalPointList.end());//�����޸�ԭʼ����

	double dbReciprocalTotalErrors = 0;//������
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//�������������,h��������ܱ�����
		originalKernelRegress(tempPointList, *it, h, kernelFuncType);
		dbReciprocalTotalErrors += (1.0 / (*it).errors);
		//cout << (*it).errors << endl;
	}

	//����һ��
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//�������������Ȩֵ
		(*it).errors_weight = (1.0 / (*it).errors) / dbReciprocalTotalErrors;
		//cout << (*it).errors_weight << endl;
	}


	double dbTotalK = 0;
	for (int nPointIndex = 0; nPointIndex < tempPointList.size(); ++nPointIndex)
	{//��������Ȩֵ
		double currentK = kernelK2((tempPointList[nPointIndex].x - spPredict.x) / h, (tempPointList[nPointIndex].y - spPredict.y) / h, kernelFuncType);
		tempPointList[nPointIndex].weight = currentK;
		//cout << currentK << endl;

		if (IS_DOUBLE_ZERO(currentK))//ȨֵΪ0�ĵ㣬���ȨֵҲ��Ϊ0
			tempPointList[nPointIndex].errors_weight = 0;
		else
			dbTotalK += currentK;
	}

	//Ȩֵ��һ��
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//����������һ��Ȩֵ
		(*it).weight_normalization = (*it).weight / dbTotalK;
		//cout << (*it).weight_normalization << endl;
	}

	//�����µ�Ȩֵ
	dbTotalK = 0;
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//����������һ��Ȩֵ
		(*it).weight = ((*it).weight_normalization - 0.9 * (*it).errors_weight);
		dbTotalK += (*it).weight;
		//cout << (*it).weight << endl;//��ʾ�µ�Ȩ��
	}

	//3.N_predict = W1 * N1 + W2 * N2 + ... + Wn * Nn
	double sum_k_z = 0;
	for (int nPointIndex = 0; nPointIndex < tempPointList.size(); ++nPointIndex)
	{
		sum_k_z += tempPointList[nPointIndex].weight * tempPointList[nPointIndex].N;
	}

	//4.����Ԥ����
	spPredict.N_predict = sum_k_z / dbTotalK;
	spPredict.errors = fabs(spPredict.N - spPredict.N_predict);
	return spPredict.N_predict;
}

double CKernelAlgorithm::logErrorsWeightKernelRegress(vector<SPoint> & originalPointList, SPoint & spPredict, double h, int kernelFuncType)
{
	vector<SPoint> tempPointList(originalPointList.begin(), originalPointList.end());//�����޸�ԭʼ����

	double dbReciprocalTotalErrors = 0;//������
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//�������������,h��������ܱ�����
		originalKernelRegress(tempPointList, *it, h, kernelFuncType);
		dbReciprocalTotalErrors += (1.0 / (*it).errors);
	}

	//����һ��
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//�������������Ȩֵ
		(*it).errors_weight = (1.0 / (*it).errors) / dbReciprocalTotalErrors;
	}

	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//��������������Ȩֵ
		(*it).errors_weight = 0.5 * log((1 - (*it).errors_weight)/((*it).errors_weight));
	}

	double dbTotalK = 0;
	for (int nPointIndex = 0; nPointIndex < tempPointList.size(); ++nPointIndex)
	{//��������Ȩֵ
		double currentK = kernelK2((tempPointList[nPointIndex].x - spPredict.x) / h, (tempPointList[nPointIndex].y - spPredict.y) / h, kernelFuncType);
		tempPointList[nPointIndex].weight = currentK;

		if (IS_DOUBLE_ZERO(currentK))//ȨֵΪ0�ĵ㣬���ȨֵҲ��Ϊ0
			tempPointList[nPointIndex].errors_weight = 0;
		else
			dbTotalK += currentK;
	}

	//Ȩֵ��һ��
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//����������һ��Ȩֵ
		(*it).weight_normalization = (*it).weight / dbTotalK;
	}

	//�����µ�Ȩֵ
	dbTotalK = 0;
	for (vector<SPoint>::iterator it = tempPointList.begin(); it != tempPointList.end(); ++it)
	{//����������һ��Ȩֵ
		(*it).weight = ((*it).weight_normalization * (*it).errors_weight);
		dbTotalK += (*it).weight;
		//cout << (*it).weight << endl;//��ʾ�µ�Ȩ��
	}

	//3.N_predict = W1 * N1 + W2 * N2 + ... + Wn * Nn
	double sum_k_z = 0;
	for (int nPointIndex = 0; nPointIndex < tempPointList.size(); ++nPointIndex)
	{
		sum_k_z += tempPointList[nPointIndex].weight * tempPointList[nPointIndex].N;
	}

	//4.����Ԥ����
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