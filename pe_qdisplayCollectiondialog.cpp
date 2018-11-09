#include "pe_qdisplayCollectiondialog.h"

pe_qDisplayCollectionDialog::pe_qDisplayCollectionDialog(QWidget *parent)
	: QDialog(parent)
{
	m_CurrentDisplayIndex=-1;
	m_bMouseMoving = false;
	m_opt = 255;
	m_ID=-1;
	m_bMouseMoving=false;
	m_bBorder=true;
	m_bIsOnPreview=false;
	m_Status=0;
	m_LastStatus=0;
	m_OffsetVector=QPoint(0,0);
	m_lastUpdateVector=QPoint(0,0);
	m_GuiderPixmap = QPixmap(":/PSnip/Resources/CollectionGuider.png");
	m_ColorHelperDialog = new pe_SubColorHelperDialog(this);
	m_ColorHelperDialog->setVisible(false);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	this->setModal(false);

	m_cursor_move = QCursor(Qt::ArrowCursor);
	m_cursor_pick = QCursor(QPixmap(":/QSnapshotItem/Resources/Pick.png"), 0, 31);
	m_BorderColor = QColor(0,0,0);

	connect(&m_CloseTimer, SIGNAL(timeout()), this, SLOT(SLOT_ClosingTimerAnimation()));
	connect(&m_OpeningTimer, SIGNAL(timeout()), this, SLOT(SLOT_OpeningTimerAnimation()));

	this->setCursor(m_cursor_move);

	//Network
	m_Networkmanager = new QNetworkAccessManager(this);
	connect(m_Networkmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(SLOT_NewWork_URLDownloadCompleted(QNetworkReply*)));

	//////////////////////////////////////////////////////////////////////////
	//Shortcut Action
	//////////////////////////////////////////////////////////////////////////

	QAction *ActionRestore = new QAction(this);
	ActionRestore->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
	connect(ActionRestore, SIGNAL(triggered()), this, SLOT(SLOT_Shortcut_Restore()));
	this->addAction(ActionRestore);

	QAction *ActionClip = new QAction(this);
	ActionClip->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
	connect(ActionClip, SIGNAL(triggered()), this, SLOT(SLOT_Shortcut_ClipboardSourceImage()));
	this->addAction(ActionClip);

	QAction *ActionClipX = new QAction(this);
	ActionClipX->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
	connect(ActionClipX, SIGNAL(triggered()), this, SLOT(SLOT_Shortcut_ClipboardCurrentImage()));
	this->addAction(ActionClipX);

	QAction *ActionSave = new QAction(this);
	ActionSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
	connect(ActionSave, SIGNAL(triggered()), this, SLOT(SLOT_Shortcut_Save()));
	this->addAction(ActionSave);

	QAction *ActionPaste = new QAction(this);
	ActionPaste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
	connect(ActionPaste, SIGNAL(triggered()), this, SLOT(SLOT_PasteClipboardImage()));
	this->addAction(ActionPaste);


	QAction *ActionPick= new QAction(this);
	ActionPick->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
	connect(ActionPick, SIGNAL(triggered()), this, SLOT(SLOT_Menu_Pick()));
	this->addAction(ActionPick);

	QAction *ActionAlphaInc = new QAction(this);
	ActionAlphaInc->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));
	connect(ActionAlphaInc, SIGNAL(triggered()), this, SLOT(SLOT_Menu_AlphaAdd()));
	this->addAction(ActionAlphaInc);

	QAction *ActionAlphaDec = new QAction(this);
	ActionAlphaDec->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));
	connect(ActionAlphaDec, SIGNAL(triggered()), this, SLOT(SLOT_Menu_AlphaSub()));
	this->addAction(ActionAlphaDec);

	QAction *ActionPrevious = new QAction(this);
	ActionPrevious->setShortcut(QKeySequence(Qt::Key_Tab));
	connect(ActionPrevious, SIGNAL(triggered()), this, SLOT(SLOT_Shortcut_Next()));
	this->addAction(ActionPrevious);

	QAction *ActionDelete = new QAction(this);
	ActionDelete->setShortcut(QKeySequence(Qt::Key_Delete));
	connect(ActionDelete, SIGNAL(triggered()), this, SLOT(SLOT_Shortcut_Remove()));
	this->addAction(ActionDelete);

	//////////////////////////////////////////////////////////////////////////
	//Dragged
	connect(this, SIGNAL(SIGNAL_DropImageFile(QString)), this, SLOT(SLOT_DragEvent_DragFile(QString)));
	connect(this, SIGNAL(SIGNAL_DropURL(QString)), this, SLOT(SLOT_DragEvent_DragUrl(QString)));
	connect(this, SIGNAL(SIGNAL_onListView()), this, SLOT(SLOT_ManagerDialog_onImageItemsListView()));
	//////////////////////////////////////////////////////////////////////////
	//Menu
	m_Menu_Close = new QAction(this);
	m_Menu_Pick = new QAction(this);
	m_Menu_CopyCurrent = new QAction(this);
	m_Menu_CopySource = new QAction(this);
	m_Menu_AlphaAdd = new QAction(this); 
	m_Menu_AlphaSubtract = new QAction(this);
	m_Menu_Restore = new QAction(this);
	m_Menu_Next = new QAction(this);
	m_Menu_Previous = new QAction(this);
	m_Menu_RemoveCurrent = new QAction(this);
	m_Menu_ClipboardPaste = new QAction(this);
	m_Menu_AdjustMode = new QAction(this);
	m_Menu_CollapseAll = new QAction(this);
	m_Menu_DeleteThis = new QAction(this);

	m_Menu_DeleteThis->setText(tr("Delete this"));
	m_Menu_CollapseAll->setText(tr("Collapse all"));
	m_Menu_Close->setText(tr("Close (ESC)"));
	m_Menu_Pick->setText(tr("Pick Color Mode (ALT|Ctrl+P)"));
	m_Menu_ClipboardPaste->setText(tr("Paste (Ctrl+V)"));
	m_Menu_AdjustMode->setText(tr("Adjust Mode (Space)"));
	m_Menu_CopyCurrent->setText(tr("Copy current image to clipboard (Ctrl+X)"));
	m_Menu_CopySource->setText((tr("Copy source image to clipboard (Ctrl+C)")));
	m_Menu_Restore->setText(tr("Restore (Ctrl+Z)"));
	m_Menu_AlphaAdd->setText(tr("Increase opacity (Ctrl+Up)"));
	m_Menu_AlphaSubtract->setText(tr("Decrease opacity (Ctrl+Down)"));
	m_Menu_Next->setText(tr("Next (Tab)"));
	m_Menu_Previous->setText(tr("Previous"));
	m_Menu_RemoveCurrent->setText(tr("Remove (Delete)"));

	connect(m_Menu_Close, SIGNAL(triggered()), this, SLOT(SLOT_Menu_Close()));
	connect(m_Menu_Pick, SIGNAL(triggered()), this, SLOT(SLOT_Menu_Pick()));
	connect(m_Menu_CopyCurrent, SIGNAL(triggered()), this, SLOT(SLOT_Menu_CopyCurrent()));
	connect(m_Menu_CopySource, SIGNAL(triggered()), this, SLOT(SLOT_Menu_CopySource()));
	connect(m_Menu_AlphaAdd, SIGNAL(triggered()), this, SLOT(SLOT_Menu_AlphaAdd()));
	connect(m_Menu_AlphaSubtract, SIGNAL(triggered()), this, SLOT(SLOT_Menu_AlphaSub()));
	connect(m_Menu_Restore, SIGNAL(triggered()), this, SLOT(SLOT_Menu_Restore()));
	connect(m_Menu_Next, SIGNAL(triggered()), this, SLOT(SLOT_Menu_Next()));
	connect(m_Menu_Previous, SIGNAL(triggered()), this, SLOT(SLOT_Menu_Previous()));
	connect(m_Menu_RemoveCurrent, SIGNAL(triggered()), this, SLOT(SLOT_Menu_Remove()));
	connect(m_Menu_ClipboardPaste, SIGNAL(triggered()), this, SLOT(SLOT_PasteClipboardImage()));
	connect(m_Menu_AdjustMode, SIGNAL(triggered()), this, SLOT(SLOT_Menu_AdjustModeSwitch()));
	connect(m_Menu_CollapseAll, SIGNAL(triggered()), this, SLOT(SLOT_Menu_CollapseAll()));
	connect(m_Menu_DeleteThis, SIGNAL(triggered()), this, SLOT(SLOT_Menu_DeleteThis()));
	m_menu = new QMenu(this);
	
	this->setAcceptDrops(true);
}

