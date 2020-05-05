#pragma once

#include <QJsonObject>
#include <QDate>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include <vector>
#include "fn_types.h"
#include "libfromnow_global.h"

namespace FromNow
{
	class LIBFROMNOWSHARED_EXPORT Event;
	typedef std::vector<Event> EventList;
	typedef std::vector<Event>::iterator EventIterator;
	typedef std::vector<Event>::const_iterator EventIteratorConst;

	class LIBFROMNOWSHARED_EXPORT Event
	{
	public:
		Event(QDate date,QString label,Units unit) : date(date), label(label), unit(unit) { }
		Event(QJsonObject data);
		quint32 ID() const { return id; }
		const QDate& Date() const { return date; }
		const QString& Label() const { return label; }
		Units Unit() const { return unit; }
		qint64 Days() const;
		qint64 Months() const;
		qint64 Years() const;
		quint64 AbsoluteCount(qint64 total) const;
		static void Add(Event event) { events.push_back(event); }
		static void Edit(quint32 id,const QString &label);
		static void Edit(quint32 id,const Units unit);
		static void Remove(Event &event);
		static const EventList& Events();
		static bool Open(QString location);
		static void Close();
		static bool Read();
		static bool Write();
		static const QString& LastError() { return error; }
	protected:
		quint32 id;
		QDate date;
		QString label;
		Units unit;
		static EventList events;
		static QDir dataPath;
		static QFile dataFile;
		static QTextStream cli;
		static QString error;
	};
}

