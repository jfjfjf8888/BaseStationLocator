#pragma once
#include <QWidget>
#include "ui_predictResultDisplay.h"
#include "define.h"

#include <QVector>

class predictResult : public QWidget {
	Q_OBJECT

public:
	predictResult(QWidget * parent = Q_NULLPTR);
	~predictResult();

	void setScenes(QString imagePath);

	void setScenesSize(float lenth, float width, float r);

	inline void setCommunicationStandard(QString str) {
		m_communicationStandard = str;
		this->setWindowTitle(QString(u8"[" + m_communicationStandard + "]"));
	}

	//绘图信号
	void paintEvent(QPaintEvent *event) override;

	//绘制预测结果
	void paintPredictResult();

	//画定位结果
	void paintLocateRect();

	//设置约束比例，用于物理坐标向直角坐标系转换
	inline void setProportion(float proportion) { m_proportion = proportion; }

	//设置步长
	inline void setStepLenth(float stepLen) { m_stepLenth = stepLen; }

	void closeEvent(QCloseEvent *event) override;

public slots:
	void pushPointList(QVector<CPointInfo *> * pl);
	void predictStart();

	void locateStart();
	//void locateRect(Rect * rect);
	void locateRect(QVector<Rect> * locateResult);
	void locateEnd();
private:
	Ui::predictResultWidget ui;

	QString m_imagePath;//场景图片路径
	QVector<CPointInfo *> pointList;
	float m_proportion;
	float m_stepLenth;

	float m_lenth;
	float m_width;
	float m_r;
	//QString m_evaluateMode;//评估模式，严格评估|宽松评估
	QString m_communicationStandard;
	QVector<double> fieldList;//场强列表
	double minField;//最小场强值

	QVector<Rect> rectList;
};
