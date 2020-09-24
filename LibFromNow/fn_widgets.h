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
		ContentView(EventList &events,QWidget *parent=nullptr);
	protected:
		QFrame *content;
	signals:
		void Remove(const std::unique_ptr<Event> &event);
	};

	class LIBFROMNOWSHARED_EXPORT EventBlock : public QWidget
	{
		Q_OBJECT
	public:
		EventBlock(std::unique_ptr<Event> &event,QWidget *parent=nullptr);
	protected:
		QPushButton *remove;
	signals:
		void Remove(const std::unique_ptr<Event> &event);
	};

	class LIBFROMNOWSHARED_EXPORT DateBlock : public QWidget
	{
	public:
		DateBlock(const std::unique_ptr<Event> &event,QWidget *parent=nullptr);
	protected:
		QLabel *month;
		QLabel *day;
		QLabel *year;
	};

	class LIBFROMNOWSHARED_EXPORT UnitBlock : public QWidget
	{
		Q_OBJECT
	public:
		UnitBlock(std::unique_ptr<Event> &event,QWidget *parent=nullptr);
	protected:
		QRadioButton *days;
		QRadioButton *months;
		QRadioButton *years;
	signals:
		void Changed(Units unit);
	};

	/*!
	 * \class ExplicitPushButton
	 * \brief QPushButton with a label that doesn't interpret ampersands
	 */
	class LIBFROMNOWSHARED_EXPORT ExplicitPushButton : public QPushButton
	{
	public:
		ExplicitPushButton(const QString &text,QWidget *parent=nullptr) : QPushButton(stripShortcut(text),parent) { }
		void setText(const QString &text) { QPushButton::setText(stripShortcut(text)); }
	protected:
		QString stripShortcut(const QString &text) { return QString(text).replace("&","&&"); }
	};

	/*!
	 * \class SecretEdit
	 * \brief QPushButton that becomes a QLineEdit when clicked, allowing you to change the text
	 */
	class LIBFROMNOWSHARED_EXPORT SecretEdit : public QWidget
	{
		Q_OBJECT
	public:
		SecretEdit(const QString &initialText,QWidget *parent=nullptr);
		QString Text() const { return edit->text(); }
	protected:
		ExplicitPushButton *label;
		QLineEdit *edit;
	signals:
		void Edited(const QString label);
	};

	class LIBFROMNOWSHARED_EXPORT DetailsBlock : public QWidget
	{
		Q_OBJECT
	public:
		DetailsBlock(std::unique_ptr<Event> &event,QWidget *parent=nullptr);
		const QString DescribeRange(Units unit) const;
	protected:
		QLabel *count;
		SecretEdit *label;
		std::map<Units,QString> descriptions;
		const QString DaysDescription(const std::unique_ptr<Event> &event) const;
		const QString MonthsDescription(const std::unique_ptr<Event> &event) const;
		const QString YearsDescription(const std::unique_ptr<Event> &event) const;
		void PrepareDescriptions(const std::unique_ptr<Event> &event);
	signals:
		void Edit(Event event);
	public slots:
		void UnitChanged(Units unit);
	};
}

