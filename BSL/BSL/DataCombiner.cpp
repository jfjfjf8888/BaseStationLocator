#include "DataCombiner.h"
#include <QBuffer>
#include <ctime>
#include <QDebug>
#include <QFile>

// 转换形如 2014-08-01 12:38:22 的时间戳到unixtime   
time_t GetUnixTime(std::string TimeStamp)
{
	const char * pszTimeStamp = TimeStamp.c_str();
	struct tm stTime;
	memset(&stTime, 0, sizeof(stTime));
	sscanf(pszTimeStamp, "%d-%d-%d %d:%d:%d",
		&stTime.tm_year, &stTime.tm_mon, &stTime.tm_mday,
		&stTime.tm_hour, &stTime.tm_min, &stTime.tm_sec);
	stTime.tm_year -= 1900;
	stTime.tm_mon--;
	return mktime(&stTime);
}


DataCombiner::DataCombiner()
{
	m_OutputData.clear();
	m_InputData.clear();
}

DataCombiner::~DataCombiner()
{

}

void DataCombiner::combinte()
{
	sort();//对输入数据按照测试时间进行排序
	m_OutputData = m_InputData[0];//插入文件头以及第一个文件的内容
	int nTotalCount = getTotalTestPointCount();
	int currentTestPointCount = getTestPointCount(m_OutputData);
	setTestPointCount(nTotalCount);
	//int n = getTestPointCount(m_OutputData);
	for (int i = 1; i < m_InputData.size(); ++i)
	{//添加后续内容
		insertData(m_InputData[i], currentTestPointCount);
		currentTestPointCount += getTestPointCount(m_InputData[i]);
	}
//#ifdef _DEBUG
//	QFile file("D:/111111111111.bin");
//	file.open(QIODevice::WriteOnly);
//	file.write(m_OutputData);
//	file.close();
//#endif // _DEBUG
}

void DataCombiner::insertData(QByteArray & from, int nCountStart)
{
	QBuffer buffer;
	buffer.setData(from);
	if (buffer.open(QBuffer::ReadOnly))
	{
		QByteArray line = buffer.readLine();
		while (!line.isEmpty())
		{//查找数据开始处
			if (line.contains(u8"测试点,"))
			{//读取测试点
				//qDebug() << QString::fromUtf8(line);
				break;//找到需要读取内容处
			}
			line = buffer.readLine();
		}
		while (!line.isEmpty())
		{//读取数据并保存
			if (line.contains(u8"测试点,"))
			{
				QString lineUtf8 = QString::fromUtf8(line);
				QStringList lineList = lineUtf8.split(",");
				int nTestIndex = lineList.at(1).toInt() + nCountStart;
				QString replaceStrTo = u8"测试点," + QString::number(nTestIndex) + "\r\n";
				//qDebug() << QString::fromUtf8(replaceStrTo.toUtf8());
				m_OutputData.append(replaceStrTo.toUtf8());
				line = buffer.readLine();
				continue;
			}
			//qDebug() << QString::fromUtf8(line);
			m_OutputData.append(line);
			line = buffer.readLine();
		}
		buffer.close();
	}
}

time_t DataCombiner::readCreateTime(QByteArray & byte)
{
	QBuffer buffer;
	buffer.setData(byte);
	QString timeStr;

	if (buffer.open(QBuffer::ReadOnly))
	{
		QByteArray line = buffer.readLine();
		while (!line.isEmpty())
		{
			if (line.contains("<Date>"))
			{
				//获取到日期
				QString date = line.toStdString().c_str();
				line = buffer.readLine();
				QStringList list = date.split(",");
				timeStr += list.at(1) + " ";
				//获取到时间
				QString time = line.toStdString().c_str();
				list = time.split(",");
				timeStr += list.at(1);
				break;
			}
			line = buffer.readLine();
		}
	}

	buffer.close();

	timeStr = timeStr.replace("/", "-");
	//qDebug() << timeStr;
	return GetUnixTime(timeStr.toStdString());
}

void DataCombiner::sort()
{
	QByteArray temp;
	for (int i = 1; i < (int)m_InputData.size(); i++)
	{
		for (int j = (int)m_InputData.size() - 1; j >= i; j--)
		{
			if (readCreateTime(m_InputData[j]) < readCreateTime(m_InputData[j - 1]))
			{
				temp = m_InputData[j - 1];
				m_InputData[j - 1] = m_InputData[j];
				m_InputData[j] = temp;
			}

		}
	}
}

int DataCombiner::getTestPointCount(QByteArray & byte)
{
	QBuffer buffer;
	buffer.setData(byte);
	if (buffer.open(QBuffer::ReadOnly))
	{
		QByteArray line = buffer.readLine();
		while (!line.isEmpty())
		{
			if (line.contains(u8"测试点数"))
			{//读取测试点
				QString lineUtf8 = QString::fromUtf8(line);
				QStringList lineList = lineUtf8.split(",");
				return lineList.at(1).toInt();
			}
			line = buffer.readLine();
		}
		buffer.close();
	}

	return -1;
}

int DataCombiner::getTotalTestPointCount()
{
	int nCount = 0;
	for (int i = 0; i < m_InputData.size(); ++i)
	{
		nCount += getTestPointCount(m_InputData[i]);
	}
	return nCount;
}

void DataCombiner::setTestPointCount(int nCount)
{
	QBuffer buffer;
	buffer.setData(m_OutputData);
	if (buffer.open(QBuffer::ReadOnly))
	{
		QByteArray line = buffer.readLine();
		while (!line.isEmpty())
		{
			if (line.contains(u8"测试点数"))
			{//读取测试点
				//qDebug() << line;
				QString lineUtf8 = QString::fromUtf8(line);
				buffer.close();
				//int nIndex = m_OutputData.indexOf(line);
				//m_OutputData.remove(nIndex, line.length());
				QStringList lineList = lineUtf8.split(",");
				QString temp = lineList.at(0);
				temp += "," + QString::number(nCount) + "\r\n";
				//m_OutputData.insert(nIndex, temp.toUtf8());
				//qDebug() << m_OutputData;
				m_OutputData.replace(line, temp.toUtf8());
				break;
			}
			line = buffer.readLine();
		}
	}
}