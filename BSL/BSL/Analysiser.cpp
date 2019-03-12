#include "Analysiser.h"
#include <QDebug>
#include <algorithm>

InputDataAnalysiser::InputDataAnalysiser()
{
}

InputDataAnalysiser::~InputDataAnalysiser()
{
	m_buffer.deleteLater();
	m_data.clear();
	m_standardList.clear();
	m_testPointList.clear();
	m_nTestPointCount = 0;
}

void InputDataAnalysiser::startAnalysis()
{
	clearPrivateData();
	//��ȡÿ���ļ�������
	for (int nFileIndex = 0; nFileIndex < m_data.size(); ++nFileIndex)
	{
		//��ȡ�ļ��ڲ�ÿһ������
		m_buffer.setData(m_data[nFileIndex]);
		if (m_buffer.open(QBuffer::ReadOnly))
		{
			//˳���ܱ䣬Ҫ��ȡ�������Ⱥ�˳��
			readPointCount();//�����Ե����
			readStandardList();//����ʽ�б�
			readTestPointContext();//�����Ե���ϸ����
		}
		m_buffer.close();
	}
}

void InputDataAnalysiser::readStandardList()
{
	QByteArray line = m_buffer.readLine();
	while (!line.isEmpty())
	{
		if (line.contains("[Shield Section]"))
		{//��ȡ��ʽ�б�
			line = m_buffer.readLine();
			int nIndex = 1;//��ʽ����
			while (true)
			{
				line = m_buffer.readLine();
				QString lineUtf8 = QString::fromUtf8(line);
				QStringList list = lineUtf8.split(",");
				if (list.size() < 5)
					break;
				SStandard st;
				st.name = list.at(0);
				st.standard = list.at(1);
				st.transmissionDirection = list.at(2);
				st.frequency = list.at(3);
				st.id = nIndex;
				m_standardList.push_back(st);
				nIndex++;
			}
			break;//��ʽ��ȡ��ϣ�����ѭ��
		}
		line = m_buffer.readLine();
	}
}

void InputDataAnalysiser::readPointCount()
{
	QByteArray line = m_buffer.readLine();
	while (!line.isEmpty())
	{
		QString lineUtf8 = line;// QString::fromUtf8(line);
		QStringList lineList = lineUtf8.split(",");

		if (line.contains(u8"���Ե���"))
		{//��ȡ���Ե�
			QString lineUtf8 = line;// QString::fromUtf8(line);
			QStringList lineList = lineUtf8.split(",");

			m_nTestPointCount = lineList.at(1).toInt();
			break;
		}
		line = m_buffer.readLine();
	}
}

void InputDataAnalysiser::averageTestValue(QString mode)
{
	if (QString(u8"�ϸ�����") == mode)
	{
		if (!m_testPointListCopy.isEmpty())
		{
			m_testPointList = m_testPointListCopy;
		}
		m_testPointListCopy.clear();
		return;
	}

	//�����ϸ��������������ݣ���ʼ����
	if (!m_testPointListCopy.isEmpty())
		return;//������m_testPointListCopy��Ϊ�գ����ʾ�Ѿ���������ݣ�����Ҫ�ٴδ���

	m_testPointListCopy.clear();
	m_testPointListCopy.append(m_testPointList);

	for (int i = 0; i < m_testPointList.size(); ++i)
	{
		for (int j = 0; j < m_testPointList[i].standardList.size(); ++j)
		{
			STestStandard & ststd = m_testPointList[i].standardList[j];

			ststd.beforeTestValue = average(ststd.trackLine, 1);
			ststd.afterTestValue = average(ststd.trackLine, 2);
		}
	}
}

QString InputDataAnalysiser::average(QVector<STrackPoint> & trackLine, int flag)
{
	int nTotal = trackLine.size();
	double dbTotolValue = 0;
	QString value;
	QStringList list;
	for (int i = 0; i < trackLine.size(); ++i)
	{
		value = 1 == flag ? trackLine[i].beforeTestValue :
			2 == flag ? trackLine[i].afterTestValue : "";

		list = value.split("dB");

		if (list.isEmpty())
			continue;
		dbTotolValue += list[0].toFloat();
	}

	return QString::number(dbTotolValue / nTotal) + "dBmV";
}

