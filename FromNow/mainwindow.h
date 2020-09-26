#pragma once

#include <QMainWindow>
#include <QFile>
#include <QDir>
#include <memory>
#include <functional>
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
	std::unique_ptr<FromNow::ContentView,std::function<void(FromNow::ContentView*)>> viewport;
	FromNow::EventFile *eventFile;
	bool event(QEvent *event) override;
	void RefreshEvents();
signals:
	void Exit(int code);
protected slots:
	void EventAdded(FromNow::Event event);
	void EventRemoved(const std::unique_ptr<FromNow::Event> &event);
};

