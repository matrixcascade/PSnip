#include "qsnapshotitem.h"

QSnapshotItem::QSnapshotItem(QWidget *parent)
	: QWidget(parent)
{
	m_IsCollector=false;
	m_ID=-1;
	this->setMinimumSize(QSize(531, 100));
	this->setMaximumSize(QSize(16777215, 128));
	horizontalLayout_7 = new QHBoxLayout(this);
	horizontalLayout_7->setSpacing(6);
	horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
	horizontalLayout_7->setObjectName(("horizontalLayout_7"));
	horizontalLayout_7->addSpacing(QSNAPSHOTITEM_MINIPIXMAP_SIZE_WIDTH+ QSNAPSHOTITEM_MINIPIXMAP_SIZE_XOFFSET);
	verticalLayout_3 = new QVBoxLayout();
	verticalLayout_3->setSpacing(0);
	verticalLayout_3->setObjectName(("verticalLayout_3"));
	horizontalLayout_8 = new QHBoxLayout();
	horizontalLayout_8->setSpacing(6);
	horizontalLayout_8->setObjectName(("horizontalLayout_8"));
	label_diaplayname = new QModifyButton(this);
	label_diaplayname->setObjectName(("label_diaplayname"));

	horizontalLayout_8->addWidget(label_diaplayname);

	horizontalSpacer_6 = new QSpacerItem(164, 32, QSizePolicy::Fixed, QSizePolicy::Minimum);

	horizontalLayout_8->addItem(horizontalSpacer_6);


	verticalLayout_3->addLayout(horizontalLayout_8);

	horizontalLayout_9 = new QHBoxLayout();
	horizontalLayout_9->setSpacing(0);
	horizontalLayout_9->setObjectName(("horizontalLayout_9"));

	pushButton_delete = new QPushButton(this);
	pushButton_delete->setObjectName(("pushButton_delete"));
	pushButton_delete->setMinimumSize(QSize(32, 32));
	pushButton_delete->setMaximumSize(QSize(32, 32));
	pushButton_delete->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/QSnapshotItem/Resources/delete.png);}\n"
		"		QPushButton:hover{border-image: url(:/QSnapshotItem/Resources/delete_Cursor.png);}\n"
		"		QPushButton:pressed{border-image: url(:/QSnapshotItem/Resources/delete_Down.png);}"));

	horizontalLayout_9->addWidget(pushButton_delete);

	pushButton_export = new QPushButton(this);
	pushButton_export->setObjectName(("pushButton_open"));
	pushButton_export->setMinimumSize(QSize(32, 32));
	pushButton_export->setMaximumSize(QSize(32, 32));
	pushButton_export->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/QSnapshotItem/Resources/open.png);}\n"
		"		QPushButton:hover{border-image: url(:/QSnapshotItem/Resources/open_Cursor.png);}\n"
		"		QPushButton:pressed{border-image: url(:/QSnapshotItem/Resources/open_down.png);}"));

	horizontalLayout_9->addWidget(pushButton_export);

	pushButton_close = new QPushButton(this);
	pushButton_close->setObjectName(("pushButton_close"));
	pushButton_close->setMinimumSize(QSize(32, 32));
	pushButton_close->setMaximumSize(QSize(32, 32));
	pushButton_close->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/QSnapshotItem/Resources/close.png);}\n"
		"		QPushButton:hover{border-image: url(:/QSnapshotItem/Resources/close_Cursor.png);}\n"
		"		QPushButton:pressed{border-image: url(:/QSnapshotItem/Resources/close_down.png);}"));

	horizontalLayout_9->addWidget(pushButton_close);

	pushButton_restore = new QPushButton(this);
	pushButton_restore->setObjectName(("pushButton_restore"));
	pushButton_restore->setMinimumSize(QSize(32, 32));
	pushButton_restore->setMaximumSize(QSize(32, 32));
	pushButton_restore->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/QSnapshotItem/Resources/Restore.png);}\n"
		"		QPushButton:hover{border-image: url(:/QSnapshotItem/Resources/Restore_Cursor.png);}\n"
		"		QPushButton:pressed{border-image: url(:/QSnapshotItem/Resources/Restore_Down.png);}"));

	horizontalLayout_9->addWidget(pushButton_restore);

	pushButton_cut = new QPushButton(this);
	pushButton_cut->setObjectName(("pushButton_cut"));
	pushButton_cut->setMinimumSize(QSize(32, 32));
	pushButton_cut->setMaximumSize(QSize(32, 32));
	pushButton_cut->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/QSnapshotItem/Resources/cut.png);}\n"
		"		QPushButton:hover{border-image: url(:/QSnapshotItem/Resources/cut_Cursor.png);}\n"
		"		QPushButton:pressed{border-image: url(:/QSnapshotItem/Resources/cut_down.png);}"));

	horizontalLayout_9->addWidget(pushButton_cut);

	pushButton_save = new QPushButton(this);
	pushButton_save->setObjectName(("pushButton_save"));
	pushButton_save->setMinimumSize(QSize(32, 32));
	pushButton_save->setMaximumSize(QSize(32, 32));
	pushButton_save->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/QSnapshotItem/Resources/save.png);}\n"
		"		QPushButton:hover{border-image: url(:/QSnapshotItem/Resources/save_Cursor.png);}\n"
		"		QPushButton:pressed{border-image: url(:/QSnapshotItem/Resources/save_down.png);}"));

	horizontalLayout_9->addWidget(pushButton_save);

	pushButton_scale = new QPushButton(this);
	pushButton_scale->setObjectName(("pushButton_scale"));
	pushButton_scale->setMinimumSize(QSize(32, 32));
	pushButton_scale->setMaximumSize(QSize(32, 32));
	pushButton_scale->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/QSnapshotItem/Resources/scale.png);}\n"
		"		QPushButton:hover{border-image: url(:/QSnapshotItem/Resources/scale_Cursor.png);}\n"
		"		QPushButton:pressed{border-image: url(:/QSnapshotItem/Resources/scale_down.png);}"));

	horizontalLayout_9->addWidget(pushButton_scale);

	

	pushButton_pick = new QPushButton(this);
	pushButton_pick->setObjectName(("pushButton_pick"));
	pushButton_pick->setMinimumSize(QSize(32, 32));
	pushButton_pick->setMaximumSize(QSize(32, 32));
	pushButton_pick->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/QSnapshotItem/Resources/Pick.png);}\n"
		"		QPushButton:hover{border-image: url(:/QSnapshotItem/Resources/Pick_Cursor.png);}\n"
		"		QPushButton:pressed{border-image: url(:/QSnapshotItem/Resources/Pick_down.png);}"));

	horizontalLayout_9->addWidget(pushButton_pick);

	horizontalSlider = new QSlider(this);
	horizontalSlider->setObjectName(("horizontalSlider"));
	horizontalSlider->setFixedSize(84, 20);
	horizontalSlider->setOrientation(Qt::Horizontal);
	horizontalSlider->setMinimum(1);
	horizontalSlider->setMaximum(255);
	horizontalSlider->setMinimum(32);
	horizontalSlider->setValue(255);
	horizontalLayout_9->addWidget(horizontalSlider);

	horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	horizontalLayout_9->addItem(horizontalSpacer_7);


	verticalLayout_3->addLayout(horizontalLayout_9);


	horizontalLayout_7->addLayout(verticalLayout_3);
	//Buttons disabled
	pushButton_scale->setVisible(false);
	pushButton_close->setVisible(false);
	pushButton_export->setVisible(false);

	//Languages
	pushButton_cut->setToolTip(tr("Cut mode"));
	pushButton_pick->setToolTip(tr("Pick color mode"));
	pushButton_restore->setToolTip(tr("Restore Image"));
	pushButton_save->setToolTip(tr("Save Image File"));
	pushButton_scale->setToolTip(tr("Scale Mode"));
	pushButton_delete->setToolTip(tr("delete"));

	m_CursorPixmapRight.load(":/QSnapshotItem/Resources/CursorRight.png");
	m_CursorPixmapLeft.load(":/QSnapshotItem/Resources/CursorLeft.png");
	setMouseTracking(true);
	
	m_DestColor = QColor(255, 255, 255);
	m_CurrColor = QColor(255, 255, 255);
	m_bMouseCursorOverContain = false;
	m_bOpenMode = false;
	m_CursorCurXOft = -32;
	m_CursorDestOft = -32;
	connect(&m_AnimationCursorTimer, SIGNAL(timeout()), this, SLOT(SLOT_TimerAnimationCursor()));
	connect(&m_AnimationOpenTimer, SIGNAL(timeout()), this, SLOT(SLOT_TimerAnimationOpen()));
	connect(&m_AnimationCloseTimer, SIGNAL(timeout()), this, SLOT(SLOT_TimerAnimationClose()));
	connect(pushButton_close, SIGNAL(clicked()), this, SLOT(SLOT_onButtonClose()));
	connect(pushButton_export, SIGNAL(clicked()), this, SLOT(SLOT_onButtonOpen()));
	connect(pushButton_restore, SIGNAL(clicked()), this, SLOT(SLOT_onButtonRestore()));
	connect(pushButton_pick, SIGNAL(clicked()), this, SLOT(SLOT_onButtonPick()));
	connect(pushButton_cut, SIGNAL(clicked()), this, SLOT(SLOT_onButtonCut()));
	connect(pushButton_save, SIGNAL(clicked()), this, SLOT(SLOT_onButtonSave()));
	connect(pushButton_delete, SIGNAL(clicked()), this, SLOT(SLOT_onButtonDelete()));
	connect(pushButton_scale, SIGNAL(clicked()), this, SLOT(SLOT_onButtonScale()));
	connect(horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(SLOT_onSliderChanged(int)));
	
}

