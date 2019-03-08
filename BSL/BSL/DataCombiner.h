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
	//合成数据
	void combinte();

	//排序
	void sort();

	//读取buffer中的创建时间，并把这个时间转换成UNIX时间戳返回
	time_t readCreateTime(QByteArray & byte);

	//获取数据流中测试点的个数
	int getTestPointCount(QByteArray & byte);

	int getTotalTestPointCount();

	//在m_OutputData中设置测试点个数
	void setTestPointCount(int nCount);

	//将from中的数据修改后插入私有成员m_OutputData中
	void insertData(QByteArray & from, int nCountStart);
private:
	QVector<QByteArray> m_InputData;
	QByteArray m_OutputData;
};

#endif // DATACOMBINER_H
