#include "bsl.h"
#include <QDomDocument>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>

BSL::BSL(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//连接选择场景界面信号和槽
	connect(&m_selectScense, SIGNAL(signal_scenseSelected(QString)),
		this, SLOT(slots_scenseSelected(QString)));

	//固定窗口大小。加载实景时，有的图片会异常的大
	//由于我用了布局管理，限制图片大小无效，布局管理
	//会自动根据图片大小调整窗口大小，有些图片加载后
	//会很难看，所以我固定了窗口大小，望高人指点如何破。
	this->setFixedSize(this->width(), this->height());

	init();

	connect(this, SIGNAL(plsInitCommunicationStandardComboBox(InputDataAnalysiser *)), 
		    &m_setting, SLOT(initCommunicationStandardComboBox(InputDataAnalysiser *)));
	connect(&m_setting, SIGNAL(dataInputed(QString, int , float)),
		    this, SLOT(slots_dataInputed(QString, int, float)));

	connect(&pThread, SIGNAL(predictStart()),
		&prw, SLOT(predictStart()));

	connect(&pThread, SIGNAL(predictEnd(QVector<CPointInfo *> *)),
		&prw, SLOT(pushPointList(QVector<CPointInfo *> *)));

	connect(&pThread, SIGNAL(locateRect(QVector<Rect> *)),
		&prw, SLOT(locateRect(QVector<Rect> *)));

	connect(&pThread, SIGNAL(locateEnd()),
		&prw, SLOT(locateEnd()));

	connect(&pThread, SIGNAL(locateStart()),
		&prw, SLOT(locateStart()));
}

BSL::~BSL()
{

}

void BSL::init()
{
	m_scenceName.clear();
	m_picPath.clear();
	m_scenceType = 0;
	m_lenth = 0;
	m_width = 0;
	m_r = 0;
	m_proportion = 0;
	m_screenshot.clear();
	m_nTestPointCount = 0;
	m_pointList.clear();

	m_testPointFilePathList.clear();

	ui.scenesLabel->clear();
	ui.scenesLabel->setText(u8"1.加载场景\n2.加载测试文件\n3.设置参数\n4.开始定位");
	m_fileContextList.clear();

	m_currentStandard.clear();
	m_currentIndex = 0;
	m_currentStepLenth = 0;
}

//加载场景按钮
void BSL::on_loadSencesToolButton_clicked()
{
	m_selectScense.exec();
}

//重置按钮
void BSL::on_resetToolButton_clicked()
{
	init();
}

//加载文件按钮
void BSL::on_loadFileToolButton_clicked()
{
	m_testPointFilePathList = QFileDialog::getOpenFileNames(
		this,
		tr(u8"打开文件"),
		tr(u8"."),
		tr(u8"二进制文件 (*.bin)"));

	if (m_testPointFilePathList.isEmpty()) {
		ui.statusBar->showMessage(u8"取消");
		return;
	}

	m_fileContextList.clear();
	for (int n = 0; n < m_testPointFilePathList.size(); ++n) {
		//验证文件是否存在
		QFileInfo fileInfo(m_testPointFilePathList.at(n));
		if ( false == fileInfo.exists()) {
			ui.statusBar->showMessage(u8"错误，文件不存在");
			return;
		}
		//文件存在，准备开始读取数据
		QFile file(m_testPointFilePathList.at(n));
		if (file.open(QIODevice::ReadOnly)) {
			m_fileContextList.push_back(file.readAll());
			file.close();
			continue;
		}
		QMessageBox::critical(0, u8"错误", u8"文件打开失败。");
	}
	//数据读取正确，将读取的数据传入分析器
	if (!m_pAnalysiser) {
		delete m_pAnalysiser;
		m_pAnalysiser = NULL;
	}
	m_pAnalysiser = new InputDataAnalysiser();
	m_pAnalysiser->setData(m_fileContextList);
	//校验场景与导入实测数据的测试点个数
	if (m_nTestPointCount != m_pAnalysiser->getTestPointCount()) {
		QMessageBox::critical(0, u8"错误", u8"场景中的测试点数量和实测数据测试点数量不符。");
		delete m_pAnalysiser;
		m_pAnalysiser = NULL;
		m_fileContextList.clear();
	}

	ui.statusBar->showMessage(u8"预处理测试数据完成。");
	emit(plsInitCommunicationStandardComboBox(m_pAnalysiser));
}

void BSL:: on_settingToolButton_clicked()
{
	m_setting.exec();
}

