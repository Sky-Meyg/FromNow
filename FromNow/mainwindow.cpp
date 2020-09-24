#include <QStandardPaths>
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

MainWindow::MainWindow(QWidget *parent)	: QMainWindow(parent), viewport(nullptr)
{
	connect(this,&MainWindow::Exit,qApp,&QCoreApplication::exit,Qt::QueuedConnection);

	if (!FromNow::Event::Open(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)))
	{
		(new QMessageBox(QMessageBox::Critical,ERROR_CRITICAL,FromNow::Event::LastError(),QMessageBox::Ok,this))->exec();
		emit Exit(FILE_ERROR);
	}

	if (!FromNow::Event::ReadAll())
	{
		(new QMessageBox(QMessageBox::Critical,ERROR_CRITICAL,FromNow::Event::LastError(),QMessageBox::Ok,this))->exec();
		emit Exit(PARSE_ERROR);
	}

	RefreshEvents();

	createBar=new FromNow::CreateBar(this);
	connect(createBar,&FromNow::CreateBar::Add,this,&MainWindow::EventAdded);
	addToolBar(createBar);
}

MainWindow::~MainWindow()
{
	FromNow::Event::Close();
}

void MainWindow::RefreshEvents()
{
	try {
		viewport=new FromNow::ContentView(this); // not sure if this is a memory leak or not in Qt parent-owner-deleteLater-craziness framework
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
	FromNow::Event::Write();
	RefreshEvents();
}

void MainWindow::EventRemoved(FromNow::Event event)
{
	FromNow::Event::Remove(event);
	FromNow::Event::Write();
	RefreshEvents();
}
