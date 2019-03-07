#ifndef BSL_H
#define BSL_H

#include <QtWidgets/QMainWindow>
#include "ui_bsl.h"

class BSL : public QMainWindow
{
	Q_OBJECT

public:
	BSL(QWidget *parent = 0);
	~BSL();
private slots:
	//加载场景按钮
	void on_loadSencesToolButton_clicked();

	//重置按钮
	void on_resetToolButton_clicked();

	//加载文件按钮
	void on_loadFileToolButton_clicked();

	//运行按钮
	void on_runToolButton_clicked();

private:
	Ui::BSLClass ui;
};

#endif // BSL_H

