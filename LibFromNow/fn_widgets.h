#pragma once

#include <QToolBar>
#include <QLabel>
#include <QLineEdit>
#include <QDate>
#include "libfromnow_global.h"

namespace FromNow
{
	class LIBFROMNOWSHARED_EXPORT CreateBar : public QToolBar
	{
	public:
		CreateBar(QWidget *parent=nullptr);
	protected:
		QLineEdit *label;
	};

	class LIBFROMNOWSHARED_EXPORT EventBlock : public QWidget
	{
	public:
		EventBlock(QDate date,QString label,QWidget *parent=nullptr);
	};
}

