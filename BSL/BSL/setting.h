#pragma once
#include <QDialog>
#include "ui_setting.h"
#include "Analysiser.h"

class setting :
	public QDialog
{
	Q_OBJECT
public:
	setting();
	~setting();

public slots:
	void initCommunicationStandardComboBox(InputDataAnalysiser * pAnalysiser);
private:
	Ui::settingDialog ui;
};

