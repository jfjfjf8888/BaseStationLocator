#include "setting.h"

setting::setting()
{
	ui.setupUi(this);
	//���ù̶�����
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
	{//ͨ����ʽ
		QString comboBoxText = standardList[nIndex].name + " "
			+ standardList[nIndex].standard;// + " " + standardList[nIndex].frequency;
		ui.communicationStandardComboBox->addItem(comboBoxText);
	}
}