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
	EventList Event::events;
	QTextStream Event::cli(stderr);
	QDir Event::dataPath;
	QFile Event::dataFile;
	QString Event::error;

	Event::Event(QJsonObject data)
	{
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
		id=QRandomGenerator::global()->generate();
#else
		id=qrand();
#endif
		date=QDate::fromString(data.value(JSON_KEY_DATE).toString(),DATE_FORMAT);
		label=data.value(JSON_KEY_LABEL).toString();
		unit=unitMap.at(data.value(JSON_KEY_UNIT).toString());
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

	void Event::Edit(quint32 id,const QString &label)
	{
		EventIterator candidate=std::find_if(events.begin(),events.end(),[&id](Event &candidate) { return candidate.ID() == id; });
		if (candidate != events.end()) candidate->label=label;
	}

	void Event::Edit(quint32 id,const Units unit)
	{
		EventIterator candidate=std::find_if(events.begin(),events.end(),[&id](Event &candidate) { return candidate.ID() == id; });
		if (candidate != events.end()) candidate->unit=unit;
	}

	void Event::Remove(Event &event)
	{
		EventIteratorConst candidate=std::find_if(events.cbegin(),events.cend(),[&event](const Event &candidate) { return candidate.ID() == event.ID(); });
		if (candidate != events.end()) events.erase(candidate);
	}

	bool Event::Open(QString location)
	{
		dataPath.setPath(location);
		if (!dataPath.mkpath(dataPath.absolutePath()))
		{
			OutputError(QString("Failed to create data storage path: %1").arg(dataPath.absolutePath()));
			return false;
		}

		dataFile.setFileName(dataPath.filePath(DATA_FILE_NAME));
		if (!dataFile.open(QIODevice::ReadWrite))
		{
			OutputError(QString("Failed to open data storage file: %1").arg(dataFile.fileName()));
			return false;
		}

		return true;
	}

	void Event::Close()
	{
		dataFile.close();
	}

	bool Event::ReadAll()
	{
		static const QString failedText("Failed to extract JSON from data file: %1");

		try
		{
			QJsonParseError jsonError;
			QByteArray data=dataFile.readAll();
			if (data.isEmpty()) data="[]";
			QJsonDocument json=QJsonDocument::fromJson(data,&jsonError);
			if (json.isNull()) throw std::runtime_error(jsonError.errorString().toStdString());
			for (const QJsonValue object : json.array()) FromNow::Event::Add(FromNow::Event(object.toObject()));
		}

		catch (std::runtime_error &exception)
		{
			OutputError(failedText.arg(exception.what()));
			return false;
		}

		catch (std::out_of_range &exception)
		{
			OutputError(failedText.arg(exception.what()));
			return false;
		}

		return true;
	}

	bool Event::Write()
	{
		QJsonDocument json;
		QJsonArray objects;
		for (const Event& event : Events())
		{
			QJsonObject object;
			object.insert(JSON_KEY_DATE,event.Date().toString(DATE_FORMAT));
			object.insert(JSON_KEY_LABEL,event.Label());
			switch (event.Unit())
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

	void Event::OutputError(const QString &message)
	{
		error=message;
		cli << error;
	}

	const EventList& Event::Events()
	{
		std::sort(events.begin(),events.end(),[](const Event &left,const Event &right) { return left.AbsoluteCount(left.Days()) < right.AbsoluteCount(right.Days()); });
		return events;
	}
}
