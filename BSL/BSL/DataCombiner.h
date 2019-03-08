#ifndef DATACOMBINER_H
#define DATACOMBINER_H
#include <QVector>


class DataCombiner
{
public:
	DataCombiner();
	~DataCombiner();
	void setData(QVector<QByteArray> & data)
	{
		m_InputData = data;
	}

	QByteArray & getData()
	{
		combinte();
		return m_OutputData;
	}
private:
	//�ϳ�����
	void combinte();

	//����
	void sort();

	//��ȡbuffer�еĴ���ʱ�䣬�������ʱ��ת����UNIXʱ�������
	time_t readCreateTime(QByteArray & byte);

	//��ȡ�������в��Ե�ĸ���
	int getTestPointCount(QByteArray & byte);

	int getTotalTestPointCount();

	//��m_OutputData�����ò��Ե����
	void setTestPointCount(int nCount);

	//��from�е������޸ĺ����˽�г�Աm_OutputData��
	void insertData(QByteArray & from, int nCountStart);
private:
	QVector<QByteArray> m_InputData;
	QByteArray m_OutputData;
};

#endif // DATACOMBINER_H