pe_qDisplayCollectionDialog::~pe_qDisplayCollectionDialog()
{
}


void pe_qDisplayCollectionDialog::RemoveCurrent()
{
	if (m_CurrentDisplayIndex==-1)
	{
		return;
	}
	m_Pixmaps.erase(m_Pixmaps.begin() + m_CurrentDisplayIndex);
	SwitchNext();
}

void pe_qDisplayCollectionDialog::ScaleCurrentImage(float scale,bool AutoMove)
{
	if (m_CurrentDisplayIndex == -1)
	{
		return;
	}
	if (scale> DISPLAYCOLLECTIONDIALOG_SCALE_MAXTIMES)
	{
		scale = DISPLAYCOLLECTIONDIALOG_SCALE_MAXTIMES;
	}
	if (scale < DISPLAYCOLLECTIONDIALOG_SCALE_MINITIMES)
	{
		scale = DISPLAYCOLLECTIONDIALOG_SCALE_MINITIMES;
	}
	m_Pixmaps[m_CurrentDisplayIndex].Scale = scale;
	QPixmap &refSrcPixmap = m_Pixmaps[m_CurrentDisplayIndex].ResPixmap;
	QPixmap &refRenderPixmap = m_Pixmaps[m_CurrentDisplayIndex].RenderPixmap;
	int SrcWidth = refSrcPixmap.width();
	int SrcHeight = refSrcPixmap.height();

	int FinalWidth = SrcWidth*m_Pixmaps[m_CurrentDisplayIndex].Scale;
	int FinalHeight = SrcHeight*m_Pixmaps[m_CurrentDisplayIndex].Scale;
	if (AutoMove) 
	{
		int Incw = refRenderPixmap.width() - FinalWidth;
		int Inch = refRenderPixmap.height() - FinalHeight;
		QPoint PosOft = QPoint(Incw / 2, Inch / 2);
		m_Pixmaps[m_CurrentDisplayIndex].StartPoint += PosOft;
	}
	m_Pixmaps[m_CurrentDisplayIndex].RenderPixmap = m_Pixmaps[m_CurrentDisplayIndex].ResPixmap.scaledToHeight(FinalHeight);
	update();
}

