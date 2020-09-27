#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDate>
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QMessageBox>
#include <stdexcept>
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
		viewport=std::unique_ptr<FromNow::ContentView,std::function<void(FromNow::ContentView*)>>(new FromNow::ContentView(eventFile->Events(),this),[](FromNow::ContentView *ptr) { ptr->deleteLater(); });
		connect(viewport.get(),&FromNow::ContentView::Remove,this,&MainWindow::EventRemoved);
		setCentralWidget(viewport.get());
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
