#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDate>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "types.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)	: QMainWindow(parent), dataFile(nullptr), content(nullptr)
{
	content=new QWidget(this);
	content->setLayout(new QVBoxLayout(content));
	viewport=new QScrollArea(this);

	dataPath=QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	//if (!QDir::exists(dataPath)) QDir::mkdir(dataPath);
	dataFile=new QFile(dataPath.filePath("fromnow.json"));
	dataFile->open(QIODevice::ReadWrite);
	ReadEvents();

	addToolBar(new FromNow::CreateBar(this));
	viewport->setWidget(content);
	setCentralWidget(viewport);
}

MainWindow::~MainWindow()
{
	delete dataFile;
}

void MainWindow::ReadEvents()
{
	QJsonDocument json=QJsonDocument::fromJson(dataFile->readAll());
	for (const QJsonValue object : json.array())
	{
		AddEvent(QDate::fromString(object.toObject().value(JSON_KEY_DATE).toString(),DATE_FORMAT),object.toObject().value(JSON_KEY_LABEL).toString());
	}
}

void MainWindow::AddEvent(QDate date,QString label)
{
	content->layout()->addWidget(new FromNow::EventBlock(date,label,this));
}
