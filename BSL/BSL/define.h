#pragma once
#include <QString>
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

	float X;//单点预测 X
	float Y;//单点预测 Y
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