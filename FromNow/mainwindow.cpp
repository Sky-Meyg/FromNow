#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDate>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "types.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)	: QMainWindow(parent), dataFile(nullptr), viewport(nullptr)
{
	dataPath=QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	//if (!QDir::exists(dataPath)) QDir::mkdir(dataPath);
	dataFile=new QFile(dataPath.filePath("fromnow.json"));
	dataFile->open(QIODevice::ReadWrite);

	ReadEvents();
	RefreshEvents();

	createBar=new FromNow::CreateBar(this);
	connect(createBar,&FromNow::CreateBar::Add,this,&MainWindow::EventAdded);
	addToolBar(createBar);
}

MainWindow::~MainWindow()
{
	delete dataFile;
}

void MainWindow::ReadEvents()
{
	QJsonDocument json=QJsonDocument::fromJson(dataFile->readAll());
	for (const QJsonValue object : json.array()) FromNow::Event::Add(FromNow::Event(object.toObject()));
}

void MainWindow::RefreshEvents()
{
	viewport=new FromNow::ContentView(this); // not sure if this is a memory leak or not in Qt parent-owner-deleteLater-craziness framework
	setCentralWidget(viewport);
}

void MainWindow::EventAdded(FromNow::Event event)
{
	RefreshEvents();
}
