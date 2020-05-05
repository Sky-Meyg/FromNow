#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	application.setOrganizationName("Sky-Meyg");
	application.setApplicationName("FromNow");
	MainWindow window;
	window.show();

	return application.exec();
}
