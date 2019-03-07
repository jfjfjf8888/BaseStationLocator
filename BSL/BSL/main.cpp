#include "bsl.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BSL w;
	w.show();
	return a.exec();
}
