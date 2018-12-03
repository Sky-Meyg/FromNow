#include <QBoxLayout>
#include "fn_widgets.h"

namespace FromNow
{
	CreateBar::CreateBar(QWidget *parent) : QToolBar(parent)
	{
		calendar=new QCalendarWidget(this);
		calendar->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
		addWidget(calendar);
		addWidget(new QLabel("Label",this));
		label=new QLineEdit(this);
		QSizePolicy labelPolicy=label->sizePolicy();
		labelPolicy.setHorizontalStretch(2);
		label->setSizePolicy(labelPolicy);
		addWidget(label);
	}

	ContentView::ContentView(QWidget *parent) : QScrollArea(parent), content(nullptr)
	{
		content=new QWidget(this);
		content->setLayout(new QVBoxLayout(content));
		for (const Event &event : Event::Events()) content->layout()->addWidget(new EventBlock(event,content));
		setWidget(content);
	}

	EventBlock::EventBlock(const Event &event,QWidget *parent) : QWidget(parent)
	{
		setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed));
		setLayout(new QHBoxLayout(this));
		layout()->addWidget(new DateBlock(event.Date(),this));
		layout()->addWidget(new QLabel(event.Label(),this));
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
