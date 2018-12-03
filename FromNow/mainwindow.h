#pragma once

#include <QMainWindow>
#include <QFile>
#include <QDir>
#include "fn_entities.h"
#include "fn_widgets.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent=nullptr);
	~MainWindow();
protected:
	FromNow::CreateBar *createBar;
	FromNow::ContentView *viewport;
	void RefreshEvents();
signals:
	void Exit(int code);
protected slots:
	void EventAdded(FromNow::Event event);
};

