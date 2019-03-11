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
	//开始预测
	CKernelAlgorithm ka;//两个对象来处理预测，一个处理屏蔽前，一个处理屏蔽后
	//设置数据，准备开始预测
	ka.setPointList(m_pointList);
	ka.setWindowShape(0);
	kNear(ka);//K近邻非参数核回归
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
	if (m_pp.algorithm == QString(u8"原始非参数核回归"))
		return 1;
	if (m_pp.algorithm == QString(u8"增加样本点-非参数核回归"))
		return 2;
	if (m_pp.algorithm == QString(u8"增加样本点-去噪-非参数核回归"))
		return 3;
	if (m_pp.algorithm == QString(u8"误差加权-非参数核回归"))
		return 4;
	if (m_pp.algorithm == QString(u8"改进的误差加权-非参数核回归"))
		return 5;
	if (m_pp.algorithm == QString(u8"K近邻-非参数核回归"))
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
	QVector<vector<SPoint> > pointListSet;//屏蔽前K 2~7 增加样本点的样本点列表
	QVector<double> distanceList;//距离列表

	//保存求得的窗宽列表
	//K循环，从2～7
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
				//找窗宽
				for (int i = 0; i < m_pointList.size(); ++i) {
					double distance = sqrtf(powf(m_pointList[i].x - x, 2) + powf(m_pointList[i].y - y, 2));
					distanceList.push_back(distance);
				}
				qSort(distanceList.begin(), distanceList.end());
				//预测
				sp.N = ka.predict(m_pointList, sp, distanceList[k - 1], getCoreFuncId());
				pointList.push_back(sp);
			}
		}
		pointListSet.push_back(pointList);
	}

	//样本点增加完成，开始用原始数据与增加样本点后的数据预测原本存在的个点，随后计算准确率
	QVector<QVector<CPointInfo *> > kNearPointList;//预测结果列表
	for (int nK = 0; nK < pointListSet.size(); ++nK)
	{
		QVector<CPointInfo *> vec_pi;
		for (int nPoint = 0; nPoint < m_pointList.size(); ++nPoint)
		{
			distanceList.clear();
			double predict;
			//准备预测数据
			SPoint sp;
			sp.x = m_pointList[nPoint].x;
			sp.y = m_pointList[nPoint].y;

			//开始预测
			//准备原始数据,从原始数据中排除正在预测的点
			vector<SPoint> point = m_pointList;

			//删除正要预测的点
			vector<SPoint>::iterator it = begin(point) + nPoint;
			point.erase(it);
			point.insert(point.end(), pointListSet[nK].begin(), pointListSet[nK].end());
			//找窗宽
			for (int i = 0; i < point.size(); ++i)
			{
				double distance = sqrtf(powf(point[i].x - sp.x, 2) + powf(point[i].y - sp.y, 2));
				distanceList.push_back(distance);
			}
			qSort(distanceList.begin(), distanceList.end());

			//开始预测
			predict = ka.predict(point, sp, distanceList[/*nK + 2*/ 2], getCoreFuncId());

			//处理并保存预测结果
			CPointInfo * pi = new CPointInfo();
			pi->x = sp.x;
			pi->y = sp.y;
			pi->before_value = predict;

			vec_pi.push_back(pi);//保存
		}
		kNearPointList.push_back(vec_pi);
	}

	//思想：给每一个K对应的预测结果打分，取分最高的结果
	//K取2～7，在这些取值中找一个最佳的，发射出去，让界面显示
	int nScoreMax = 0, nIndexMax; //nScore代表最高得分，nIndex代表该得分对应的数据索引.
	QString str;
	for (int i = 0; i < kNearPointList.size(); ++i)
	{
		//挑选屏蔽前最佳结果
		int nScore = 0;
		for (int nIndex = 0; nIndex < m_pointList.size(); ++nIndex)
		{
			double N_before = kNearPointList[i][nIndex]->before_value;
			if (2 > (N_before - m_pointList[nIndex].N))
			{
				nScore++;
			}
		}
		qDebug() << u8"屏蔽前：K=" + QString::number(i + 2) + u8"正确率:" + QString::number(100 * ((float)nScore / m_pointList.size())) + "%";

		if (nScore > nScoreMax)
		{
			nScoreMax = nScore;
			nIndexMax = i;
		}
	}
	//将屏蔽前屏蔽后数据组合并发射出去
	//放入原始数据并根据原始数据计算需要的数据
	for (int i = 0; i < m_pointList.size(); ++i)
	{
		CPointInfo * pi = new CPointInfo();
		pi->x = m_pointList[i].x;
		pi->y = m_pointList[i].y;
		pi->before_value = m_pointList[i].N;
		result.push_back(pi);
	}

	//组合选取的屏蔽前屏蔽后的数据
	for (int i = 0; i < pointListSet[0].size(); ++i)
	{
		CPointInfo * pi = new CPointInfo();
		pi->before_value = pointListSet[nIndexMax][i].N;
		pi->x = pointListSet[nIndexMax][i].x;
		pi->y = pointListSet[nIndexMax][i].y;
		result.push_back(pi);
	}

	//发射结果
	qDebug() << str;
	emit(predictEnd(&result));

	//预测结束，开始定位
	Rect rect;
	rect.startX = 0;
	rect.startY = 0;
	rect.lenth = m_lenth;
	rect.width = m_width;

	locateResult.clear();
	locate(result, rect);
	emit(locateRect(&locateResult));
	emit(locateEnd());//定位全部结束
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
	//若rect表示的矩形面积低于场景的面积的1/10，则返回
	if (rect.lenth * rect.width < m_lenth * m_width * PART_OF_AREA)
		return;
	//1.对场景内所有点的场强值从大到小排序
	//2.找到前80%与后20%的点场强值分割点的场强。
	//3.确定圈的大小，现有圈的长*0.8，现有圈的宽*0.8作为圈的宽度。
	//4.缩圈，长宽循环场景内每一个区域，定时统计圈内场强值处于前80%的点数
	//  取包含点数最多的框作为下一个场景，若前N个框点的个数都相等，则合并
	//  这些框作为下一个场景，若前N个框点数都相等，则合并这些框作为下一次
	//  缩圈的场景。
	//5.循环1～4，直到圈的面积在在原始场景面积的10%以内。
	QVector<CPointInfo *> tempResult(predictResult);//备份数据
	myQuickSortByFeild(tempResult, 0, tempResult.size() - 1);//从小到大排序
	reverse(tempResult.begin(), tempResult.end());//容器翻转为从大到小排序
	double criticalField = tempResult[(int)(tempResult.size() * PART_OF_FEILD)]->before_value;//找到前80%与后20%分割点的临界值
	float cycleLenth = rect.lenth * PART_OF_LENTH;
	float cycleWidth = rect.width * PART_OF_LENTH;

	//临时存储最大值相关内容
	Rect maxFeildCountRect;
	memset(&maxFeildCountRect, 0, sizeof(Rect));
	int maxFeildCount = 0;
	QVector<CPointInfo *> maxRectData;
	bool hasData = false;

	//缩圈过程
	for (float x = rect.startX; x <= (rect.startX + rect.lenth); x += m_pp.predictiveStepSize)
	{//X方向扫描
		if (cycleLenth + x > rect.lenth) 
			break;

		for (float y = rect.startY; y <= (rect.startY + rect.width); y += m_pp.predictiveStepSize)
		{//Y方向扫描
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
	//存储结果，全部完成后发射结果
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
		CPointInfo * key = vec[l];//记录key值

		while (l < r)
		{
			while (l < r && key->before_value <= vec[r]->before_value)//从右往左遍历,找到第一个小于key的元素
				--r;
			vec[l] = vec[r];
			while (l < r && key->before_value >= vec[l]->before_value)//从左往右遍历,找到第一个大于key值的元素
				++l;
			vec[r] = vec[l];
		}
		vec[l] = key;//其实此时l=r

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
		CPointInfo * key = vec[l];//记录key值

		while (l < r)
		{
			while (l < r && key->x <= vec[r]->x)//从右往左遍历,找到第一个小于key的元素
				--r;
			vec[l] = vec[r];
			while (l < r && key->x >= vec[l]->x)//从左往右遍历,找到第一个大于key值的元素
				++l;
			vec[r] = vec[l];
		}
		vec[l] = key;//其实此时l=r

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
		CPointInfo * key = vec[l];//记录key值

		while (l < r)
		{
			while (l < r && key->y <= vec[r]->y)//从右往左遍历,找到第一个小于key的元素
				--r;
			vec[l] = vec[r];
			while (l < r && key->y >= vec[l]->y)//从左往右遍历,找到第一个大于key值的元素
				++l;
			vec[r] = vec[l];
		}
		vec[l] = key;//其实此时l=r

		myQuickSortByY(vec, low, l - 1);
		myQuickSortByY(vec, r + 1, high);
	}
}

