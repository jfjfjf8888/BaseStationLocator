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
	//���س�����ť
	void on_loadSencesToolButton_clicked();

	//���ð�ť
	void on_resetToolButton_clicked();

	//�����ļ���ť
	void on_loadFileToolButton_clicked();

	//���а�ť
	void on_runToolButton_clicked();

private:
	Ui::BSLClass ui;
};

#endif // BSL_H

