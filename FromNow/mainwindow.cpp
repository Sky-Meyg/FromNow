#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDate>
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QMessageBox>
#include "mainwindow.h"
#include "types.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)	: QMainWindow(parent), viewport(nullptr), eventFile(nullptr)
{
	connect(this,&MainWindow::Exit,qApp,&QCoreApplication::exit,Qt::QueuedConnection);

	createBar=new FromNow::CreateBar(this);
	connect(createBar,&FromNow::CreateBar::Add,this,&MainWindow::EventAdded);
	addToolBar(createBar);
}

MainWindow::~MainWindow()
{
	if (eventFile) delete eventFile;
}

bool MainWindow::event(QEvent *event)
{
	if (event->type() == QEvent::Polish)
	{
		try
		{
			eventFile=new FromNow::EventFile();
			RefreshEvents();
		}

		catch (std::runtime_error &exception)
		{
			(new QMessageBox(QMessageBox::Critical,ERROR_CRITICAL,eventFile->LastError(),QMessageBox::Ok,this))->exec();
			return false;
		}
	}

	return QWidget::event(event);
}

void MainWindow::RefreshEvents()
{
	try {
		viewport=new FromNow::ContentView(eventFile->Events(),this); // not sure if this is a memory leak or not in Qt parent-owner-deleteLater-craziness framework
		connect(viewport,&FromNow::ContentView::Remove,this,&MainWindow::EventRemoved);
		setCentralWidget(viewport);
	}

	catch (std::logic_error &exception) {
		(new QMessageBox(QMessageBox::Critical,ERROR_CRITICAL,exception.what(),QMessageBox::Ok,this))->exec();
		emit Exit(BUG);
	}

	catch (std::runtime_error &exception) {
		(new QMessageBox(QMessageBox::Critical,ERROR_GENERAL,exception.what(),QMessageBox::Ok,this))->exec();
		emit Exit(GENERAL_ERROR);
	}
}

void MainWindow::EventAdded(FromNow::Event event)
{
	eventFile->Add(event);
	RefreshEvents();
}

void MainWindow::EventRemoved(const std::unique_ptr<FromNow::Event> &event)
{
	eventFile->Remove(event);
	RefreshEvents();
}
