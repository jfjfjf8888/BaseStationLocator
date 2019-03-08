#ifndef ANALYSISER_H
#define ANALYSISER_H
#include <QVector>
#include <QBuffer>
#include "DataCombiner.h"

struct SStandard
{
	QString name;//ҵ��
	QString standard;//��ʽ
	QString transmissionDirection;//���䷽��
	QString frequency;//����Ƶ��(MHz)
	int id;//���
};

struct STrackPoint
{
	int id;
	QString beforeTestValue; //����ǰ����
	QString afterTestValue;//���κ����
};

struct STestStandard
{
	int id;//��ǰ���ID����Ӧ�켣�������ID
	QString name;//ҵ��
	QString standard;//��ʽ
	QString beforeTestValue; //����ǰ����
	QString afterTestValue;//���κ����
	QString quality;

	QVector<STrackPoint> trackLine;//�켣��
};

struct STestPoint
{
	QVector<STestStandard> standardList;//��ʽ�б�
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
		{//ֻ��һ���ļ�ʱ��ֱ�ӿ�ʼ����
			startAnalysis();
			//֮ǰ�汾��ȡ�����κ����ݲ��ԣ���������������
			CorrectionData();
		}
		else if(1 < data.size())
		{//����ļ�ʱ���Ƚ�����ļ��ϳ�һ���ļ����ڽ��з���
			DataCombiner dc;
			dc.setData(m_data);
			QByteArray comData = dc.getData();
			m_data.clear();
			m_data.push_back(comData);
			startAnalysis();
			//֮ǰ�汾��ȡ�����κ����ݲ��ԣ���������������
			CorrectionData();
		}
		else
		{//û�����ݣ�ֱ�ӷ���
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

	//��Ӧ�ڿ����������ú���������ǰ���κ�����ȥƽ�����������ǰ���Ժ�����
	void averageTestValue(QString mode);

	//�Թ켣������ȡƽ����1����ȡ����Ǯ���ݣ�2����ȥ���κ�����
	QString average(QVector<STrackPoint> & trackLine, int flag);
private:

	//��ȡ���Ե����
	void readPointCount();
	//��ȡ��׼�б�
	void readStandardList();
	//��ȡ���Ե�����
	void readTestPointContext();
	//��ȡ�켣��
	void readTrackLine(STestPoint & stp);

	void clearPrivateData();

	//��������
	/*
	 * ֮ǰ��ȡ�����ݣ����κ����ݲ��ԣ�����Ӵ˺������ӹ켣����ѡȡһ�����κ���Сֵ��
	 * �滻���κ�ǿֵ
	*/
	void CorrectionData();

	//�ҵ��������������κ���Сֵ��������
	QString MinAfterTestValue(QVector<STrackPoint> & stpList);

	//���볡ǿ�ַ���(����λ)�����س�ǿֵ��float��ʽ
	float getdbValue(QString str);
private:
	QVector<QByteArray> m_data;//����buffer
	QVector<SStandard> m_standardList;//��ʽ�б�
	QVector<STestPoint> m_testPointList;//���Ե��б������켣��
	QVector<STestPoint> m_testPointListCopy;//���Ե��б������켣��
	QBuffer m_buffer;
	int m_nTestPointCount;//���Ե���
};

#endif // ANALYSISER_H
