#pragma once

#include <QStandardPaths>
#include <QJsonObject>
#include <QDate>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include <vector>
#include <memory>
#include "fn_types.h"
#include "libfromnow_global.h"

namespace FromNow
{
	class LIBFROMNOWSHARED_EXPORT Event
	{
	public:
		Event(QDate date,QString label,Units unit);
		quint32 ID() const { return id; }
		const QDate& Date() const { return date; }
		const QString& Label() const { return label; }
		void Label(const QString &label) { this->label=label; }
		Units Unit() const { return unit; }
		void Unit(const Units unit) { this->unit=unit; }
		qint64 Days() const;
		qint64 Months() const;
		qint64 Years() const;
		quint64 AbsoluteCount(qint64 total) const;
	protected:
		quint32 id;
		QDate date;
		QString label;
		Units unit;
	};

	using EventList=std::vector<std::unique_ptr<Event>>;
	using EventIterator=std::vector<Event>::iterator;
	using EventIteratorConst=std::vector<Event>::const_iterator;

	class LIBFROMNOWSHARED_EXPORT EventFile
	{
	public:
		EventFile(QString directory=QStandardPaths::writableLocation(QStandardPaths::AppDataLocation),QString filename=DATA_FILE_NAME);
		~EventFile();
		void Add(const Event &event) { events.push_back(std::make_unique<Event>(event)); }
		void Remove(const std::unique_ptr<Event> &event);
		EventList& Events();
		bool Open(QString location);
		bool Write();
		const QString& LastError() { return error; }
	protected:
		EventList events;
		QDir dataPath;
		QFile dataFile;
		QTextStream cli;
		QString error;
		void OutputError(const QString &message);
	};
}

