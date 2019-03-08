#include "setting.h"

setting::setting()
{
	ui.setupUi(this);
	//设置固定窗体
	this->setFixedSize(this->width(), this->height());
}


setting::~setting()
{
}

void setting::initCommunicationStandardComboBox(InputDataAnalysiser  * pAnalysiser)
{
	if (!pAnalysiser)
		return;

	ui.communicationStandardComboBox->clear();
	QVector<SStandard> standardList = pAnalysiser->getStandardList();
	for (int nIndex = 0; nIndex < pAnalysiser->getStandardCount(); ++nIndex)
	{//通信制式
		QString comboBoxText = standardList[nIndex].name + " "
			+ standardList[nIndex].standard;// + " " + standardList[nIndex].frequency;
		ui.communicationStandardComboBox->addItem(comboBoxText);
	}
}

void setting::on_okButton_clicked()
{
	emit(dataInputed(ui.communicationStandardComboBox->currentText(), 
		             ui.communicationStandardComboBox->currentIndex(), 
		             ui.predictiveStepSizelineEdit->text().toFloat()));
	this->close();
}