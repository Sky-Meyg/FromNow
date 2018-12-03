#pragma once

#include <QToolBar>
#include <QLabel>
#include <QTextEdit>
#include <QDate>
#include <QCalendarWidget>
#include <QScrollArea>
#include <QPushButton>
#include "fn_entities.h"
#include "libfromnow_global.h"

namespace FromNow
{
	class LIBFROMNOWSHARED_EXPORT CreateBar : public QToolBar
	{
		Q_OBJECT
	public:
		CreateBar(QWidget *parent=nullptr);
	protected:
		QCalendarWidget *calendar;
		QTextEdit *label;
		QPushButton *addEvent;
	signals:
		void Add(Event event);
	protected slots:
		void AddClicked();
		void TextChanged();
	};

	class LIBFROMNOWSHARED_EXPORT ContentView : public QScrollArea
	{
		Q_OBJECT
	public:
		ContentView(QWidget *parent=nullptr);
	protected:
		QFrame *content;
	signals:
		void Remove(Event event);
	};

	class LIBFROMNOWSHARED_EXPORT EventBlock : public QWidget
	{
		Q_OBJECT
	public:
		EventBlock(const Event &event,QWidget *parent=nullptr);
	protected:
		QPushButton *remove;
	signals:
		void Remove(Event event);
	};

	class LIBFROMNOWSHARED_EXPORT DateBlock : public QWidget
	{
	public:
		DateBlock(const Event &event,QWidget *parent=nullptr);
	protected:
		QLabel *month;
		QLabel *day;
		QLabel *year;
	};

	class LIBFROMNOWSHARED_EXPORT DetailsBlock : public QWidget
	{
	public:
		DetailsBlock(const Event &event,QWidget *parent=nullptr);
	protected:
		QLabel *count;
		QLabel *label;
	};
}

