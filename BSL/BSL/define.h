#pragma once
#include <QString>
struct predictParameter
{
	QString standard;//ͨ����ʽ
	QString predictModel;//Ԥ��ģ��
	QString algorithm;//�㷨
	int hWindow;//����
	QString windowShape;//������״
	QString coreFunction;//�˺���
	float predictiveStepSize;//����
	QString predictMethod;//Ԥ�ⷽʽ ����Ԥ�� or ����Ԥ��

	float X;//����Ԥ�� X
	float Y;//����Ԥ�� Y
};

struct CPointInfo
{
	CPointInfo()
	{
		x = 0;
		y = 0;
		value = 0;
	}

	float x;
	float y;
	float value;
	float before_value;
	float after_value;
};