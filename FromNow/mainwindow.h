#pragma once

#include <QMainWindow>
#include <QFile>
#include <QDir>
#include "fn_widgets.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent=nullptr);
	~MainWindow();
protected:
	FromNow::CreateBar *createBar;
	QDir dataPath;
	QFile *dataFile;
	FromNow::ContentView *viewport;
	void RefreshEvents();
};

