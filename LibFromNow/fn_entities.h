#pragma once

#include <QJsonObject>
#include <QDate>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include <vector>
#include "libfromnow_global.h"

namespace FromNow
{
	class LIBFROMNOWSHARED_EXPORT Event;
	typedef std::vector<Event> EventList;
	typedef std::vector<Event>::const_iterator EventIterator;

	class LIBFROMNOWSHARED_EXPORT Event
	{
	public:
		Event(QDate date,QString label) : date(date), label(label) { }
		Event(QJsonObject data);
		quint32 ID() const { return id; }
		const QDate& Date() const { return date; }
		const QString& Label() const { return label; }
		qint64 Days() const;
		QString Detail() const;
		static void Add(Event event) { events.push_back(event); }
		static void Remove(Event &event);
		static const EventList& Events() { return events; }
		static bool Open(QString location);
		static void Close();
		static bool Read();
		static bool Write();
		static const QString& LastError() { return error; }
	protected:
		quint32 id;
		QDate date;
		QString label;
		static EventList events;
		static QDir dataPath;
		static QFile dataFile;
		static QTextStream cli;
		static QString error;
	};
}