void pe_qDisplayCollectionDialog::SwitchNext()
{
	if (m_Pixmaps.size() == 0)
	{
		m_CurrentDisplayIndex = -1;
		return;
	}
	m_CurrentDisplayIndex--;
	if (m_CurrentDisplayIndex < 0)
	{
		m_CurrentDisplayIndex = m_Pixmaps.size() - 1;
	}
	update();
}

void pe_qDisplayCollectionDialog::SwitchPrevious()
{
	if (m_Pixmaps.size()==0)
	{
		m_CurrentDisplayIndex = -1;
		return;
	}
	m_CurrentDisplayIndex++;
	if (m_CurrentDisplayIndex>=m_Pixmaps.size())
	{
		m_CurrentDisplayIndex = 0;
	}
	update();
}

void pe_qDisplayCollectionDialog::RestoreCurrentDisplay()
{
	if (m_CurrentDisplayIndex==-1)
	{
		return;
	}
	QPixmap opPixmap = m_Pixmaps[m_CurrentDisplayIndex].ResPixmap;

	int PixHeight = opPixmap.height();
	int PixWidth = opPixmap.width();
	int windowHeight = height();
	int windowWidth = width();
	if (PixHeight * 1000 / windowHeight > PixWidth * 1000 / windowWidth)
	{
		m_Pixmaps[m_CurrentDisplayIndex].RenderPixmap = opPixmap.scaledToHeight(windowHeight);
		m_Pixmaps[m_CurrentDisplayIndex].Scale = windowHeight / (PixHeight*1.0f);
	}
	else
	{
		m_Pixmaps[m_CurrentDisplayIndex].RenderPixmap = opPixmap.scaledToWidth(windowWidth);
		m_Pixmaps[m_CurrentDisplayIndex].Scale = windowWidth / (PixWidth*1.0f);
	}

	m_Pixmaps[m_CurrentDisplayIndex].StartPoint = QPoint((windowWidth - m_Pixmaps[m_CurrentDisplayIndex].RenderPixmap.width()) / 2, (windowHeight - m_Pixmaps[m_CurrentDisplayIndex].RenderPixmap.height()) / 2);
	update();
}

void pe_qDisplayCollectionDialog::AddPixmap(QPixmap Pixmap)
{
	CollectionPixmapInfo Info;
	Info.ResPixmap = Pixmap;
	m_Pixmaps.push_back(Info);
	m_CurrentDisplayIndex = m_Pixmaps.size() - 1;
	RestoreCurrentDisplay();
	update();
}

void pe_qDisplayCollectionDialog::AddCollectionPixInfo(CollectionPixmapInfo Info)
{
	m_Pixmaps.push_back(Info);
	m_CurrentDisplayIndex = m_Pixmaps.size() - 1;
	ScaleCurrentImage(Info.Scale,false);
	update();
}

