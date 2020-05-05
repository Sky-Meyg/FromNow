#pragma once

#include <QString>
#include <map>

namespace FromNow
{
	const QString JSON_KEY_DATE="Date";
	const QString JSON_KEY_LABEL="Label";
	const QString JSON_KEY_UNIT="Unit";

	const QString JSON_UNIT_DAYS="days";
	const QString JSON_UNIT_MONTHS="months";
	const QString JSON_UNIT_YEARS="years";

	const QString DATE_FORMAT="yyyy/MM/dd";

	const QString DATA_FILE_NAME="fromnow.json";

	enum class Units
	{
		DAYS,
		MONTHS,
		YEARS
	};
	typedef std::map<QString,Units> UnitMap;
	const UnitMap unitMap={
		{JSON_UNIT_DAYS,Units::DAYS},
		{JSON_UNIT_MONTHS,Units::MONTHS},
		{JSON_UNIT_YEARS,Units::YEARS}
	};
}
