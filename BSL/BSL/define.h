#pragma once
#include <QString>

//�궨�� 
#define Tricube       1
#define Boxcar        2
#define Gaussian      3
#define Epanechnikov  4

#define IS_DOUBLE_ZERO(d)  (abs(d) < MIN_VALUE)
#define MIN_VALUE 1e-8

#define PI            3.14159265358979323846 //pi

#define __INPUT__
#define __OUTPUT__

#define PART_OF_AREA 0.1 //�ٽ����
#define PART_OF_LENTH 0.8 //��Ȧ����
#define PART_OF_FEILD 0.8 //��Ȧ�ٽ糡ǿ����

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

struct SPoint
{
	SPoint()
	{
		id = 0;
		x = 0;
		y = 0;
		N = 0;
		N_predict = 0;
		errors = 0;
		errors_weight = 0;
		weight = 0;
		weight_normalization = 0;
	}
	int id;
	float x;            //X����
	float y;            //Y����
	float N;            //��ʵֵ
	float N_predict;    //Ԥ��ֵ
	float errors;       //���
	float errors_weight;//���Ȩ��
	float weight;       //��ǰ���Ȩֵ
	float weight_normalization;//��һ����Ȩֵ
};

struct Rect
{
	float startX;
	float startY;
	float lenth;
	float width;
};
