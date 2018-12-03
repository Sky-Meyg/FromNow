#pragma once

#include <QToolBar>
#include <QLabel>
#include <QLineEdit>
#include <QDate>
#include <QCalendarWidget>
#include <QScrollArea>
#include "fn_entities.h"
#include "libfromnow_global.h"

namespace FromNow
{
	class LIBFROMNOWSHARED_EXPORT CreateBar : public QToolBar
	{
	public:
		CreateBar(QWidget *parent=nullptr);
	protected:
		QCalendarWidget *calendar;
		QLineEdit *label;
	};

	class LIBFROMNOWSHARED_EXPORT ContentView : public QScrollArea
	{
	public:
		ContentView(QWidget *parent=nullptr);
	protected:
		QWidget *content;
	};

	class LIBFROMNOWSHARED_EXPORT EventBlock : public QWidget
	{
	public:
		EventBlock(const Event &event,QWidget *parent=nullptr);
	};

	class LIBFROMNOWSHARED_EXPORT DateBlock : public QWidget
	{
	public:
		DateBlock(QDate date,QWidget *parent=nullptr);
	protected:
		QLabel *month;
		QLabel *day;
		QLabel *year;
	};

	class LIBFROMNOWSHARED_EXPORT DetailsBlock : public QWidget
	{

	};
}

