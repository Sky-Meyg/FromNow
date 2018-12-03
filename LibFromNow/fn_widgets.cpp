#include <QBoxLayout>
#include "fn_widgets.h"

namespace FromNow
{
	CreateBar::CreateBar(QWidget *parent) : QToolBar(parent)
	{
		addWidget(new QLabel("Label",this));
		label=new QLineEdit(this);
		addWidget(label);
	}

	EventBlock::EventBlock(QDate date,QString label,QWidget *parent) : QWidget(parent)
	{
		setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed));
		setLayout(new QHBoxLayout(this));
		layout()->addWidget(new DateBlock(date,this));
		layout()->addWidget(new QLabel(label,this));
	}

	DateBlock::DateBlock(QDate date,QWidget *parent) : QWidget(parent)
	{
		setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
		setLayout(new QVBoxLayout(this));
		month=new QLabel(QDate::shortMonthName(date.month()),this);
		month->setStyleSheet("font-size: 14pt");
		month->setAlignment(Qt::AlignCenter);
		layout()->addWidget(month);
		day=new QLabel(QString::number(date.day()),this);
		day->setStyleSheet("font-size: 18pt");
		day->setAlignment(Qt::AlignCenter);
		layout()->addWidget(day);
		year=new QLabel(QString::number(date.year()),this);
		year->setStyleSheet("font-size: 12pt");
		year->setAlignment(Qt::AlignCenter);
		layout()->addWidget(year);
	}
}