void predictThread::foundRectPointList(const QVector<CPointInfo *> & __INPUT__ predictResult, QVector<CPointInfo *> & __OUTPUT__ rectPointList, Rect & rect)
{
	//计算X的范围 
	float endX = rect.startX + rect.lenth;
	//计算Y的范围
	float endY = rect.startY + rect.width;

	//QVector<CPointInfo *> tmpPredictResult(predictResult);
	//myQuickSortByX(tmpPredictResult, 0, tmpPredictResult.size() - 1);

	////计算需要的数据的索引范围
	//int nStartIndex = rect.startX / m_pp.predictiveStepSize;
	//int nEndIndex = nStartIndex + ((rect.lenth * rect.width) / m_pp.predictiveStepSize);

	//nStartIndex = nStartIndex - 10 > 0 ? nStartIndex - 10 : 0;
	//nEndIndex = nEndIndex + 10 < tmpPredictResult.size() ? nEndIndex + 10 : tmpPredictResult.size();

	//for (int i = nStartIndex; i < nEndIndex; ++i)//索引前后留下弹性空间
	//{
	//	if (tmpPredictResult[i]->x > rect.startX && tmpPredictResult[i]->x < endX)
	//	{//x在需要的范围内
	//		if (tmpPredictResult[i]->y > rect.startY && tmpPredictResult[i]->y < endY)
	//		{//y在需要的范围内
	//			rectPointList.push_back(tmpPredictResult[i]);//保存当前点
	//		}
	//	}
	//}

	for (int i = 0; i < predictResult.size(); ++i)//索引前后留下弹性空间
	{
		if (predictResult[i]->x > rect.startX && predictResult[i]->x < endX)
		{//x在需要的范围内
			if (predictResult[i]->y > rect.startY && predictResult[i]->y < endY)
			{//y在需要的范围内
				rectPointList.push_back(predictResult[i]);//保存当前点
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