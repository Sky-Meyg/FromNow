#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
	qsrand(static_cast<unsigned int>(QTime::currentTime().msecsSinceStartOfDay()));
#endif

	QApplication application(argc, argv);
	application.setOrganizationName("Sky-Meyg");
	application.setApplicationName("FromNow");
	MainWindow window;
	window.show();

	return application.exec();
}
