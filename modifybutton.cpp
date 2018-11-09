#include "modifybutton.h"
QModifyButton::QModifyButton(QWidget *parent ,quint32 ID,QString ObjectName,QString Text)
	: QWidget(parent)
{
	m_ID=quint32(-1);
	m_Text=Text;
	m_ID=ID;
	m_Enabled=true;
	m_EnabledModify=true;
	m_Status=MODIFYBUTTON_STATUS_DISPLAY;
	setObjectName(ObjectName);
	m_LabelButton	=	new QLabelButton(this);
	m_LabelButton->setObjectName(ObjectName+QString("LabelButton"));
	m_TextEdit		=	new QModifySubEdit(this);
	m_LabelButton->setObjectName(ObjectName+QString("Edit"));
	m_LabelButton->setMinimumSize(QSize(1, 1));
	m_LabelButton->setMaximumSize(QSize(16777215, 22));
	m_LabelButton->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	m_TextEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	QFont font;
	font.setPixelSize(22);
	m_LabelButton->setFont(font);

	m_TextEdit->setMinimumSize(QSize(1, 1));
	m_TextEdit->setMaximumSize(QSize(16777215, 22));

	m_TextEdit->setPlainText(m_Text);
	m_LabelButton->setText(m_Text);
	
	m_TextEdit->setVisible(false);
	m_LabelButton->setVisible(true);

	m_TextEdit->setFrameShape(QFrame::NoFrame);
	m_TextEdit->setFrameShadow(QFrame::Plain); 
	m_TextEdit->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
	m_TextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_TextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_TextEdit->setLineWrapMode(QTextEdit::NoWrap);

	connect(m_LabelButton,SIGNAL(SIGNAL_LButtonDown()),this,SLOT(ButtonClicked()));
	connect(m_TextEdit,SIGNAL(OnConfirm()),this,SLOT(ExitModify()));
	connect(m_TextEdit,SIGNAL(OnCancel()),this,SLOT(CancelModify()));
	
	m_TextEdit->adjustSize();
	m_LabelButton->adjustSize();
	QMetaObject::connectSlotsByName(this);
}

QModifyButton::~QModifyButton()
{
	if (m_LabelButton)
	{
		m_LabelButton->deleteLater();
		m_LabelButton=NULL;
	}
	if (m_TextEdit)
	{
		m_TextEdit->deleteLater();
		m_TextEdit=NULL;
	}
}

void QModifyButton::focusOutEvent( QFocusEvent * )
{
		ExitModify();
}

void QModifyButton::keyPressEvent( QKeyEvent * e)
{
	if (e->key()==Qt::Key_Enter)
	{
		ExitModify();
	}
}


void QModifyButton::resizeEvent( QResizeEvent * e)
{
	m_TextEdit->setFixedSize(e->size());
	m_LabelButton->setFixedSize(e->size());
	m_TextEdit->adjustSize();
	m_LabelButton->adjustSize();
}

void QModifyButton::mousePressEvent( QMouseEvent * )
{
	EnterModify();
}

void QModifyButton::EnterModify()
{
	if (m_EnabledModify)
	{
		if (m_Enabled)
		{
			m_TextEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
			m_Status=MODIFYBUTTON_STATUS_MODIFY;
			m_TextEdit->setVisible(true);
			m_LabelButton->setVisible(false);
			m_TextEdit->selectAll();
			m_TextEdit->adjustSize();
			m_LabelButton->adjustSize();
			m_TextEdit->setFocus();
		}
	}
}

void QModifyButton::ExitModify()
{
	m_Status=MODIFYBUTTON_STATUS_DISPLAY;
	m_TextEdit->setVisible(false);
	m_LabelButton->setVisible(true);
	if (m_EnabledModify)
	{
		if (m_Enabled)
		{
		if(m_TextEdit->toPlainText()!=m_Text)
		{
			SetText(m_TextEdit->toPlainText());
			emit OnModify(m_ID,m_Text);
		}
		return;
		}
	}
	m_TextEdit->setPlainText(m_Text);
	m_LabelButton->setText(m_Text);
	m_TextEdit->adjustSize();
	m_LabelButton->adjustSize();

}

void QModifyButton::CancelModify()
{
	m_TextEdit->setVisible(false);
	m_LabelButton->setVisible(true);

	m_TextEdit->setPlainText(m_Text);
	m_LabelButton->setText(m_Text);

	m_TextEdit->adjustSize();
	m_LabelButton->adjustSize();
}
void QModifyButton::ButtonClicked()
{
	
	emit OnClicked(m_ID);
	EnterModify();
}

void QLabelButton::mousePressEvent(QMouseEvent * event)
{
	if (event->button()==Qt::LeftButton)
	{
		emit SIGNAL_LButtonDown();
	}
}
