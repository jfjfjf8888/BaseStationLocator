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

	//����ѡ�񳡾������źźͲ�
	connect(&m_selectScense, SIGNAL(signal_scenseSelected(QString)),
		this, SLOT(slots_scenseSelected(QString)));

	//�̶����ڴ�С������ʵ��ʱ���е�ͼƬ���쳣�Ĵ�
	//���������˲��ֹ�������ͼƬ��С��Ч�����ֹ���
	//���Զ�����ͼƬ��С�������ڴ�С����ЩͼƬ���غ�
	//����ѿ��������ҹ̶��˴��ڴ�С��������ָ������ơ�
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
	ui.scenesLabel->setText(u8"1.���س���\n2.���ز����ļ�\n3.���ò���\n4.��ʼ��λ");
	m_fileContextList.clear();

	m_currentStandard.clear();
	m_currentIndex = 0;
	m_currentStepLenth = 0;
}

//���س�����ť
void BSL::on_loadSencesToolButton_clicked()
{
	m_selectScense.exec();
}

//���ð�ť
void BSL::on_resetToolButton_clicked()
{
	init();
}

//�����ļ���ť
void BSL::on_loadFileToolButton_clicked()
{
	m_testPointFilePathList = QFileDialog::getOpenFileNames(
		this,
		tr(u8"���ļ�"),
		tr(u8"."),
		tr(u8"�������ļ� (*.bin)"));

	if (m_testPointFilePathList.isEmpty()) {
		ui.statusBar->showMessage(u8"ȡ��");
		return;
	}

	m_fileContextList.clear();
	for (int n = 0; n < m_testPointFilePathList.size(); ++n) {
		//��֤�ļ��Ƿ����
		QFileInfo fileInfo(m_testPointFilePathList.at(n));
		if ( false == fileInfo.exists()) {
			ui.statusBar->showMessage(u8"�����ļ�������");
			return;
		}
		//�ļ����ڣ�׼����ʼ��ȡ����
		QFile file(m_testPointFilePathList.at(n));
		if (file.open(QIODevice::ReadOnly)) {
			m_fileContextList.push_back(file.readAll());
			file.close();
			continue;
		}
		QMessageBox::critical(0, u8"����", u8"�ļ���ʧ�ܡ�");
	}
	//���ݶ�ȡ��ȷ������ȡ�����ݴ��������
	if (!m_pAnalysiser) {
		delete m_pAnalysiser;
		m_pAnalysiser = NULL;
	}
	m_pAnalysiser = new InputDataAnalysiser();
	m_pAnalysiser->setData(m_fileContextList);
	//У�鳡���뵼��ʵ�����ݵĲ��Ե����
	if (m_nTestPointCount != m_pAnalysiser->getTestPointCount()) {
		QMessageBox::critical(0, u8"����", u8"�����еĲ��Ե�������ʵ�����ݲ��Ե�����������");
		delete m_pAnalysiser;
		m_pAnalysiser = NULL;
		m_fileContextList.clear();
	}

	ui.statusBar->showMessage(u8"Ԥ�������������ɡ�");
	emit(plsInitCommunicationStandardComboBox(m_pAnalysiser));
}

void BSL:: on_settingToolButton_clicked()
{
	m_setting.exec();
}

//���а�ť
void BSL::on_runToolButton_clicked()
{
	pp.standard = m_currentStandard;
	pp.predictModel = u8"�ǲ����˻ع�";
	pp.algorithm = u8"K����-�ǲ����˻ع�";//�㷨
	pp.hWindow = 5;//����
	pp.windowShape = u8"��������";
	pp.coreFunction = "Gaussian";
	pp.predictiveStepSize = m_currentStepLenth;
	pp.predictMethod = u8"����Ԥ��";

	//�ָ�m_pointList����ȡÿ����������Լ����
	QStringList pointXYList = m_pointList.split("-");
	QVector<STestPoint> totalPoint = m_pAnalysiser->getTestPointTotalList();

	//׼��ԭʼ����
	for (int n = 0; n < m_nTestPointCount; ++n)
	{
		SPoint sp;
		sp.id = getID(pointXYList.at(n));
		sp.x = getX(pointXYList.at(n)) * m_proportion;
		sp.y = getY(pointXYList.at(n)) * m_proportion;
		sp.N = getN(totalPoint, sp.id, m_currentStandard);//���Ե㳡ǿֵ
		testPointList.push_back(sp);
	}

	if (pThread.isRunning())
		pThread.terminate();//����̻߳��ڼ��㣬������ֹ�߳�

	prw.setScenes(m_screenshot);
	prw.setProportion(m_proportion);
	prw.setStepLenth(m_currentStepLenth);
	prw.setScenesSize(m_lenth, m_width, m_r);
	prw.setCommunicationStandard(m_currentStandard);
	prw.show();

	pThread.setPredictData(pp, testPointList);
	pThread.setScenesInfo(m_lenth, m_width, m_r);
	pThread.start();//�����߳�
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
														 //У��ǩ��
	QString ss = signature_element.nodeName();
	if (signature_element.nodeName().contains("scenes")) {
		if (QString(u8"SC File Format") != signature_element.attribute("signature")) {//У��ʧ��
			QMessageBox::critical(0, u8"����", u8"�ļ�ǩ��У��ʧ�ܡ�");
			return;
		}
	}
	else {//У��ʧ��
		QMessageBox::critical(0, u8"����", u8"�ļ�ǩ��У��ʧ�ܡ�");
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
	//ͼƬ����ӦLabel��С
	QPixmap *pixmap = new QPixmap(m_picPath);
	pixmap->scaled(ui.scenesLabel->size(), Qt::KeepAspectRatio);
	ui.scenesLabel->setScaledContents(true);
	ui.scenesLabel->setPixmap(*pixmap);
}

void BSL::slots_dataInputed(QString standard, int index, float stepLenth)
{
	ui.currentsettingLabel->setText(
		u8"��ǰ��ʽ:" + standard + u8",��ǰ����:" + 
		QString::number(stepLenth) + u8"��"
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
	//���� + �ո� + ��ʽ
	QString result_str;

	//��standradIndex == -1 ���ʾ����Ԥ�⣬ȡ��ǰ������Ϊ��ʽ������������Ϊ����Ԥ�⣬���һ������Ϊ��������
	result_str = data[id - 1].standardList[standradIndex == -1 ? m_currentIndex : standradIndex].afterTestValue;
	QStringList temp = result_str.split("dB");
	return temp.at(0).toFloat();
}