void pe_qDisplayCollectionDialog::SetCurrentDisplayIndex(int i)
{
	if (i>m_Pixmaps.size()-1)
	{
		i = -1;
	}
	m_CurrentDisplayIndex = i;
	update();
}

int pe_qDisplayCollectionDialog::GetCurrentDisplayIndex()
{
	return m_CurrentDisplayIndex;
}

void pe_qDisplayCollectionDialog::SnipCacheForPickColor()
{
	m_bBorder = false;
	update();
	QPixmap pixmap = QPixmap::grabWidget(this, rect());
	m_CacheImage = pixmap.toImage();
	m_bBorder = true;
	update();
}

void pe_qDisplayCollectionDialog::ShowDirectly()
{
	this->setFixedSize(m_DialogLastSize);
	emit SIGNAL_onOpened(m_ID);
	show();
}

void pe_qDisplayCollectionDialog::CloseDirectly()
{
	emit SIGNAL_onClosed(m_ID);
	hide();
}

void pe_qDisplayCollectionDialog::SetShowSize(QSize size)
{
	m_DialogLastSize = size;
}


bool pe_qDisplayCollectionDialog::ShowAction()
{
	if (m_CloseTimer.isActive())
	{
		return false;
	}
	if (this->isVisible() == true||m_OpeningTimer.isActive())
	{
		return true;
	}
	setFixedSize(m_DialogLastSize.width(), 1);
	m_OpeningTimer.start(32);
	show();
	return true;
}

bool pe_qDisplayCollectionDialog::CloseAction()
{
	if (m_OpeningTimer.isActive())
	{
		return false;
	}
	if (this->isVisible()==false||m_CloseTimer.isActive())
	{
		return true;
	}
	m_DialogLastSize = this->size();
	m_CloseTimer.start(32);
	return true;
}

void pe_qDisplayCollectionDialog::CloseDialog()
{
	this->close();
	emit SIGNAL_onClosed(m_ID);
}

void pe_qDisplayCollectionDialog::SetOpacity(int opt)
{
	if (opt<0)
	{
		opt = 0;
	}
	if (opt>255)
	{
		opt = 255;
	}
	m_opt = opt;
	setWindowOpacity(m_opt / 255.0);
}


void pe_qDisplayCollectionDialog::NormalMode()
{
	m_ColorHelperDialog->setVisible(false);
	m_Status = DISPLAYCOLLECTIONDIALOG_STATUS_NORMALMODE;
	this->setCursor(m_cursor_move);
}

void pe_qDisplayCollectionDialog::EnterPickMode()
{
	if (m_CurrentDisplayIndex==-1)
	{
		return;
	}
	m_ColorHelperDialog->setVisible(false);
	SnipCacheForPickColor();
	m_Status = DISPLAYCOLLECTIONDIALOG_STATUS_PICKCOLORMODE;
	this->setCursor(m_cursor_pick);
}

void pe_qDisplayCollectionDialog::ExitPickMode()
{
	NormalMode();
}

void pe_qDisplayCollectionDialog::EnterAdjustMode()
{
	m_Status = DISPLAYCOLLECTIONDIALOG_STATUS_ADJUSTMODE;
	this->setCursor(Qt::DragMoveCursor);
}

void pe_qDisplayCollectionDialog::ExitAdjustMode()
{
	NormalMode();
}

void pe_qDisplayCollectionDialog::MoveColorHelperDialog(QPoint point)
{
	int x=point.x(), y=point.y();
	if (point.x()>width()/2)
	{
		x = point.x() - m_ColorHelperDialog->width()-32;
	}
	else
	{
		x = point.x() + 32;
	}
	if (point.y()>height()/2)
	{
		y = point.y() - m_ColorHelperDialog->height()-32;
	}
	
	if (x > width() - m_ColorHelperDialog->width())
	{
		x = width() - m_ColorHelperDialog->width();
	}
	if (y > height())
	{
		y = height()- m_ColorHelperDialog->height();
	}
	if (y<0)
	{
		y = 0;
	}
	if (x<0)
	{
		x = 0;
	}
	
	m_ColorHelperDialog->move(x, y);
}

void pe_qDisplayCollectionDialog::SetBorderColor(QColor clr)
{
	m_BorderColor = clr;
	update();
}

void pe_qDisplayCollectionDialog::ClipboardCurrentImage()
{
	m_bBorder = false;
	update();
	QApplication::clipboard()->setPixmap(QPixmap::grabWidget(this, rect()));
	m_bBorder = true;
}

