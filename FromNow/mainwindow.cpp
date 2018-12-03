#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDate>
#include <QVBoxLayout>
#include "fn_entities.h"
#include "mainwindow.h"
#include "types.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)	: QMainWindow(parent), dataFile(nullptr), viewport(nullptr)
{
	dataPath=QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	//if (!QDir::exists(dataPath)) QDir::mkdir(dataPath);
	dataFile=new QFile(dataPath.filePath("fromnow.json"));
	dataFile->open(QIODevice::ReadWrite);
	RefreshEvents();

	addToolBar(new FromNow::CreateBar(this));

}

MainWindow::~MainWindow()
{
	delete dataFile;
}

void MainWindow::RefreshEvents()
{
	QJsonDocument json=QJsonDocument::fromJson(dataFile->readAll());
	for (const QJsonValue object : json.array()) FromNow::Event::Add(FromNow::Event(object.toObject()));
	viewport=new FromNow::ContentView(this);
	setCentralWidget(viewport);
}
