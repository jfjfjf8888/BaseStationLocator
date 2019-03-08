#include "Analysiser.h"
#include <QDebug>


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
{//ð������,����Сֵ����

	//�˴�ð�������ܴ����ã������ԭʼ���ݣ����⣬ð������̫��������ֻ��Ҫ����Сֵ���ʲ���
	//ð��������ԭʼ�������������Ч��

	//STrackPoint temp;
	//for (int i = 1; i < (int)stpList.size(); i++)
	//{
	//	for (int j = (int)stpList.size() - 1; j >= i; j--)
	//	{
	//		//if (atoi(flavorList[j].flavorName.c_str() + 6) < atoi(flavorList[j - 1].flavorName.c_str() + 6))
	//		//{
	//		//	temp = flavorList[j - 1];
	//		//	flavorList[j - 1] = flavorList[j];
	//		//	flavorList[j] = temp;
	//		//}

	//		if (getdbValue(stpList[j].afterTestValue) < getdbValue(stpList[j - 1].afterTestValue))
	//		{
	//			temp = stpList[j - 1];
	//			stpList[j - 1] = stpList[j];
	//			stpList[j] = temp;
	//		}

	//	}
	//}

	//return stpList[0].afterTestValue;
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