void pe_qDisplayCollectionDialog::ClipboardSourceImage()
{
	if (m_CurrentDisplayIndex>m_Pixmaps.size()-1)
	{
		return;
	}
	QApplication::clipboard()->setPixmap(m_Pixmaps[m_CurrentDisplayIndex].ResPixmap);
}

void pe_qDisplayCollectionDialog::SaveImage()
{
	emit SIGNAL_onSave(m_ID);
}

void pe_qDisplayCollectionDialog::PushLastState()
{
	m_LastStatus = m_Status;
	NormalMode();
}

void pe_qDisplayCollectionDialog::RestoreLastState()
{
	
}

void pe_qDisplayCollectionDialog::ExecMenu()
{
	m_menu->clear();
	m_menu->addAction(m_Menu_Pick);
	m_menu->addAction(m_Menu_Next);
	m_menu->addAction(m_Menu_Previous);
	m_menu->addAction(m_Menu_RemoveCurrent);
	m_menu->addAction(m_Menu_Close);
	m_menu->addAction(m_Menu_AdjustMode);   
	m_menu->addAction(m_Menu_ClipboardPaste);
	m_menu->addAction(m_Menu_CopySource);
	m_menu->addAction(m_Menu_CopyCurrent);
	m_menu->addAction(m_Menu_Restore);
	m_menu->addAction(m_Menu_AlphaAdd);
	m_menu->addAction(m_Menu_AlphaSubtract);
	m_menu->addAction(m_Menu_CollapseAll);
	m_menu->addAction(m_Menu_DeleteThis);
	//m_menu->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	m_menu->exec(QCursor::pos());
}

void pe_qDisplayCollectionDialog::PaintStandby()
{
	if (m_OpeningTimer.isActive()|m_CloseTimer.isActive())
	{
		return;
	}
	QPainter pt(this);
	QPen pen(QColor(0, 0, 0));
	pt.setPen(pen);
	int Lenght = height() > width() ? width() : height();
	Lenght = Lenght * 2 / 3;
	if(Lenght!=0)
	pt.drawPixmap(QRect((width() - Lenght) / 2, (height() - Lenght) / 2, Lenght, Lenght), m_GuiderPixmap);
}

void pe_qDisplayCollectionDialog::PaintPreview()
{
	if (!m_bIsOnPreview)
	{
		return;
	}
	if (m_PreviewPixmap.isNull())
	{
		return;
	}
	QPainter pt(this);
	QPen pen(QColor(0, 255, 0));
	pt.setPen(pen);
	pt.drawPixmap(QPoint((width() - m_PreviewPixmap.width()) / 2, (height() - m_PreviewPixmap.height()) / 2), m_PreviewPixmap);
	pt.drawRect((width() - m_PreviewPixmap.width()) / 2, (height() - m_PreviewPixmap.height()) / 2, m_PreviewPixmap.width(), m_PreviewPixmap.height());
}

void pe_qDisplayCollectionDialog::PaintBoard()
{
	QPainter pt(this);
	if (m_bBorder) {
		QPainter pt(this);
		QPen bPen(m_BorderColor);
		if (isActiveWindow())
		{
			bPen.setColor(QColor(0, 0, 255));
		}
		pt.setPen(bPen);
		bPen.setWidth(4);
		pt.drawRect(QRect(0, 0, width() - 1, height() - 1));
	}
}

void pe_qDisplayCollectionDialog::PaintImage()
{
	if (m_CurrentDisplayIndex==-1)
	{
		return;
	}
	QPainter pt(this);
	QPoint StartPoint = m_Pixmaps[m_CurrentDisplayIndex].StartPoint;
	StartPoint = QPoint(0, 0) - StartPoint;
	pt.drawPixmap(rect(), m_Pixmaps[m_CurrentDisplayIndex].RenderPixmap, QRect(StartPoint.x(), StartPoint.y(), width(), height()));
}

void pe_qDisplayCollectionDialog::EnterPreview(QPixmap PreviewPixmap)
{
	
	m_bIsOnPreview = true;

	int PixHeight = PreviewPixmap.height();
	int PixWidght = PreviewPixmap.width();
	int windowHeight = height();
	int windowWidth = width();
	if (PixHeight*1000/windowHeight>PixWidght*1000/windowWidth)
	{
		m_PreviewPixmap = PreviewPixmap.scaledToHeight(windowHeight);
	}
	else
	{
		m_PreviewPixmap = PreviewPixmap.scaledToWidth(windowWidth);
	}
	update();
}

