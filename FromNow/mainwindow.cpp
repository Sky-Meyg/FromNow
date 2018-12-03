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

	if (!FromNow::Event::Read())
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
	viewport=new FromNow::ContentView(this); // not sure if this is a memory leak or not in Qt parent-owner-deleteLater-craziness framework
	setCentralWidget(viewport);
}

void MainWindow::EventAdded(FromNow::Event event)
{
	FromNow::Event::Write();
	RefreshEvents();
}
