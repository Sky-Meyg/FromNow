#pragma once

#include <QMainWindow>
#include <QFile>
#include <QDir>
#include <QScrollArea>
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
	QScrollArea *viewport;
	QWidget *content;
	void ReadEvents();
	void AddEvent(QDate date,QString label);
};