void pe_qDisplayCollectionDialog::ExitPreview()
{
	if (!m_bIsOnPreview)
		return;
	m_bIsOnPreview = false;
	update();
}

QPixmap pe_qDisplayCollectionDialog::GetCurrentDisplayPixmap()
{
	QPixmap pixmap;
	m_bBorder = false;
	update();
	pixmap=QPixmap::grabWidget(this, rect());
	m_bBorder = true;
	update();
	return pixmap;
}

QColor pe_qDisplayCollectionDialog::GetWidgetColor(QPoint Point)
{
	return m_CacheImage.pixel(Point);
}

void pe_qDisplayCollectionDialog::paintEvent(QPaintEvent *event)
{
	if (m_bIsOnPreview)
	{
		PaintPreview();
	}
	else
	{
		if (m_Pixmaps.size() == 0)
			PaintStandby();
		PaintImage();
		PaintBoard();
	}
	
}

void pe_qDisplayCollectionDialog::mousePressEvent(QMouseEvent *event)
{
	if(!this->isActiveWindow())
		this->activateWindow();

	switch (m_Status)
	{
	case DISPLAYCOLLECTIONDIALOG_STATUS_NORMALMODE:
	{
		switch (event->button()) {
		case Qt::LeftButton:
			m_bMouseMoving = true;
			if (m_Pixmaps.size() != 0) {
				m_lastUpdateVector = event->globalPos();
				m_OffsetVector = QPoint(0, 0);
			}
			else
			{
				m_OffsetVector = event->globalPos() - this->frameGeometry().topLeft();
			}
			break;
		case Qt::RightButton:
			ExecMenu();
			break;
		}
		break;
	case  DISPLAYCOLLECTIONDIALOG_STATUS_ADJUSTMODE:
	{
		switch (event->button()) {
		case Qt::LeftButton:
			m_bMouseMoving = true;
			m_OffsetVector = event->globalPos() - this->frameGeometry().topLeft();
			break;
		}
	}
	break;
	case  DISPLAYCOLLECTIONDIALOG_STATUS_PICKCOLORMODE:
	{
		if (event->button() == Qt::LeftButton)
		{
			if (!m_ColorHelperDialog->isVisible())
			{
				m_ColorHelperDialog->setVisible(true);
			}
			MoveColorHelperDialog(event->pos());
			m_ColorHelperDialog->SetColor(m_CacheImage.pixel(event->pos()));
		}
		if (event->button() == Qt::RightButton)
		{
			ExitPickMode();
		}
	}
	break;
	default:
		break;
	}
	QDialog::mousePressEvent(event);
	}
}
void pe_qDisplayCollectionDialog::mouseMoveEvent(QMouseEvent *event) 
{
	switch (m_Status)
	{
	case DISPLAYCOLLECTIONDIALOG_STATUS_NORMALMODE:
	{
		if (m_bMouseMoving)
		{
			if (m_Pixmaps.size() != 0) {
				if (m_CurrentDisplayIndex != -1)
				{
					QPoint _OffsetVector = event->globalPos() - m_lastUpdateVector;
					m_lastUpdateVector = event->globalPos();
					m_Pixmaps.at(m_CurrentDisplayIndex).StartPoint += _OffsetVector;
					update();
				}
			}
			else
			{
				if (m_bMouseMoving)
					this->move(event->globalPos() - m_OffsetVector);
			}
		}
	}
	break;
	case DISPLAYCOLLECTIONDIALOG_STATUS_ADJUSTMODE:
	{
		if (m_bMouseMoving)
			this->move(event->globalPos() - m_OffsetVector);

	}
	break;
	case  DISPLAYCOLLECTIONDIALOG_STATUS_PICKCOLORMODE:
	{
		if (!m_ColorHelperDialog->isVisible())
		{
			m_ColorHelperDialog->setVisible(true);
		}
		MoveColorHelperDialog(event->pos());
		m_ColorHelperDialog->SetColor(m_CacheImage.pixel(event->pos()));
	}
	break;
	}
}

void pe_qDisplayCollectionDialog::keyPressEvent(QKeyEvent *event)
{
	if (event->key()==Qt::Key_Space)
	{
		if (m_Status == DISPLAYCOLLECTIONDIALOG_STATUS_ADJUSTMODE)
			ExitAdjustMode();
		else
			EnterAdjustMode();
	}
	if (event->key()==Qt::Key_Alt)
	{
		EnterPickMode(); 
	}
	if (event->key()==Qt::Key_Escape)
	{
		switch (m_Status)
		{
		case DISPLAYCOLLECTIONDIALOG_STATUS_NORMALMODE:
			CloseAction();
			break;
		case DISPLAYCOLLECTIONDIALOG_STATUS_PICKCOLORMODE:
			ExitPickMode();
			break;
		}
	}
}

