#include "predictThread.h"
#include <QDebug>
#include <QtAlgorithms>
#include <QMessageBox>

predictThread::predictThread()
{
}


predictThread::~predictThread()
{
}

void predictThread::predictThread::run()
{
	//��ʼԤ��
	CKernelAlgorithm ka;//��������������Ԥ�⣬һ����������ǰ��һ���������κ�
	//�������ݣ�׼����ʼԤ��
	ka.setPointList(m_pointList);
	ka.setWindowShape(0);
	kNear(ka);//K���ڷǲ����˻ع�
}

bool predictThread::isPointExist(SPoint & sp, int & index)
{
	//IS_DOUBLE_ZERO()
	for (int i = 0; i < m_pointList.size(); ++i)
	{
		if (IS_DOUBLE_ZERO(fabs(sp.x - m_pointList[i].x)))
		{
			if (IS_DOUBLE_ZERO(fabs(sp.y - m_pointList[i].y)))
			{
				index = i;
				return true;
			}
		}
	}
	return false;
}

void predictThread::setPredictData(predictParameter & pp,
	vector<SPoint> & pointList)
{
	m_pp = pp;
	m_pointList = pointList;
}


int predictThread::getFuncId()
{
	if (m_pp.algorithm == QString(u8"ԭʼ�ǲ����˻ع�"))
		return 1;
	if (m_pp.algorithm == QString(u8"����������-�ǲ����˻ع�"))
		return 2;
	if (m_pp.algorithm == QString(u8"����������-ȥ��-�ǲ����˻ع�"))
		return 3;
	if (m_pp.algorithm == QString(u8"����Ȩ-�ǲ����˻ع�"))
		return 4;
	if (m_pp.algorithm == QString(u8"�Ľ�������Ȩ-�ǲ����˻ع�"))
		return 5;
	if (m_pp.algorithm == QString(u8"K����-�ǲ����˻ع�"))
		return 6;
	return -1;
}

int predictThread::getCoreFuncId()
{
	if (m_pp.coreFunction == QString("Tricube"))
		return 1;
	if (m_pp.coreFunction == QString("Boxcar"))
		return 2;
	if (m_pp.coreFunction == QString("Gaussian"))
		return 3;
	if (m_pp.coreFunction == QString("Epanechnikov"))
		return 4;
	return -1;
}

void predictThread::setScenesInfo(float lenth, float width, float r)
{
	m_lenth = lenth;
	m_width = width;
	m_r = r;
}


