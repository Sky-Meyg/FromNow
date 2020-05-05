#pragma once

#include <QToolBar>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QDate>
#include <QCalendarWidget>
#include <QScrollArea>
#include <QPushButton>
#include <QRadioButton>
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

	class LIBFROMNOWSHARED_EXPORT UnitBlock : public QWidget
	{
	public:
		UnitBlock(QWidget *parent=nullptr);
	protected:
		QRadioButton *days;
		QRadioButton *months;
		QRadioButton *years;
	};

	class LIBFROMNOWSHARED_EXPORT SecretEdit : public QWidget
	{
		Q_OBJECT
	public:
		SecretEdit(QString initialText,QWidget *parent=nullptr);
		QString Text() const { return edit->text(); }
	protected:
		QPushButton *label;
		QLineEdit *edit;
	signals:
		void Edited(const QString label);
	};

	class LIBFROMNOWSHARED_EXPORT DetailsBlock : public QWidget
	{
		Q_OBJECT
	public:
		DetailsBlock(const Event &event,QWidget *parent=nullptr);
	protected:
		QLabel *count;
		SecretEdit *label;
	signals:
		void Edit(Event event);
	};
}