void pe_qDisplayCollectionDialog::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Alt)
	{
		if (m_Status == DISPLAYCOLLECTIONDIALOG_STATUS_PICKCOLORMODE)
			ExitPickMode();
	}

}

void pe_qDisplayCollectionDialog::mouseReleaseEvent(QMouseEvent *event)
{
	switch (m_Status)
	{
	case DISPLAYCOLLECTIONDIALOG_STATUS_NORMALMODE:
	{
		m_bMouseMoving = false;
		this->releaseMouse();
		if(m_Pixmaps.size()!=0)
		this->setCursor(QCursor(Qt::ArrowCursor));
	}
	break;
	case DISPLAYCOLLECTIONDIALOG_STATUS_ADJUSTMODE:
	{
		if (event->button() == Qt::LeftButton)
		{
			m_bMouseMoving = false;
			this->releaseMouse();
		}
		if (event->button() == Qt::RightButton)
			ExitAdjustMode();
	}
	break;
	case DISPLAYCOLLECTIONDIALOG_STATUS_PICKCOLORMODE:
	{

	}
	break;
	}
}

void pe_qDisplayCollectionDialog::wheelEvent(QWheelEvent *event)
{
	switch (m_Status)
	{
	case DISPLAYCOLLECTIONDIALOG_STATUS_NORMALMODE:
	{
		if (m_Pixmaps.size() != 0) {
			if (m_CurrentDisplayIndex == -1)
			{
				return;
			}
			float IncrementScale = event->delta() / 2000.f;
			this->ScaleCurrentImage(m_Pixmaps[m_CurrentDisplayIndex].Scale*(1.0 + IncrementScale));
		}
		else
		{
			float IncrementScale = event->delta() / 2000.f;
			int Incw = width()*IncrementScale;
			int Inch = height()*IncrementScale;
			QPoint PosOft = QPoint(Incw / 2, Inch / 2);
			this->move(this->pos() - PosOft);
			this->setFixedSize(Incw + width(), Inch + height());
		}
	}
	break;
	case  DISPLAYCOLLECTIONDIALOG_STATUS_ADJUSTMODE:
	{
		float IncrementScale = event->delta() / 2000.f;
		int Incw = width()*IncrementScale;
		int Inch = height()*IncrementScale;
		QPoint PosOft = QPoint(Incw / 2, Inch / 2);
		this->move(this->pos() - PosOft);
		this->setFixedSize(Incw + width(), Inch + height());
	}
	break;
	default:
		break;
	}
	
}



void pe_qDisplayCollectionDialog::resizeEvent(QResizeEvent *event)
{
	switch (m_Status)
	{
	case DISPLAYCOLLECTIONDIALOG_STATUS_PICKCOLORMODE:
	{
		SnipCacheForPickColor();
	}
	break;
	default:
		break;
	}
	
}


void pe_qDisplayCollectionDialog::enterEvent(QEvent *event)
{
	update();
}

void pe_qDisplayCollectionDialog::leaveEvent(QEvent *event)
{
	update();
}


void pe_qDisplayCollectionDialog::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void pe_qDisplayCollectionDialog::dropEvent(QDropEvent *event)
{
	QPixmap DropPixmap;
	const QMimeData *mimeData = event->mimeData();

	if (mimeData->hasImage()) {
		emit SIGNAL_DropPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
	}
	else if (mimeData->hasUrls()) {
		QList<QUrl> urlList = mimeData->urls();
		foreach(QUrl url, event->mimeData()->urls()) {
			QString file_name = url.toLocalFile();
			if (file_name.length() == 0)
				emit SIGNAL_DropURL(url.toString());
			else
				emit SIGNAL_DropImageFile(file_name);
		}
	}

	event->acceptProposedAction();
}

void pe_qDisplayCollectionDialog::dragMoveEvent(QDragMoveEvent *event)
{
	event->acceptProposedAction();
}

void pe_qDisplayCollectionDialog::dragLeaveEvent(QDragLeaveEvent *event)
{
	event->accept();
}

void pe_qDisplayCollectionDialog::SLOT_OpeningTimerAnimation()
{
	int clamper = (m_DialogLastSize.height() - height()) / 2;
	if (clamper>2)
	{
		this->move(this->pos() + QPoint(0, -clamper/2));
		this->setFixedHeight(height() + clamper);
		update();
	}
	else
	{
		setFixedSize(m_DialogLastSize);
		emit SIGNAL_onOpened(m_ID);
		m_OpeningTimer.stop();
	}
}

