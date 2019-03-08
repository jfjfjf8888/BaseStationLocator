#include "predictresult.hpp"
#include <QPainter>
#include <QDebug>

predictResult::predictResult(QWidget * parent) : QWidget(parent) {
	ui.setupUi(this);
	pointList.clear();
	QPalette palette(this->palette());
	palette.setColor(QPalette::Background, QColor("#B0C4DE"));
	this->setPalette(palette);
}

predictResult::~predictResult() {
	
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
	//painter.scale(1, -1);//Y轴向上翻转，翻转成正常平面直角坐标系

	QPixmap pixmap(m_imagePath);
	QRect rect(0, 0, pixmap.width(), pixmap.height());
	painter.drawPixmap(rect, pixmap);
	
	//着色
	paintPredictResult();

	//画提示文字
	drawPrompt();

	painter.end();
}


void predictResult::pushPointList(QVector<CPointInfo *> * pl)
{
	this->pointList = *pl;
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

	//为了节省绘图时间，首先将所有区域绘制成绿色，循环中不对绿色进行绘制
	QPixmap bk_pm((m_lenth / m_proportion) * 20, (m_width / m_proportion) * 20);
	bk_pm.fill(Qt::green);
	painter.drawPixmap(0, 0, bk_pm);

	QPixmap pixmap(m_stepLenth / m_proportion * 20, m_stepLenth / m_proportion * 20);
	//1.坐标转换，将物理坐标转换成坐标系坐标
	for (int i = 0; i < pointList.size(); ++i)
	{
		//将物理坐标系转换成直角坐标系

		if (pointList[i]->value < 6)
		{
			if ((pointList[i]->value < 6) && (pointList[i]->value >= 2))//低风险
				pixmap.fill(Qt::yellow);

			if (pointList[i]->value < 2)//不合格
				pixmap.fill(Qt::white);

			painter.drawPixmap(QPointF((pointList[i]->x / m_proportion) * 20, (pointList[i]->y / m_proportion) * 20), pixmap);

		}
	}
	painter.end();
	update();
}

void predictResult::drawPrompt()
{
	QPainter painter;
	painter.begin(this);

	QRect rect_green(this->size().width() - 770, 20, 10, 10);
	painter.setBrush(QBrush(Qt::green));
	painter.drawRect(rect_green);
	painter.drawText(this->size().width() - 750, 30, u8"干信比 > 6dB, 安全");

	QRect rect_yellow(this->size().width() - 520, 20, 10, 10);
	painter.setBrush(QBrush(Qt::yellow));
	painter.drawRect(rect_yellow);
	painter.drawText(this->size().width() - 500, 30, u8"6dB > 干信比 >  2dB, 有风险");

	//QRect rect_orange(this->size().width() - 270, 20, 10, 10);
	//painter.setBrush(QBrush(QColor(255, 160, 90)));//橙色
	//painter.drawRect(rect_orange);
	//painter.drawText(this->size().width() - 250, 30, u8" 9dB > 干信比 >  6dB, 中风险");

	//QRect rect_red(this->size().width() - 770, 40, 10, 10);
	//painter.setBrush(QBrush(Qt::red));
	//painter.drawRect(rect_red);
	//painter.drawText(this->size().width() - 750, 50, u8" 6dB > 干信比 >  3dB, 高风险");

	//QRect rect_white(this->size().width() - 520, 40, 10, 10);
	//painter.setBrush(QBrush(Qt::white));
	//painter.drawRect(rect_white);
	//painter.drawText(this->size().width() - 500, 50, u8" 3dB > 干信比, 不合格");

	QRect rect_orange(this->size().width() - 270, 20, 10, 10);
	painter.setBrush(QBrush(Qt::white));//白色
	painter.drawRect(rect_orange);
	painter.drawText(this->size().width() - 250, 30, u8" 2dB > 干信比, 不合格");

	update();

	painter.end();

}

void predictResult::closeEvent(QCloseEvent *event)
{
	pointList.clear();
	QWidget::closeEvent(event);
}

