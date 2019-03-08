#ifndef ANALYSISER_H
#define ANALYSISER_H
#include <QVector>
#include <QBuffer>
#include "DataCombiner.h"

struct SStandard
{
	QString name;//业务
	QString standard;//制式
	QString transmissionDirection;//传输方向
	QString frequency;//中心频率(MHz)
	int id;//编号
};

struct STrackPoint
{
	int id;
	QString beforeTestValue; //屏蔽前测试
	QString afterTestValue;//屏蔽后测试
};

struct STestStandard
{
	int id;//当前点的ID，对应轨迹线里面的ID
	QString name;//业务
	QString standard;//制式
	QString beforeTestValue; //屏蔽前测试
	QString afterTestValue;//屏蔽后测试
	QString quality;

	QVector<STrackPoint> trackLine;//轨迹线
};

struct STestPoint
{
	QVector<STestStandard> standardList;//制式列表
};

class InputDataAnalysiser
{
public:
	InputDataAnalysiser();
	~InputDataAnalysiser();

	void setData(QVector<QByteArray> data)
	{ 
		clearPrivateData(); 
		m_data = data; 
		if (1 == data.size())
		{//只有一个文件时，直接开始分析
			startAnalysis();
			//之前版本读取的屏蔽后数据不对，以下修正此问题
			CorrectionData();
		}
		else if(1 < data.size())
		{//多个文件时，先将多个文件合成一个文件，在进行分析
			DataCombiner dc;
			dc.setData(m_data);
			QByteArray comData = dc.getData();
			m_data.clear();
			m_data.push_back(comData);
			startAnalysis();
			//之前版本读取的屏蔽后数据不对，以下修正此问题
			CorrectionData();
		}
		else
		{//没有内容，直接返回
			return;
		} 
		
	}

	QByteArray & getData()
	{
		return m_data[0];
	}

	void startAnalysis();

	inline QVector<SStandard> & getStandardList()
	{	
		return m_standardList;
	}

	inline int getStandardCount()
	{	
		return m_standardList.size();
	}
	
	inline int getTestPointCount()
	{	
		return m_nTestPointCount;
	}

	inline QVector<STestPoint> & getTestPointTotalList()
	{  
		return m_testPointList;   
	}

	inline QVector<STestStandard> & getTestStandardList(int nIndex)
	{
		return m_testPointList[nIndex].standardList;
	}

	//对应于宽松评估，该函数将屏蔽前屏蔽后数据去平均，替代测试前测试后数据
	void averageTestValue(QString mode);

	//对轨迹线数据取平均，1代表取屏蔽钱数据，2代表去屏蔽后数据
	QString average(QVector<STrackPoint> & trackLine, int flag);
private:

	//读取测试点个数
	void readPointCount();
	//读取标准列表
	void readStandardList();
	//读取测试点内容
	void readTestPointContext();
	//读取轨迹线
	void readTrackLine(STestPoint & stp);

	void clearPrivateData();

	//矫正数据
	/*
	 * 之前读取的数据，屏蔽后数据不对，故添加此函数，从轨迹线中选取一个屏蔽后最小值，
	 * 替换屏蔽后场强值
	*/
	void CorrectionData();

	//找到传入容器中屏蔽后最小值，并返回
	QString MinAfterTestValue(QVector<STrackPoint> & stpList);

	//传入场强字符串(带单位)，返回场强值，float格式
	float getdbValue(QString str);
private:
	QVector<QByteArray> m_data;//输入buffer
	QVector<SStandard> m_standardList;//制式列表
	QVector<STestPoint> m_testPointList;//测试点列表，包含轨迹线
	QVector<STestPoint> m_testPointListCopy;//测试点列表，包含轨迹线
	QBuffer m_buffer;
	int m_nTestPointCount;//测试点数
};

#endif // ANALYSISER_H
