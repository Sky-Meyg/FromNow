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
		setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
		setLayout(new QVBoxLayout(this));
		layout()->addWidget(new QLabel(label,this));
	}
}
