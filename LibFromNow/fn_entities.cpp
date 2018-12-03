#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonArray>
#include <QRandomGenerator>
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
		id=QRandomGenerator::global()->generate();
		date=QDate::fromString(data.value(JSON_KEY_DATE).toString(),DATE_FORMAT);
		label=data.value(JSON_KEY_LABEL).toString();
	}

	qint64 Event::Days() const
	{
		return date.daysTo(QDate::currentDate());
	}

	QString Event::Detail() const
	{
		if (Days() < 0)
			return QString("%1 days until").arg(QString::number(Days() * -1));
		else
			return QString("%1 days since").arg(QString::number(Days()));
	}

	void Event::Edit(quint32 id,const QString &label)
	{
		EventIterator candidate=std::find_if(events.begin(),events.end(),[&id](Event &candidate) { return candidate.ID() == id; });
		if (candidate != events.end()) candidate->label=label;
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
			error=QString("Failed to create data storage path: %1").arg(dataPath.absolutePath());
			cli << error;
			return false;
		}

		dataFile.setFileName(dataPath.filePath(DATA_FILE_NAME));
		if (!dataFile.open(QIODevice::ReadWrite))
		{
			error=QString("Failed to open data storage file: %1").arg(dataFile.fileName());
			cli << error;
			return false;
		}

		return true;
	}

	void Event::Close()
	{
		dataFile.close();
	}

	bool Event::Read()
	{
		QJsonParseError jsonError;
		QJsonDocument json=QJsonDocument::fromJson(dataFile.readAll(),&jsonError);
		if (json.isNull())
		{
			error=QString("Failed to extract JSON from data file: %1").arg(jsonError.errorString());
			cli << error;
			return false;
		}

		for (const QJsonValue object : json.array()) FromNow::Event::Add(FromNow::Event(object.toObject()));
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
			objects.append(object);
		}
		json.setArray(objects);
		dataFile.seek(0);
		dataFile.resize(0);
		if (dataFile.write(json.toJson(QJsonDocument::Indented)) < 0)
		{
			error=QString("Failed to write JSON to data file");
			cli << error;
			return false;
		}
		return true;
	}
}