QSnapshotItem::~QSnapshotItem()
{

}

void QSnapshotItem::paintEvent(QPaintEvent *e)
{
	QPainter pt(this);
	pt.fillRect(QRect(0+m_OffsetMargins,0+m_OffsetMargins,width()+m_OffsetMargins,height()+m_OffsetMargins), m_CurrColor);
	if (m_bMouseCursorOverContain)
	{
		QPen pen(QColor(0, 0, 0));
		pt.setPen(pen);
		pt.drawRect(QRect(m_OffsetMargins, m_OffsetMargins, width()-1+m_OffsetMargins, height()-1+m_OffsetMargins));
	}
	QPixmap DrawPixmap=m_CursorPixmapRight;
	if (!m_bOpenMode)
	{
		DrawPixmap = m_CursorPixmapLeft;
	}
	if (m_CursorCurXOft != 0)
	{
		pt.drawPixmap(
			QRect(width() - QSNAPSHOTITEM_CURSOR_OFT - 32 , height() / 2 - 16, 32, 32),
			DrawPixmap,
			QRect(-m_CursorCurXOft, 0, 32, 32)
		);
	}
	else
	{
		pt.drawPixmap(
			QRect(width() - QSNAPSHOTITEM_CURSOR_OFT - 32 + m_CursorCurXOft, height() / 2 - 16, 32, 32),
			DrawPixmap
		);
	}


	pt.drawPixmap(QSNAPSHOTITEM_MINIPIXMAP_SIZE_XOFFSET+ (QSNAPSHOTITEM_MINIPIXMAP_SIZE_WIDTH - m_MiniPixmap.width()) / 2, (height()- m_MiniPixmap.height()) / 2,  m_MiniPixmap);
}