void predictThread::kNear(CKernelAlgorithm & ka)
{
	result.clear();
	QVector<vector<SPoint> > pointListSet;//����ǰK 2~7 ������������������б�
	QVector<double> distanceList;//�����б�

	//������õĴ����б�
	//Kѭ������2��7
	for (int k = 2; k < 8; ++k)
	{
		vector<SPoint> pointList;
		for (float x = 0; x <= m_lenth; x += m_pp.predictiveStepSize)
		{
			for (float y = 0; y <= m_width; y += m_pp.predictiveStepSize)
			{
				distanceList.clear();
				if (isPointExist(x, y))
					continue;
				SPoint sp;
				sp.x = x;
				sp.y = y;
				//�Ҵ���
				for (int i = 0; i < m_pointList.size(); ++i) {
					double distance = sqrtf(powf(m_pointList[i].x - x, 2) + powf(m_pointList[i].y - y, 2));
					distanceList.push_back(distance);
				}
				qSort(distanceList.begin(), distanceList.end());
				//Ԥ��
				sp.N = ka.predict(m_pointList, sp, distanceList[k - 1], getCoreFuncId());
				pointList.push_back(sp);
			}
		}
		pointListSet.push_back(pointList);
	}

	//������������ɣ���ʼ��ԭʼ����������������������Ԥ��ԭ�����ڵĸ��㣬������׼ȷ��
	QVector<QVector<CPointInfo *> > kNearPointList;//Ԥ�����б�
	for (int nK = 0; nK < pointListSet.size(); ++nK)
	{
		QVector<CPointInfo *> vec_pi;
		for (int nPoint = 0; nPoint < m_pointList.size(); ++nPoint)
		{
			distanceList.clear();
			double predict;
			//׼��Ԥ������
			SPoint sp;
			sp.x = m_pointList[nPoint].x;
			sp.y = m_pointList[nPoint].y;

			//��ʼԤ��
			//׼��ԭʼ����,��ԭʼ�������ų�����Ԥ��ĵ�
			vector<SPoint> point = m_pointList;

			//ɾ����ҪԤ��ĵ�
			vector<SPoint>::iterator it = begin(point) + nPoint;
			point.erase(it);
			point.insert(point.end(), pointListSet[nK].begin(), pointListSet[nK].end());
			//�Ҵ���
			for (int i = 0; i < point.size(); ++i)
			{
				double distance = sqrtf(powf(point[i].x - sp.x, 2) + powf(point[i].y - sp.y, 2));
				distanceList.push_back(distance);
			}
			qSort(distanceList.begin(), distanceList.end());

			//��ʼԤ��
			predict = ka.predict(point, sp, distanceList[/*nK + 2*/ 2], getCoreFuncId());

			//��������Ԥ����
			CPointInfo * pi = new CPointInfo();
			pi->x = sp.x;
			pi->y = sp.y;
			pi->before_value = predict;

			vec_pi.push_back(pi);//����
		}
		kNearPointList.push_back(vec_pi);
	}

	//˼�룺��ÿһ��K��Ӧ��Ԥ������֣�ȡ����ߵĽ��
	//Kȡ2��7������Щȡֵ����һ����ѵģ������ȥ���ý�����ʾ
	int nScoreMax = 0, nIndexMax; //nScore������ߵ÷֣�nIndex����õ÷ֶ�Ӧ����������.
	QString str;
	for (int i = 0; i < kNearPointList.size(); ++i)
	{
		//��ѡ����ǰ��ѽ��
		int nScore = 0;
		for (int nIndex = 0; nIndex < m_pointList.size(); ++nIndex)
		{
			double N_before = kNearPointList[i][nIndex]->before_value;
			if (2 > (N_before - m_pointList[nIndex].N))
			{
				nScore++;
			}
		}
		qDebug() << u8"����ǰ��K=" + QString::number(i + 2) + u8"��ȷ��:" + QString::number(100 * ((float)nScore / m_pointList.size())) + "%";

		if (nScore > nScoreMax)
		{
			nScoreMax = nScore;
			nIndexMax = i;
		}
	}
	//������ǰ���κ�������ϲ������ȥ
	//����ԭʼ���ݲ�����ԭʼ���ݼ�����Ҫ������
	for (int i = 0; i < m_pointList.size(); ++i)
	{
		CPointInfo * pi = new CPointInfo();
		pi->x = m_pointList[i].x;
		pi->y = m_pointList[i].y;
		pi->before_value = m_pointList[i].N;
		result.push_back(pi);
	}

	//���ѡȡ������ǰ���κ������
	for (int i = 0; i < pointListSet[0].size(); ++i)
	{
		CPointInfo * pi = new CPointInfo();
		pi->before_value = pointListSet[nIndexMax][i].N;
		pi->x = pointListSet[nIndexMax][i].x;
		pi->y = pointListSet[nIndexMax][i].y;
		result.push_back(pi);
	}

	//������
	qDebug() << str;
	emit(predictEnd(&result));

	//Ԥ���������ʼ��λ
	locate();
}


double predictThread::foundPredictValue(double x, double y, QVector<CPointInfo *> & pl, int flag)
{
	for (int i = 0; i < pl.size(); ++i)
	{
		if (IS_DOUBLE_ZERO(pl[i]->x - x) && IS_DOUBLE_ZERO(pl[i]->y - y))
			return flag == 0 ? pl[i]->after_value : pl[i]->before_value;
	}
	return -999;
}

bool predictThread::isPointExist(float x, float y)
{
	for (int i = 0; i < m_pointList.size(); ++i)
	{
		if (IS_DOUBLE_ZERO(m_pointList[i].x - x) && IS_DOUBLE_ZERO(m_pointList[i].y - y))
			return true;
	}
	return false;
}

void predictThread::locate()
{
	//1.�Գ��������е�ĳ�ǿֵ�Ӵ�С����
	//2.�ҵ�ǰ80%���20%�ĵ㳡ǿֵ�ָ��ĳ�ǿ��
	//3.ȷ��Ȧ�Ĵ�С������Ȧ�ĳ�*0.8������Ȧ�Ŀ�*0.8��ΪȦ�Ŀ�ȡ�
	//4.��Ȧ������ѭ��������ÿһ�����򣬶�ʱͳ��Ȧ�ڳ�ǿֵ����ǰ80%�ĵ���
	//  ȡ�����������Ŀ���Ϊ��һ����������ǰN�����ĸ�������ȣ���ϲ�
	//  ��Щ����Ϊ��һ����������ǰN�����������ȣ���ϲ���Щ����Ϊ��һ��
	//  ��Ȧ�ĳ�����
	//5.ѭ��1��4��ֱ��Ȧ���������ԭʼ���������10%���ڡ�
}