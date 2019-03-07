#include "selectscense.h"
#include <QDir>

selectScense::selectScense(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.listWidget->setFlow(QListWidget::LeftToRight);
}

selectScense::~selectScense()
{

}

int selectScense::exec()
{
	//ɨ�����Ŀ¼ScenesĿ¼���ҳ����г����ļ�
#ifdef _DEBUG
	QString scanPath = "D:/Project/Evaluator/Scenes";
#else //Release
	QString scanPath = qApp->applicationDirPath() + "/Scenes";
#endif
	scanScenesFile(scanPath);
	ui.listWidget->clear();
	//���볡���ļ�·�����ļ��������ļ�����ʾ��������
	parseFilePathAndShow();
	return QDialog::exec();
}

void selectScense::treeDoubleClicked(QModelIndex index)
{
	//int scences = index.data().toString() == QString(u8"Բ�γ���") ?  0 : 1;
	//�����źţ������źŽ������û�ѡ��ĳ���
	QString ss = index.data(Qt::WhatsThisRole).toString();
	emit(signal_scenseSelected(index.data(Qt::WhatsThisRole).toString()));
	//�رճ���ѡ�����
	this->close();
}

void selectScense::scanScenesFile(QString Path)
{
	FindFile(Path);
}

void selectScense::parseFilePathAndShow()
{
	QFileInfo fileInfo;
	for (int i = 0; i < m_scenesPathList.size(); ++i)
	{
		fileInfo.setFile(m_scenesPathList[i]);
		QString ss = fileInfo.baseName();
		QListWidgetItem * scense = new QListWidgetItem(fileInfo.baseName());
		scense->setIcon(QPixmap(":/BSL/Icon/scenes.png"));
		scense->setData(Qt::WhatsThisRole, m_scenesPathList[i]);
		ui.listWidget->addItem(scense);
	}
}

bool selectScense::FindFile(const QString & path)
{
	m_scenesPathList.clear();//��ʼɨ��ǰ�����������

	QDir dir(path);
	if (!dir.exists())
		return false;
	dir.setFilter(QDir::Dirs | QDir::Files);//����Ŀ¼���ļ��������Ĺ��˵�
	dir.setSorting(QDir::DirsFirst);//������ʾĿ¼
	QFileInfoList list = dir.entryInfoList();//��ȡ�ļ���Ϣ�б�
	int i = 0;
	bool bIsDir;
	do 
	{
		QFileInfo fileInfo = list.at(i);
		if (fileInfo.fileName() == "." | fileInfo.fileName() == "..")
		{
			i++;
			continue;
		}
		bIsDir = fileInfo.isDir();
		if (bIsDir)
		{
			//FindFile(fileInfo.filePath());
			continue;
		}
		else
		{//���ļ�
			if (QString("sc") == fileInfo.completeSuffix())
			{//�ǳ����ļ������泡���ļ�·��
				m_scenesPathList.push_back(fileInfo.absoluteFilePath());
			}
		}
		i++;
	} while (i<list.size());
	return true;
}