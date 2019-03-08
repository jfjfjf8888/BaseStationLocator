#ifndef BSL_H
#define BSL_H

#include <QtWidgets/QMainWindow>
#include "ui_bsl.h"
#include "selectscense.h"
#include "Analysiser.h"
#include "setting.h"

class BSL : public QMainWindow
{
	Q_OBJECT

public:
	BSL(QWidget *parent = 0);
	~BSL();
	void init();
private slots:
	//加载场景按钮
	void on_loadSencesToolButton_clicked();

	//重置按钮
	void on_resetToolButton_clicked();

	//加载文件按钮
	void on_loadFileToolButton_clicked();

	//设置按钮
	void on_settingToolButton_clicked();

	//运行按钮
	void on_runToolButton_clicked();
public slots:
	void slots_scenseSelected(QString);
signals:
	void plsInitCommunicationStandardComboBox(InputDataAnalysiser * pAnalysiser);
private:
	Ui::BSLClass ui;
	selectScense m_selectScense;//选择场景对象
	setting m_setting;//设置窗口

	//场景信息
	QString m_scenceName;
	QString m_picPath;
	int m_scenceType;
	float m_lenth;
	float m_width;
	float m_r;
	float m_proportion;
	QString m_screenshot;
	int m_nTestPointCount;
	QString m_pointList;

	//测试文件路径
	QStringList m_testPointFilePathList;

	//测试点文件列表，因需支持多文件，故需要列表
	QVector<QByteArray> m_fileContextList;

	InputDataAnalysiser * m_pAnalysiser;
};

#endif // BSL_H

