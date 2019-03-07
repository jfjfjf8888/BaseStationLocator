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

private:
	Ui::BSLClass ui;
};

#endif // BSL_H
