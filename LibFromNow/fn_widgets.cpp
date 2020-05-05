#include <QBoxLayout>
#include <QStackedLayout>
#include <QFrame>
#include <QGroupBox>
#include "fn_widgets.h"

namespace FromNow
{
	CreateBar::CreateBar(QWidget *parent) : QToolBar(parent)
	{
		calendar=new QCalendarWidget(this);
		calendar->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
		addWidget(calendar);
		QFrame *container=new QFrame(this);
		QSizePolicy containerPolicy=container->sizePolicy();
		containerPolicy.setHorizontalStretch(2);
		containerPolicy.setVerticalStretch(2);
		container->setSizePolicy(containerPolicy);
		container->setLayout(new QVBoxLayout(container));
		label=new QTextEdit(container);
		connect(label,&QTextEdit::textChanged,this,&CreateBar::TextChanged);
		container->layout()->addWidget(label);
		addEvent=new QPushButton("Add Event",container);
		addEvent->setEnabled(false);
		connect(addEvent,&QPushButton::clicked,this,&CreateBar::AddClicked);
		container->layout()->addWidget(addEvent);
		addWidget(container);
	}

	void CreateBar::TextChanged()
	{
		if (label->toPlainText().isEmpty())
			addEvent->setEnabled(false);
		else
			addEvent->setEnabled(true);
	}

	void CreateBar::AddClicked()
	{
		Event event(calendar->selectedDate(),label->toPlainText(),Units::DAYS);
		Event::Add(event);
		emit Add(event);
		label->clear();
	}

	ContentView::ContentView(QWidget *parent) : QScrollArea(parent), content(nullptr)
	{
		setWidgetResizable(true);
		content=new QFrame(this);
		content->setLayout(new QVBoxLayout(content));
		content->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
		for (const Event &event : Event::Events())
		{
			EventBlock *eventBlock=new EventBlock(event,content);
			connect(eventBlock,&EventBlock::Remove,this,&ContentView::Remove);
			content->layout()->addWidget(eventBlock);
		}
		setWidget(content);
	}

	EventBlock::EventBlock(const Event &event,QWidget *parent) : QWidget(parent)
	{
		setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
		setLayout(new QHBoxLayout(this));
		layout()->addWidget(new DateBlock(event,this));
		layout()->addWidget(new UnitBlock(event,this));
		layout()->addWidget(new DetailsBlock(event,this));
		remove=new QPushButton("Remove",this);
		connect(remove,&QPushButton::clicked,[this,event]() { emit Remove(event); });
		layout()->addWidget(remove);
	}

	UnitBlock::UnitBlock(const Event &event,QWidget *parent) : QWidget(parent)
	{
		setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
		setLayout(new QGridLayout(this));
		QGroupBox *groupBox=new QGroupBox(this);
		groupBox->setLayout(new QVBoxLayout(groupBox));
		days=new QRadioButton("Days",groupBox);
		groupBox->layout()->addWidget(days);
		months=new QRadioButton("Months",groupBox);
		groupBox->layout()->addWidget(months);
		years=new QRadioButton("Years",groupBox);
		groupBox->layout()->addWidget(years);
		layout()->addWidget(groupBox);

		switch (event.Unit())
		{
		case Units::DAYS:
			days->setChecked(true);
			break;
		case Units::MONTHS:
			months->setChecked(true);
			break;
		case Units::YEARS:
			years->setChecked(true);
			break;
		default:
			throw std::logic_error("Unsupported units encountered when constructing unit block");
		}

		connect(days,&QRadioButton::toggled,[event](bool checked) {
			if (!checked) return;
			Event::Edit(event.ID(),Units::DAYS);
			Event::Write();
		});
		connect(months,&QRadioButton::toggled,[event](bool checked) {
			if (!checked) return;
			Event::Edit(event.ID(),Units::MONTHS);
			Event::Write();
		});
		connect(years,&QRadioButton::toggled,[event](bool checked) {
			if (!checked) return;
			Event::Edit(event.ID(),Units::YEARS);
			Event::Write();
		});
	}

	DateBlock::DateBlock(const Event &event,QWidget *parent) : QWidget(parent)
	{
		setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
		setLayout(new QVBoxLayout(this));
		month=new QLabel(QDate::shortMonthName(event.Date().month()),this);
		month->setStyleSheet("font-size: 14pt");
		month->setAlignment(Qt::AlignCenter);
		layout()->addWidget(month);
		day=new QLabel(QString::number(event.Date().day()),this);
		day->setStyleSheet("font-size: 18pt");
		day->setAlignment(Qt::AlignCenter);
		layout()->addWidget(day);
		year=new QLabel(QString::number(event.Date().year()),this);
		year->setStyleSheet("font-size: 12pt");
		year->setAlignment(Qt::AlignCenter);
		layout()->addWidget(year);
	}

	DetailsBlock::DetailsBlock(const Event &event,QWidget *parent) : QWidget(parent)
	{
		setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
		setLayout(new QVBoxLayout(this));
		switch (event.Unit())
		{
			case Units::DAYS:
			{
				if (event.Days() < 0)
					count=new QLabel(QString("%1 days until").arg(QString::number(event.AbsoluteCount(event.Days()))),this);
				else
					count=new QLabel(QString("%1 days since").arg(QString::number(event.AbsoluteCount(event.Days()))),this);
				break;
			}
			case Units::MONTHS:
			{
				qint64 months=event.Months();
				if (months == 0)
				{
					count=new QLabel("Less than a month until",this);
					break;
				}
				if (months < 0)
				{
					count=new QLabel(QString("%1 months since").arg(QString::number(event.AbsoluteCount(event.Months()))),this);
					break;
				}
				count=new QLabel(QString("%1 months until").arg(QString::number(event.AbsoluteCount(event.Months()))),this);
				break;
			}
			case Units::YEARS:
			{
				qint64 years=event.Years();
				if (years == 0)
				{
					count=new QLabel("Less than a year until",this);
					break;
				}
				if (years < 0)
				{
					count=new QLabel(QString("%1 years since").arg(QString::number(event.AbsoluteCount(event.Years()))),this);
					break;
				}
				count=new QLabel(QString("%1 years until").arg(QString::number(event.AbsoluteCount(event.Years()))),this);
				break;
			}
			default:
				throw std::logic_error("Unsupported unit encountered when constructing details block");
		}
		count->setStyleSheet("font-size: 20pt");
		count->setAlignment(Qt::AlignCenter);
		layout()->addWidget(count);
		label=new SecretEdit(event.Label(),this);
		connect(label,&SecretEdit::Edited,[event](const QString label) {
			Event::Edit(event.ID(),label);
			Event::Write();
		});
		layout()->addWidget(label);
	}

	SecretEdit::SecretEdit(const QString &initialText,QWidget *parent) : QWidget(parent)
	{
		setLayout(new QStackedLayout(this));
		label=new QPushButton(initialText,this);
		label->setFlat(true);
		connect(label,&QPushButton::clicked,[this]() { static_cast<QStackedLayout*>(layout())->setCurrentIndex(1); });
		layout()->addWidget(label);
		edit=new QLineEdit(initialText,this);
		edit->setAlignment(Qt::AlignCenter);
		connect(edit,&QLineEdit::editingFinished,[this]() {
			static_cast<QStackedLayout*>(layout())->setCurrentIndex(0);
			label->setText(edit->text());
			emit Edited(edit->text());
		});
		layout()->addWidget(edit);
	}
}
