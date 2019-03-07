#ifndef BSL_H
#define BSL_H

#include <QtWidgets/QMainWindow>
#include "ui_bsl.h"
#include "selectscense.h"

class BSL : public QMainWindow
{
	Q_OBJECT

public:
	BSL(QWidget *parent = 0);
	~BSL();
	void init();
private slots:
	//���س�����ť
	void on_loadSencesToolButton_clicked();

	//���ð�ť
	void on_resetToolButton_clicked();

	//�����ļ���ť
	void on_loadFileToolButton_clicked();

	//���а�ť
	void on_runToolButton_clicked();
public slots:
	void slots_scenseSelected(QString);

private:
	Ui::BSLClass ui;
	selectScense m_selectScense;//ѡ�񳡾�����

	//������Ϣ
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

	//�����ļ�·��
	QString m_testPointFilePath;
};

#endif // BSL_H