void QSnapshotItem::mouseMoveEvent(QMouseEvent *e)
{
	if (m_bMouseCursorOverContain == false)
	{
		m_CursorDestOft = 0;
		m_DestColor = QColor(192, 192, 192);
		m_AnimationCursorTimer.start(50);
		if (m_bOpenMode)
		{
			m_CursorCurXOft = -32;
			m_AnimationOpenTimer.start(35);
		}
		else
		{
			m_CursorCurXOft =  32;
			m_AnimationCloseTimer.start(35);
		}
		m_bMouseCursorOverContain = true;
		emit SIGNAL_onMouseFocus(m_ID);
	}
}

void QSnapshotItem::mousePressEvent(QMouseEvent * e)
{
	if (e->button() == Qt::LeftButton) {
		m_OffsetMargins = -3;
		update();
	}
	
}

void QSnapshotItem:: mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		m_OffsetMargins =0;
		if (m_bOpenMode)
		{
			m_CursorDestOft = 32;
		}
		else
		{
			m_CursorDestOft = -32;
		}
		emit SIGNAL_onMousedown(m_ID);
		update();
	}
}

void QSnapshotItem::leaveEvent(QEvent *e)
{
	m_OffsetMargins =0;
	m_DestColor = QColor(255, 255, 255);
	m_AnimationCursorTimer.start(50);
	m_bMouseCursorOverContain = false;

	if (m_bOpenMode)
	{
		m_CursorDestOft = 32;
		m_AnimationOpenTimer.start(35);
	}
	else
	{
		m_CursorDestOft = -32;
		m_AnimationCloseTimer.start(35);
	}
	emit SIGNAL_onMouseLostFocus(m_ID);

}

