#ifndef SELECTSCENSE_H
#define SELECTSCENSE_H

#include <QDialog>
#include "ui_selectscense.h"

#include "selectscense.h"


#include <QVector>

class selectScense : public QDialog
{
	Q_OBJECT

public:
	selectScense(QWidget *parent = 0);
	~selectScense();
	int exec() override;//÷ÿ‘ÿshow∫Ø ˝
public slots:
	void treeDoubleClicked(QModelIndex);
signals:
	void signal_scenseSelected(QString);
private:
	void parseFilePathAndShow();
	void scanScenesFile(QString Path);
	bool FindFile(const QString & path);
private:
	Ui::selectScenseClass ui;
	QVector<QString> m_scenesPathList;
};

#endif // SELECTSCENSE_H
