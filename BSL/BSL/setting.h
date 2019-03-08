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
signals:
	void dataInputed(QString standard, int index, float stepLenth);
public slots:
	void initCommunicationStandardComboBox(InputDataAnalysiser * pAnalysiser);
    
	//okButton
	void on_okButton_clicked();

	//cancelButton
	inline void on_cancelButton_clicked() { this->close(); }
private:
	Ui::settingDialog ui;
};