void QSnapshotItem::SLOT_onButtonOpen()
{
	emit SIGNAL_onOpen(m_ID);
}

void QSnapshotItem::SLOT_onButtonClose()
{
	emit SIGNAL_onClose(m_ID);
}

void QSnapshotItem::SLOT_onButtonDelete()
{
	emit SIGNAL_onDelete(m_ID);
}

void QSnapshotItem::SLOT_onButtonCut()
{
	emit SIGNAL_onCut(m_ID);
}

void QSnapshotItem::SLOT_onButtonScale()
{
	emit SIGNAL_onScale(m_ID);
}

void QSnapshotItem::SLOT_onButtonPick()
{
	emit SIGNAL_onPick(m_ID);
}

void QSnapshotItem::SLOT_onButtonSave()
{
	emit SIGNAL_onSave(m_ID);
}

void QSnapshotItem::SLOT_onButtonRestore()
{
	emit SIGNAL_onRestore(m_ID);
}

void QSnapshotItem::SLOT_onSliderChanged(int val)
{
	emit SIGNAL_onAlphaChange(m_ID,val);
}

void QSnapshotItem::SLOT_TimerAnimationOpen()
{
	if (m_CursorCurXOft<m_CursorDestOft)
	{
		m_CursorCurXOft += QSNAPSHOTITEM_CURSOR_STEP;
	}
	else
	{
		m_AnimationCloseTimer.stop();
		m_AnimationOpenTimer.stop();
		m_CursorCurXOft = m_CursorDestOft;
	}
	update();
}

void QSnapshotItem::SLOT_TimerAnimationClose()
{
	if (m_CursorCurXOft > m_CursorDestOft)
	{
		m_CursorCurXOft -= QSNAPSHOTITEM_CURSOR_STEP;
	}
	else
	{
		m_AnimationCloseTimer.stop();
		m_AnimationOpenTimer.stop();
		m_CursorCurXOft = m_CursorDestOft;
	}
	update();
}

void QSnapshotItem::SLOT_TimerAnimationCursor()
{
	if (m_DestColor.red()-m_CurrColor.red()>24)
	{
		m_CurrColor.setRed(m_CurrColor.red() + 24);
		m_CurrColor.setGreen(m_CurrColor.green() + 24);
		m_CurrColor.setBlue(m_CurrColor.blue() + 24);
	}
	else if (m_DestColor.red() - m_CurrColor.red() < -24)
	{
		m_CurrColor.setRed(m_CurrColor.red() - 24);
		m_CurrColor.setGreen(m_CurrColor.green() - 24);
		m_CurrColor.setBlue(m_CurrColor.blue() - 24);
	}
	else
	{
		m_CurrColor = m_DestColor;
		m_AnimationCursorTimer.stop();
	}
	update();
}
void QSnapshotItem::SetMiniPixmap(QPixmap pixmap)
{
	if (pixmap.width()>pixmap.height())
	{
		m_MiniPixmap = pixmap.scaledToWidth(QSNAPSHOTITEM_MINIPIXMAP_SIZE_WIDTH);
	}
	else
	{
		m_MiniPixmap = pixmap.scaledToHeight(QSNAPSHOTITEM_MINIPIXMAP_SIZE_HEIGHT);
	}

}

void QSnapshotItem::AsCollectionItem()
{
	pushButton_export->setVisible(false);
	pushButton_cut->setVisible(false);
	pushButton_save->setVisible(false);
	m_IsCollector = true;
}