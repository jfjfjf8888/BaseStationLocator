#include "predictThread.h"
#include <QDebug>
#include <QtAlgorithms>
#include <QMessageBox>

predictThread::predictThread()
{
	rectIndex = 0;
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
	Rect rect;
	rect.startX = 0;
	rect.startY = 0;
	rect.lenth = m_lenth;
	rect.width = m_width;

	locateResult.clear();
	locate(result, rect);
	emit(locateRect(&locateResult));
	emit(locateEnd());//��λȫ������
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

void predictThread::locate(const QVector<CPointInfo *> & predictResult, Rect & rect)
{
	//��rect��ʾ�ľ���������ڳ����������1/10���򷵻�
	if (rect.lenth * rect.width < m_lenth * m_width * PART_OF_AREA)
		return;
	//1.�Գ��������е�ĳ�ǿֵ�Ӵ�С����
	//2.�ҵ�ǰ80%���20%�ĵ㳡ǿֵ�ָ��ĳ�ǿ��
	//3.ȷ��Ȧ�Ĵ�С������Ȧ�ĳ�*0.8������Ȧ�Ŀ�*0.8��ΪȦ�Ŀ�ȡ�
	//4.��Ȧ������ѭ��������ÿһ�����򣬶�ʱͳ��Ȧ�ڳ�ǿֵ����ǰ80%�ĵ���
	//  ȡ�����������Ŀ���Ϊ��һ����������ǰN�����ĸ�������ȣ���ϲ�
	//  ��Щ����Ϊ��һ����������ǰN�����������ȣ���ϲ���Щ����Ϊ��һ��
	//  ��Ȧ�ĳ�����
	//5.ѭ��1��4��ֱ��Ȧ���������ԭʼ���������10%���ڡ�
	QVector<CPointInfo *> tempResult(predictResult);//��������
	myQuickSortByFeild(tempResult, 0, tempResult.size() - 1);//��С��������
	reverse(tempResult.begin(), tempResult.end());//������תΪ�Ӵ�С����
	double criticalField = tempResult[(int)(tempResult.size() * PART_OF_FEILD)]->before_value;//�ҵ�ǰ80%���20%�ָ����ٽ�ֵ
	float cycleLenth = rect.lenth * PART_OF_LENTH;
	float cycleWidth = rect.width * PART_OF_LENTH;

	//��ʱ�洢���ֵ�������
	Rect maxFeildCountRect;
	memset(&maxFeildCountRect, 0, sizeof(Rect));
	int maxFeildCount = 0;
	QVector<CPointInfo *> maxRectData;
	bool hasData = false;

	//��Ȧ����
	for (float x = rect.startX; x <= (rect.startX + rect.lenth); x += m_pp.predictiveStepSize)
	{//X����ɨ��
		if (cycleLenth + x > rect.lenth) 
			break;

		for (float y = rect.startY; y <= (rect.startY + rect.width); y += m_pp.predictiveStepSize)
		{//Y����ɨ��
			if (cycleWidth + y > rect.width) 
				break;

			QVector<CPointInfo *> tempRectData;
			Rect tempRect;
			tempRect.startX = x;
			tempRect.startY = y;
			tempRect.lenth = cycleLenth;
			tempRect.width = cycleWidth;

			foundRectPointList(tempResult, tempRectData, tempRect);
			int nCount = rectPointListFeildMaxThancriticalCount(tempRectData, criticalField);
			if (nCount > maxFeildCount) {
				maxFeildCount = nCount;
				maxFeildCountRect = tempRect;
				maxRectData.clear();
				maxRectData.append(tempRectData);
				hasData = true;
			}
		}
	}
	//�洢�����ȫ����ɺ�����
	if (hasData)
		locateResult.push_back(maxFeildCountRect);
	locate(maxRectData, maxFeildCountRect);
}

void predictThread::myQuickSortByFeild(QVector<CPointInfo *> &vec, int low, int high)
{
	if (low < high)
	{
		int l = low;
		int r = high;
		CPointInfo * key = vec[l];//��¼keyֵ

		while (l < r)
		{
			while (l < r && key->before_value <= vec[r]->before_value)//�����������,�ҵ���һ��С��key��Ԫ��
				--r;
			vec[l] = vec[r];
			while (l < r && key->before_value >= vec[l]->before_value)//�������ұ���,�ҵ���һ������keyֵ��Ԫ��
				++l;
			vec[r] = vec[l];
		}
		vec[l] = key;//��ʵ��ʱl=r

		myQuickSortByFeild(vec, low, l - 1);
		myQuickSortByFeild(vec, r + 1, high);
	}
}

void predictThread::myQuickSortByX(QVector<CPointInfo *> &vec, int low, int high)
{
	if (low < high)
	{
		int l = low;
		int r = high;
		CPointInfo * key = vec[l];//��¼keyֵ

		while (l < r)
		{
			while (l < r && key->x <= vec[r]->x)//�����������,�ҵ���һ��С��key��Ԫ��
				--r;
			vec[l] = vec[r];
			while (l < r && key->x >= vec[l]->x)//�������ұ���,�ҵ���һ������keyֵ��Ԫ��
				++l;
			vec[r] = vec[l];
		}
		vec[l] = key;//��ʵ��ʱl=r

		myQuickSortByX(vec, low, l - 1);
		myQuickSortByX(vec, r + 1, high);
	}
}

void predictThread::myQuickSortByY(QVector<CPointInfo *> &vec, int low, int high)
{
	if (low < high)
	{
		int l = low;
		int r = high;
		CPointInfo * key = vec[l];//��¼keyֵ

		while (l < r)
		{
			while (l < r && key->y <= vec[r]->y)//�����������,�ҵ���һ��С��key��Ԫ��
				--r;
			vec[l] = vec[r];
			while (l < r && key->y >= vec[l]->y)//�������ұ���,�ҵ���һ������keyֵ��Ԫ��
				++l;
			vec[r] = vec[l];
		}
		vec[l] = key;//��ʵ��ʱl=r

		myQuickSortByY(vec, low, l - 1);
		myQuickSortByY(vec, r + 1, high);
	}
}

void predictThread::foundRectPointList(const QVector<CPointInfo *> & __INPUT__ predictResult, QVector<CPointInfo *> & __OUTPUT__ rectPointList, Rect & rect)
{
	//����X�ķ�Χ 
	float endX = rect.startX + rect.lenth;
	//����Y�ķ�Χ
	float endY = rect.startY + rect.width;

	//QVector<CPointInfo *> tmpPredictResult(predictResult);
	//myQuickSortByX(tmpPredictResult, 0, tmpPredictResult.size() - 1);

	////������Ҫ�����ݵ�������Χ
	//int nStartIndex = rect.startX / m_pp.predictiveStepSize;
	//int nEndIndex = nStartIndex + ((rect.lenth * rect.width) / m_pp.predictiveStepSize);

	//nStartIndex = nStartIndex - 10 > 0 ? nStartIndex - 10 : 0;
	//nEndIndex = nEndIndex + 10 < tmpPredictResult.size() ? nEndIndex + 10 : tmpPredictResult.size();

	//for (int i = nStartIndex; i < nEndIndex; ++i)//����ǰ�����µ��Կռ�
	//{
	//	if (tmpPredictResult[i]->x > rect.startX && tmpPredictResult[i]->x < endX)
	//	{//x����Ҫ�ķ�Χ��
	//		if (tmpPredictResult[i]->y > rect.startY && tmpPredictResult[i]->y < endY)
	//		{//y����Ҫ�ķ�Χ��
	//			rectPointList.push_back(tmpPredictResult[i]);//���浱ǰ��
	//		}
	//	}
	//}

	for (int i = 0; i < predictResult.size(); ++i)//����ǰ�����µ��Կռ�
	{
		if (predictResult[i]->x > rect.startX && predictResult[i]->x < endX)
		{//x����Ҫ�ķ�Χ��
			if (predictResult[i]->y > rect.startY && predictResult[i]->y < endY)
			{//y����Ҫ�ķ�Χ��
				rectPointList.push_back(predictResult[i]);//���浱ǰ��
			}
		}
	}
}

int predictThread::rectPointListFeildMaxThancriticalCount(QVector<CPointInfo *> & rectPointList, double criticalField)
{
	int nCount = 0;
	for (QVector<CPointInfo *>::iterator it = rectPointList.begin(); it != rectPointList.end(); ++it) {
		if ((*it)->before_value > criticalField || (*it)->before_value == criticalField) {
			nCount++;
		}
	}
	return nCount;
}