#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setOrganizationName("Sky-Meyg");
	a.setApplicationName("FromNow");
	MainWindow w;
	w.show();

	return a.exec();
}
