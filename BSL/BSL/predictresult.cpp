#include "predictresult.hpp"
#include <QPainter>
#include <QDebug>

predictResult::predictResult(QWidget * parent) : QWidget(parent) 
{
	ui.setupUi(this);
	pointList.clear();
	rectList.clear();
}

predictResult::~predictResult() 
{
	
}

void predictResult::setScenes(QString imagePath)
{
	m_imagePath = imagePath;
	QSize size = QPixmap(m_imagePath).size();
	this->resize(size);
	this->setFixedSize(size);//设置窗体固定大小
}

void predictResult::setScenesSize(float lenth, float width, float r)
{
	m_lenth = lenth;
	m_width = width;
	m_r = r;
}

void predictResult::paintEvent(QPaintEvent *event)
{
	QPainter painter;
	painter.begin(this);
	QPixmap pixmap(m_imagePath);
	QRect rect(0, 0, pixmap.width(), pixmap.height());
	painter.drawPixmap(rect, pixmap);
	//着色
	paintPredictResult();
	//画定位框
	paintLocateRect();
	painter.end();
}


void predictResult::pushPointList(QVector<CPointInfo *> * pl)
{
	this->pointList = *pl;

	fieldList.clear();
	//场强大小从大到小排序
	for (QVector<CPointInfo *>::iterator it = pointList.begin(); it != pointList.end(); ++it) {
		fieldList.push_back((*it)->before_value);
	}
	qSort(fieldList.begin(), fieldList.end());
	minField = fieldList[0];
	this->repaint();//重绘界面
}

void predictResult::predictStart()
{
	pointList.clear();
}

void predictResult::paintPredictResult()
{
	if (!pointList.size())
		return;//没有需要绘制的像素点

	//有需要绘制的点
	QPainter painter;
	painter.begin(this);

	QSize windowsSize = this->size();
	//移动坐标轴顶点位置，从左上角移动到左下角
	painter.translate(20, windowsSize.height() - 20);

	painter.scale(1, -1);//Y轴向上翻转，翻转成正常平面直角坐标系
	//着色
	QPixmap pixmap(m_stepLenth / m_proportion * 20, m_stepLenth / m_proportion * 20);
	//1.坐标转换，将物理坐标转换成坐标系坐标
	for (int i = 0; i < pointList.size(); ++i)
	{
		int G = (int)((pointList.at(i)->before_value - minField) * 10/* 10 *放大差值，颜色变化更明显*/);
		if (G > 255) G = 255;
		//场强越强越绿
		pixmap.fill(QColor(0, G, 0));
		painter.drawPixmap(QPointF((pointList[i]->x / m_proportion) * 20, (pointList[i]->y / m_proportion) * 20), pixmap);
	}
	painter.end();
	update();
}

void predictResult::paintLocateRect()
{
	if (!rectList.size())
		return;//没有需要绘制的矩形

	//有需要绘制的矩形
	QPainter painter;
	painter.begin(this);

	QSize windowsSize = this->size();
	//移动坐标轴顶点位置，从左上角移动到左下角
	painter.translate(20, windowsSize.height() - 20);

	painter.scale(1, -1);//Y轴向上翻转，翻转成正常平面直角坐标系
	painter.setPen(Qt::red);
						 //着色
	//QPixmap pixmap(m_stepLenth / m_proportion * 20, m_stepLenth / m_proportion * 20);
	//QRectF rect();
	//1.坐标转换，将物理坐标转换成坐标系坐标
	//painter.drawRect((rectList[0].startX / m_proportion) * 20, (rectList[0].startY / m_proportion) * 20,
	//	(rectList[0].lenth / m_proportion) * 20, (rectList[0].width / m_proportion) * 20);
	for (int i = 0; i < rectList.size(); ++i)
	{
		//int G = (int)((pointList.at(i)->before_value - minField) * 10/* 10 *放大差值，颜色变化更明显*/);
		//if (G > 255) G = 255;
		//场强越强越绿
		//pixmap.fill(QColor(0, G, 0));
		//painter.drawPixmap(QPointF((pointList[i]->x / m_proportion) * 20, (pointList[i]->y / m_proportion) * 20), pixmap);
		painter.drawRect((rectList[i].startX / m_proportion) * 20, (rectList[i].startY / m_proportion) * 20, 
			(rectList[i].lenth / m_proportion) * 20, (rectList[i].width / m_proportion) * 20);
	}

	painter.end();
	update();
}


void predictResult::closeEvent(QCloseEvent *event)
{
	pointList.clear();
	rectList.clear();
	QWidget::closeEvent(event);
}


//void predictResult::locateRect(Rect * rect)
//{
//	//收到定位信息
//	Rect tRect;
//	tRect.startX = rect->startX;
//	tRect.startY = rect->startY;
//	tRect.width = rect->width;
//	tRect.lenth = rect->lenth;
//	rectList.push_back(tRect);
//}

void predictResult::locateRect(QVector<Rect> * locateResult)
{
	rectList = *locateResult;
}

void predictResult::locateEnd()
{
	//定位结束
}

void predictResult::locateStart()
{
	rectList.clear();
}