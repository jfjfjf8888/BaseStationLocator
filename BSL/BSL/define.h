#pragma once
#include <QString>

//宏定义 
#define Tricube       1
#define Boxcar        2
#define Gaussian      3
#define Epanechnikov  4

#define IS_DOUBLE_ZERO(d)  (abs(d) < MIN_VALUE)
#define MIN_VALUE 1e-8

#define PI            3.14159265358979323846 //pi

#define __INPUT__
#define __OUTPUT__

#define PART_OF_AREA 0.1 //临界面积
#define PART_OF_LENTH 0.8 //缩圈比例
#define PART_OF_FEILD 0.8 //缩圈临界场强比例

struct predictParameter
{
	QString standard;//通信制式
	QString predictModel;//预测模型
	QString algorithm;//算法
	int hWindow;//窗宽
	QString windowShape;//窗口形状
	QString coreFunction;//核函数
	float predictiveStepSize;//步长
	QString predictMethod;//预测方式 单点预测 or 区域预测
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
	float x;            //X坐标
	float y;            //Y坐标
	float N;            //真实值
	float N_predict;    //预测值
	float errors;       //误差
	float errors_weight;//误差权重
	float weight;       //当前点的权值
	float weight_normalization;//归一化的权值
};

struct Rect
{
	float startX;
	float startY;
	float lenth;
	float width;
};
