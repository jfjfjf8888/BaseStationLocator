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
	//扫描程序目录Scenes目录，找出所有场景文件
#ifdef _DEBUG
	QString scanPath = "D:/Project/Evaluator/Scenes";
#else //Release
	QString scanPath = qApp->applicationDirPath() + "/Scenes";
#endif
	scanScenesFile(scanPath);
	ui.listWidget->clear();
	//分离场景文件路径与文件名并将文件名显示到界面上
	parseFilePathAndShow();
	return QDialog::exec();
}

void selectScense::treeDoubleClicked(QModelIndex index)
{
	//int scences = index.data().toString() == QString(u8"圆形场景") ?  0 : 1;
	//发射信号，告诉信号接收类用户选择的场景
	QString ss = index.data(Qt::WhatsThisRole).toString();
	emit(signal_scenseSelected(index.data(Qt::WhatsThisRole).toString()));
	//关闭场景选择界面
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
	m_scenesPathList.clear();//开始扫描前先情况缓冲区

	QDir dir(path);
	if (!dir.exists())
		return false;
	dir.setFilter(QDir::Dirs | QDir::Files);//除了目录或文件，其他的过滤掉
	dir.setSorting(QDir::DirsFirst);//优先显示目录
	QFileInfoList list = dir.entryInfoList();//获取文件信息列表
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
		{//是文件
			if (QString("sc") == fileInfo.completeSuffix())
			{//是场景文件，保存场景文件路径
				m_scenesPathList.push_back(fileInfo.absoluteFilePath());
			}
		}
		i++;
	} while (i<list.size());
	return true;
}