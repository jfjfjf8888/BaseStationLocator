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

	m_testPointFilePath.clear();

	ui.scenesLabel->clear();
	ui.scenesLabel->setText(u8"1.���س���\n2.���ز����ļ�\n3.������λ(�����ò���)");
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
	m_testPointFilePath = QFileDialog::getOpenFileName(
		this,
		tr(u8"���ļ�"),
		tr(u8"."),
		tr(u8"�������ļ� (*.bin)"));

	if (m_testPointFilePath.isEmpty())
	{
		ui.statusBar->showMessage(u8"ȡ��");
		return;
	}

	QFileInfo file(m_testPointFilePath);
	if (file.exists() == false)
	{
		ui.statusBar->showMessage(u8"�����ļ�������");
		return;
	}
}

//���а�ť
void BSL::on_runToolButton_clicked()
{
	
}

void BSL::slots_scenseSelected(QString path)
{
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
	if (signature_element.nodeName().contains("scenes"))
	{
		if (QString(u8"SC File Format") != signature_element.attribute("signature"))
		{//У��ʧ��
			QMessageBox::critical(0, u8"����", u8"�ļ�ǩ��У��ʧ�ܡ�");
			return;
		}
	}
	else
	{//У��ʧ��
		QMessageBox::critical(0, u8"����", u8"�ļ�ǩ��У��ʧ�ܡ�");
		return;
	}

#ifdef  _DEBUG
	QString currentPath = "D:/Project/Evaluator/Scenes/";
#else
	QString currentPath = qApp->applicationDirPath() + "/Scenes/";
#endif //  _DEBUG

	while (!node.isNull())
	{
		QDomElement element = node.toElement(); // try to convert the node to an element.  
		if (!element.isNull())
		{
			if (QString("info") == element.nodeName())
			{
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

	//ui.scenesPointLabel->setPixmap(m_screenshot);
}