//运行按钮
void BSL::on_runToolButton_clicked()
{
	pp.standard = m_currentStandard;
	pp.predictModel = u8"非参数核回归";
	pp.algorithm = u8"K近邻-非参数核回归";//算法
	pp.hWindow = 5;//窗宽
	pp.windowShape = u8"方形区域";
	pp.coreFunction = "Gaussian";
	pp.predictiveStepSize = m_currentStepLenth;
	pp.predictMethod = u8"区域预测";

	//分割m_pointList，获取每个点的坐标以及编号
	QStringList pointXYList = m_pointList.split("-");
	QVector<STestPoint> totalPoint = m_pAnalysiser->getTestPointTotalList();

	//准备原始数据
	for (int n = 0; n < m_nTestPointCount; ++n)
	{
		SPoint sp;
		sp.id = getID(pointXYList.at(n));
		sp.x = getX(pointXYList.at(n)) * m_proportion;
		sp.y = getY(pointXYList.at(n)) * m_proportion;
		sp.N = getN(totalPoint, sp.id, m_currentStandard);//测试点场强值
		testPointList.push_back(sp);
	}

	if (pThread.isRunning())
		pThread.terminate();//如果线程还在计算，则先终止线程

	prw.setScenes(m_screenshot);
	prw.setProportion(m_proportion);
	prw.setStepLenth(m_currentStepLenth);
	prw.setScenesSize(m_lenth, m_width, m_r);
	prw.setCommunicationStandard(m_currentStandard);
	prw.show();

	pThread.setPredictData(pp, testPointList);
	pThread.setScenesInfo(m_lenth, m_width, m_r);
	pThread.start();//启动线程
}

void BSL::slots_scenseSelected(QString path)
{
	init();

	QDomDocument doc;
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
		return;
	if (!doc.setContent(&file))
		return;
	file.close();

	QDomElement docElem = doc.documentElement();
	QDomNode node = docElem.firstChild();
	QDomElement signature_element = docElem.toElement(); // try to convert the node to an element. 
														 //校验签名
	QString ss = signature_element.nodeName();
	if (signature_element.nodeName().contains("scenes")) {
		if (QString(u8"SC File Format") != signature_element.attribute("signature")) {//校验失败
			QMessageBox::critical(0, u8"错误", u8"文件签名校验失败。");
			return;
		}
	}
	else {//校验失败
		QMessageBox::critical(0, u8"错误", u8"文件签名校验失败。");
		return;
	}

#ifdef  _DEBUG
	QString currentPath = "D:/Project/Evaluator/Scenes/";
#else
	QString currentPath = qApp->applicationDirPath() + "/Scenes/";
#endif //  _DEBUG

	while (!node.isNull()) {
		QDomElement element = node.toElement(); // try to convert the node to an element.  
		if (!element.isNull()) {
			if (QString("info") == element.nodeName()) {
				m_scenceName = element.attribute("name");
				m_picPath = currentPath + element.attribute("picPath");
				m_scenceType = element.attribute("scenceType").toInt();;
				m_lenth = element.attribute("lenth").toFloat();
				m_width = element.attribute("width").toFloat();
				m_r = element.attribute("r").toFloat();
				m_proportion = element.attribute("proportion").toFloat();
				m_screenshot = currentPath + element.attribute("screenshot");
				m_nTestPointCount = element.attribute("testPointCount").toInt();
				m_pointList = element.attribute("testPointList");
			}
		}
		node = node.nextSibling();
	}
	//图片自适应Label大小
	QPixmap *pixmap = new QPixmap(m_picPath);
	pixmap->scaled(ui.scenesLabel->size(), Qt::KeepAspectRatio);
	ui.scenesLabel->setScaledContents(true);
	ui.scenesLabel->setPixmap(*pixmap);
}

void BSL::slots_dataInputed(QString standard, int index, float stepLenth)
{
	ui.currentsettingLabel->setText(
		u8"当前制式:" + standard + u8",当前步长:" + 
		QString::number(stepLenth) + u8"米"
	);
	m_currentStandard = standard;
	m_currentIndex = index;
	m_currentStepLenth = stepLenth;
}

int BSL::getID(QString str)
{
	QStringList list = str.split(".");
	return list.at(0).toInt();
}
float BSL::getX(QString str)
{
	QStringList list = str.split("(");
	list = list.at(1).split(",");
	//list = list.at(0).split("(");
	return list.at(0).toFloat();
}
float BSL::getY(QString str)
{
	QStringList list = str.split("(");
	list = list.at(1).split(",");
	list = list.at(1).split(")");
	return list.at(0).toFloat();
}

float BSL::getN(QVector<STestPoint> & data, int id, QString standard, int standradIndex)
{
	//名称 + 空格 + 制式
	QString result_str;

	//若standradIndex == -1 则表示区域预测，取当前索引作为制式的索引，否则为单点预测，最后一个参数为传入索引
	result_str = data[id - 1].standardList[standradIndex == -1 ? m_currentIndex : standradIndex].afterTestValue;
	QStringList temp = result_str.split("dB");
	return temp.at(0).toFloat();
}