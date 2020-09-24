#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonArray>

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
#include <QRandomGenerator>
#endif

#include "fn_entities.h"
#include "fn_types.h"

namespace FromNow
{
	EventFile::EventFile(QString directory,QString filename) : cli(stderr)
	{
		dataPath.setPath(directory);
		if (!dataPath.mkpath(dataPath.absolutePath())) throw std::runtime_error(QString("Failed to create data storage path: %1").arg(dataPath.absolutePath()).toStdString());

		dataFile.setFileName(dataPath.filePath(filename));
		if (!dataFile.open(QIODevice::ReadWrite)) throw std::runtime_error(QString("Failed to open data storage file: %1").arg(dataFile.fileName()).toStdString());

		//static const QString failedText("Failed to extract JSON from data file: %1");

		QJsonParseError jsonError;
		QByteArray data=dataFile.readAll();
		if (data.isEmpty()) data="[]";
		QJsonDocument json=QJsonDocument::fromJson(data,&jsonError);
		if (json.isNull()) throw std::runtime_error(jsonError.errorString().toStdString());
		for (const QJsonValue jsonValue : json.array())
		{
			QJsonObject jsonObject=jsonValue.toObject();
			events.push_back(std::make_unique<Event>(
				QDate::fromString(jsonObject.value(JSON_KEY_DATE).toString(),DATE_FORMAT),
				jsonObject.value(JSON_KEY_LABEL).toString(),
				unitMap.at(jsonObject.value(JSON_KEY_UNIT).toString())
			));
		}
	}

	EventFile::~EventFile()
	{
		Write();
		dataFile.close();
	}

	bool EventFile::Write()
	{
		QJsonDocument json;
		QJsonArray objects;
		for (const std::unique_ptr<Event> &event : Events())
		{
			QJsonObject object;
			object.insert(JSON_KEY_DATE,event->Date().toString(DATE_FORMAT));
			object.insert(JSON_KEY_LABEL,event->Label());
			switch (event->Unit())
			{
			case Units::DAYS:
				object.insert(JSON_KEY_UNIT,JSON_UNIT_DAYS);
				break;
			case Units::MONTHS:
				object.insert(JSON_KEY_UNIT,JSON_UNIT_MONTHS);
				break;
			case Units::YEARS:
				object.insert(JSON_KEY_UNIT,JSON_UNIT_YEARS);
				break;
			default:
				throw std::logic_error("Unsupported units encountered saving unit block");
			}
			objects.append(object);
		}
		json.setArray(objects);
		dataFile.seek(0);
		dataFile.resize(0);
		if (dataFile.write(json.toJson(QJsonDocument::Indented)) < 0)
		{
			OutputError("Failed to write JSON to data file");
			return false;
		}
		return true;
	}

	void EventFile::Remove(const std::unique_ptr<Event> &event)
	{
		events.erase(std::remove_if(events.begin(),events.end(),[&event](const std::unique_ptr<Event> &candidate) { return candidate->ID() == event->ID(); }));
	}

	EventList& EventFile::Events()
	{
		std::sort(events.begin(),events.end(),[](const std::unique_ptr<Event> &left,const std::unique_ptr<Event> &right) { return left->AbsoluteCount(left->Days()) < right->AbsoluteCount(right->Days()); });
		return events;
	}

	Event::Event(QDate date,QString label,Units unit) : date(date), label(label), unit(unit)
	{
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
		id=QRandomGenerator::global()->generate();
#else
		id=qrand();
#endif
	}

	void EventFile::OutputError(const QString &message)
	{
		error=message;
		cli << error;
	}

	qint64 Event::Days() const
	{
		return QDate::currentDate().daysTo(date);
	}

	qint64 Event::Months() const
	{
		qint64 months=0;
		QDate today=QDate::currentDate();
		if (today.daysTo(date) > 0)
		{
			while (date.month() != today.month() || date.year() != today.year())
			{
				today=today.addMonths(1);
				months++;
			}
		}
		else
		{
			while (date.month() != today.month() || date.year() != today.year())
			{
				today=today.addMonths(-1);
				months--;
			}
		}
		return months;
	}

	qint64 Event::Years() const
	{
		qint64 years=0;
		QDate today=QDate::currentDate();
		if (today.daysTo(date) > 0)
		{
			today=today.addYears(1);
			while (today < date)
			{
				today=today.addYears(1);
				years++;
			}
		}
		else
		{
			today=today.addYears(-1);
			while (today > date)
			{
				today=today.addYears(-1);
				years--;
			}
		}
		return years;
	}

	quint64 Event::AbsoluteCount(qint64 total) const
	{
		if (total < 0)
			return static_cast<quint64>(total * -1);
		else
			return static_cast<quint64>(total);
	}
}