void InputDataAnalysiser::averageTopXValue(int x)
{
	topX = x;

	for (int i = 0; i < m_testPointList.size(); ++i)
	{
		for (int j = 0; j < m_testPointList[i].standardList.size(); ++j)
		{
			STestStandard & ststd = m_testPointList[i].standardList[j];

			//ststd.beforeTestValue = trackLineTopXAverage(ststd.trackLine, 1);
			//ststd.afterTestValue = trackLineTopXAverage(ststd.trackLine, 2);
	
			QVector<double> trackLinedb_before, trackLinedb_after;
			for (int i = 0; i < ststd.trackLine.size(); ++i) 
			{
				trackLinedb_before.push_back(strFieldToDouble(ststd.trackLine[i].beforeTestValue));
				trackLinedb_after.push_back(strFieldToDouble(ststd.trackLine[i].afterTestValue));
			}
			qSort(trackLinedb_before.begin(), trackLinedb_before.end());
			qSort(trackLinedb_after.begin(), trackLinedb_after.end());

			std::reverse(trackLinedb_before.begin(), trackLinedb_before.end());
			std::reverse(trackLinedb_after.begin(), trackLinedb_after.end());

			double beforeTestValue = 0, afterTestValue = 0;
			for (int n = 0; n < x; ++n)
			{
				beforeTestValue += trackLinedb_before[n];
				afterTestValue += trackLinedb_after[n];
			}
			ststd.beforeTestValue = QString::number(beforeTestValue / x) + "dBmV";
			ststd.afterTestValue = QString::number(afterTestValue / x) + "dBmV";
		}
	}
}

//void InputDataAnalysiser::myQuickSortByFeild(QVector<STrackPoint> & trackLine, int low, int high, int flag)
//{
//	if (low < high)
//	{
//		int l = low;
//		int r = high;
//		STrackPoint key = trackLine[l];//��¼keyֵ
//
//		while (l < r)
//		{
//			while (l < r && strFieldToDouble(1 == flag ? key.beforeTestValue : key.afterTestValue)
//				<= strFieldToDouble(1 == flag ? trackLine[l].beforeTestValue : trackLine[l].afterTestValue))//�����������,�ҵ���һ��С��key��Ԫ��
//				--r;
//			trackLine[l] = trackLine[r];
//			while (l < r && strFieldToDouble(1== flag ? key.beforeTestValue : key.afterTestValue) 
//				>= strFieldToDouble(1 == flag ? trackLine[l].beforeTestValue : trackLine[l].afterTestValue))//�������ұ���,�ҵ���һ������keyֵ��Ԫ��
//				++l;
//			trackLine[r] = trackLine[l];
//		}
//		trackLine[l] = key;//��ʵ��ʱl=r
//
//		myQuickSortByFeild(trackLine, low, l - 1, flag);
//		myQuickSortByFeild(trackLine, r + 1, high, flag);
//	}
//}
//
//QString InputDataAnalysiser::trackLineTopXAverage(QVector<STrackPoint> & trackLine, int flag)
//{
//	double dbTotolValue = 0;
//	
//	myQuickSortByFeild(trackLine, 0, trackLine.size() - 1, flag);
//
//	for (int n = 299; n > 299 - topX; --n) {
//		if (1 == flag)
//			dbTotolValue += strFieldToDouble(trackLine[n].beforeTestValue);
//		else if (2 == flag)
//			dbTotolValue += strFieldToDouble(trackLine[n].afterTestValue);
//	}
//	return QString::number(dbTotolValue / topX) + "dBmV";
//}

