#pragma once

#include <QString>

enum ExitCodes
{
	OK,
	BUG,
	GENERAL_ERROR,
	FILE_ERROR,
	PARSE_ERROR,
};

const QString ERROR_GENERAL="Something went wrong";
const QString ERROR_CRITICAL="A critical error has occurred";