void pe_qDisplayCollectionDialog::SLOT_ClosingTimerAnimation()
{
	int clamper = height() / 2;
	if (clamper > 2)
	{
		this->move(this->pos() + QPoint(0, clamper / 2));
		this->setFixedHeight(height() - clamper);
		update();
	}
	else
	{
		m_CloseTimer.stop();
		CloseDialog();
	}
}

void pe_qDisplayCollectionDialog::SLOT_Shortcut_Next()
{
	SwitchNext();
}


void pe_qDisplayCollectionDialog::SLOT_Shortcut_Remove()
{
	RemoveCurrent();
}

void pe_qDisplayCollectionDialog::SLOT_Shortcut_ClipboardCurrentImage()
{
	ClipboardCurrentImage();
}

void pe_qDisplayCollectionDialog::SLOT_Shortcut_ClipboardSourceImage()
{
	ClipboardSourceImage();
}

void pe_qDisplayCollectionDialog::SLOT_Shortcut_Restore()
{
	RestoreCurrentDisplay();
}

void pe_qDisplayCollectionDialog::SLOT_Menu_Remove()
{
	RemoveCurrent();
}

void pe_qDisplayCollectionDialog::SLOT_Menu_Next()
{
	this->SwitchNext();
}

void pe_qDisplayCollectionDialog::SLOT_Menu_Previous()
{
	this->SwitchPrevious();
}

void pe_qDisplayCollectionDialog::SLOT_Menu_Close()
{
	CloseAction();
}


void pe_qDisplayCollectionDialog::SLOT_Menu_Pick()
{
	EnterPickMode();
}

void pe_qDisplayCollectionDialog::SLOT_Menu_CopyCurrent()
{
	ClipboardCurrentImage();
}

void pe_qDisplayCollectionDialog::SLOT_Menu_CopySource()
{
	ClipboardSourceImage();
}

void pe_qDisplayCollectionDialog::SLOT_Menu_AlphaAdd()
{
	SIGNAL_onAlphaIncreaseChanged(m_ID,10);
}

void pe_qDisplayCollectionDialog::SLOT_Menu_AlphaSub()
{
	SIGNAL_onAlphaIncreaseChanged(m_ID,-10);
}



void pe_qDisplayCollectionDialog::SLOT_Menu_Restore()
{
	RestoreCurrentDisplay();
	update();
}

void pe_qDisplayCollectionDialog::SLOT_Menu_Save()
{
	SaveImage();
}

void pe_qDisplayCollectionDialog::SLOT_Menu_AdjustModeSwitch()
{
	if (m_Status==DISPLAYCOLLECTIONDIALOG_STATUS_ADJUSTMODE)
	{
		ExitAdjustMode();
	}else
	{
		EnterAdjustMode();
	}
}

void pe_qDisplayCollectionDialog::SLOT_Menu_CollapseAll()
{
	emit SIGNAL_onCollapseAll();
}

void pe_qDisplayCollectionDialog::SLOT_Menu_DeleteThis()
{
	emit SIGNAL_onDeleteThis(m_ID);
}

void pe_qDisplayCollectionDialog::SLOT_DragEvent_DragImage(QPixmap dragPixmap)
{
	AddPixmap(dragPixmap);
}

void pe_qDisplayCollectionDialog::SLOT_DragEvent_DragFile(QString Path)
{
	AddPixmap(QPixmap(Path));
}

void pe_qDisplayCollectionDialog::SLOT_DragEvent_DragUrl(QString url)
{
	m_Networkmanager->get(QNetworkRequest(QUrl(url)));
}

void pe_qDisplayCollectionDialog::SLOT_NewWork_URLDownloadCompleted(QNetworkReply *reply)
{
	if (reply->error() == QNetworkReply::NoError)
	{
		QPixmap pixmap;
		pixmap.loadFromData(reply->readAll());
		if (!pixmap.isNull())
		{
			AddPixmap(pixmap);
		}
		else
		{
			emit SIGNAL_onMessageError(QString("Url is not a picture file: ") + reply->url().toString());
		}
	}
	else
	{
		emit SIGNAL_onMessageError(QString("Url download failed from ") + reply->url().toString());
	}
}

void pe_qDisplayCollectionDialog::SLOT_PasteClipboardImage()
{
	QPixmap pixmap = QApplication::clipboard()->pixmap();
	if (!pixmap.isNull())
	{
		AddPixmap(pixmap);
	}
}