void InputDataAnalysiser::readTestPointContext()
{
	QByteArray line = m_buffer.readLine();
	while (!line.isEmpty())
	{
		QString lineUtf811 = QString::fromUtf8(line);
		QStringList lineList11 = lineUtf811.split(",");
		if (line.contains(u8"���Ե�"))
		{//��ȡ���Ե�
			STestPoint stp;//�½�һ�����Ե�
			line = m_buffer.readLine(); line = m_buffer.readLine(); line = m_buffer.readLine(); line = m_buffer.readLine();//��������Ҫ����
			for (int nStandardIndex = 0; nStandardIndex < getStandardCount(); ++nStandardIndex)
			{//��ʽѭ��
				STestStandard sstd;
				//ҵ��,��ʽ,����ǰ����,���κ����,����
				QString lineUtf8 = QString::fromUtf8(line);
				QStringList lineList = lineUtf8.split(",");
				sstd.id = nStandardIndex + 1;
				sstd.name = lineList.at(0);
				sstd.standard = lineList.at(1);
				sstd.beforeTestValue = lineList.at(2);
				sstd.afterTestValue = lineList.at(3);
				sstd.quality = lineList.at(4);
				//�켣��readTrackLine()�����ж�ȡ
				stp.standardList.push_back(sstd);
				line = m_buffer.readLine();//��ȡ��һ��
			}
			readTrackLine(stp);//���켣��
			m_testPointList.push_back(stp);//������ʽ���ݺ͹켣�����ݺ󣬴洢�ò��Ե�
		}
		line = m_buffer.readLine();
	}
}

void InputDataAnalysiser::readTrackLine(STestPoint & stp)
{
	int nStandardIndex = 0;
	QByteArray line = m_buffer.readLine();
	while (!line.isEmpty())
	{
		if (nStandardIndex == getStandardCount())
			break;//�켣�ߵĶ�ȡ��������ʽ��ȵ�ʱ���������ǰ��Ĺ켣���Ѿ���ȡ���
		if (line.contains(u8"�켣��"))
		{//��ȡ�켣��
			line = m_buffer.readLine(); line = m_buffer.readLine();//��������Ҫ����
			for (int nTrackPointIndex = 0; nTrackPointIndex < 300; ++nTrackPointIndex)
			{//�켣�����й̶���300����
				STrackPoint trackPoint;
				QString lineUtf8 = QString::fromUtf8(line);
				QStringList lineList = lineUtf8.split(",");
				trackPoint.id = lineList.at(0).toInt();
				trackPoint.beforeTestValue = lineList.at(1);
				trackPoint.afterTestValue = lineList.at(2);

				stp.standardList[nStandardIndex].trackLine.push_back(trackPoint);
				line = m_buffer.readLine();
			}
			nStandardIndex++;
		}
		line = m_buffer.readLine();
	}
}

void InputDataAnalysiser::clearPrivateData()
{
	m_standardList.clear();
	m_testPointList.clear();
	m_nTestPointCount = 0;
}

void InputDataAnalysiser::CorrectionData()
{
	for (int i = 0; i < m_testPointList.size(); ++i)
	{
		for (int j = 0; j < m_testPointList[i].standardList.size(); ++j)
		{
			STestStandard & ststd = m_testPointList[i].standardList[j];
			ststd.afterTestValue = MinAfterTestValue(ststd.trackLine);
		}
	}
}

QString InputDataAnalysiser::MinAfterTestValue(QVector<STrackPoint> & stpList)
{//����Сֵ����
	QString minValue = stpList[0].afterTestValue;
	for (int i = 1; i < stpList.size(); ++i)
	{
		if (getdbValue(stpList[i].afterTestValue) < getdbValue(minValue))
		{
			minValue = stpList[i].afterTestValue;
		}
	}
	return minValue;
}

float InputDataAnalysiser::getdbValue(QString str)
{
	QStringList list = str.split("dB");
	if (list.isEmpty())
		return 0;
	return list.at(0).toFloat();
}

double InputDataAnalysiser::strFieldToDouble(QString str)
{
	QStringList list = str.split("dB");

	return list[0].toDouble();
}