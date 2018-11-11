#include "pe_qdisplaydialog.h"

pe_qDisplayDialog::pe_qDisplayDialog(QWidget *parent)
	: QDialog(parent)
{
	m_bMoving = false;
	m_opt = 255;
	m_bCutting=false;
	m_ID=-1;
	m_bBorder=true;
	m_bDraggingOpt=false;
	m_Status=0;
	m_LastStatus=0;
	m_OffsetVector=QPoint(0,0);
	m_ColorHelperDialog = new pe_SubColorHelperDialog(this);
	m_BrushDialog=new pe_qBrushDialog(this);
	m_BrushDialog->setModal(false);
	m_bpainting=false;
	m_ColorHelperDialog->setVisible(false);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	this->setModal(false);
	
	m_cursor_move = QCursor(Qt::ArrowCursor);
	m_cursor_cut = QCursor(QPixmap(":/QSnapshotItem/Resources/cut.png"),0,0);
	m_cursor_pick = QCursor(QPixmap(":/QSnapshotItem/Resources/Pick.png"), 0, 31);
	m_BorderColor = QColor(0,0,0);

	connect(&m_CloseTimer, SIGNAL(timeout()), this, SLOT(SLOT_ClosingTimerAnimation()));
	connect(&m_OpeningTimer, SIGNAL(timeout()), this, SLOT(SLOT_OpeningTimerAnimation()));

	this->setCursor(m_cursor_move);

	//////////////////////////////////////////////////////////////////////////
	//Shortcut Action
	//////////////////////////////////////////////////////////////////////////
	QAction *ActionCut = new QAction(this);
	ActionCut->setShortcut(QKeySequence(Qt::SHIFT+Qt::Key_C));
	connect(ActionCut, SIGNAL(triggered()), this,SLOT(SLOT_Shortcut_CutMode()));
	this->addAction(ActionCut);

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

	//////////////////////////////////////////////////////////////////////////
	//Menu
	m_menu = new QMenu(this);

	m_Menu_Close = new QAction(this);
	m_Menu_Cut = new QAction(this);
	m_Menu_Pick = new QAction(this);
	m_Menu_CopyCurrent = new QAction(this);
	m_Menu_CopySource = new QAction(this);
	m_Menu_AlphaAdd = new QAction(this); 
	m_Menu_AlphaSubtract = new QAction(this);
	m_Menu_Restore = new QAction(this);
	m_Menu_Save = new QAction(this);
	m_Menu_CollapseAll = new QAction(this);
	m_Menu_DeleteThis = new QAction(this);
	m_Menu_MiniRestore = new QAction(this);
	m_Menu_Flip_Horizontal=new QAction(this);
	m_Menu_Flip_Vertical=new QAction(this);
	m_Menu_Pen=new QAction(this);

	m_Menu_MiniRestore->setText(tr("Restore"));
	m_Menu_DeleteThis->setText(tr("Delete this"));
	m_Menu_Close->setText(tr("Close (ESC)"));
	m_Menu_Cut->setText(tr("Clipper Mode (SHIFT+C)"));
	m_Menu_Pick->setText(tr("Pick Color Mode (ALT|Ctrl+P)"));
	m_Menu_CollapseAll->setText(tr("Collapse all"));
	m_Menu_CopyCurrent->setText(tr("Copy current image to clipboard (Ctrl+X)"));
	m_Menu_CopySource->setText((tr("Copy source image to clipboard (Ctrl+C)")));
	m_Menu_Restore->setText(tr("Restore (Ctrl+Z)"));
	m_Menu_Save->setText(tr("Save to (Ctrl+S)"));
	m_Menu_AlphaAdd->setText(tr("Increase opacity (Ctrl+Up)"));
	m_Menu_AlphaSubtract->setText(tr("Decrease opacity (Ctrl+Down)"));
	m_Menu_Flip_Horizontal->setText(tr("Flip Horizontal"));
	m_Menu_Flip_Vertical->setText(tr("Flip Vertical"));
	m_Menu_Pen->setText(tr("Pen"));

	connect(m_Menu_Close, SIGNAL(triggered()), this, SLOT(SLOT_Menu_Close()));
	connect(m_Menu_Cut, SIGNAL(triggered()), this, SLOT(SLOT_Menu_Cut()));
	connect(m_Menu_Flip_Horizontal, SIGNAL(triggered()), this, SLOT(SLOT_Menu_Flip_H()));
	connect(m_Menu_Flip_Vertical, SIGNAL(triggered()), this, SLOT(SLOT_Menu_Flip_V()));
	connect(m_Menu_Pen, SIGNAL(triggered()), this, SLOT(SLOT_Menu_Pen()));
	connect(m_Menu_Pick, SIGNAL(triggered()), this, SLOT(SLOT_Menu_Pick()));
	connect(m_Menu_CopyCurrent, SIGNAL(triggered()), this, SLOT(SLOT_Menu_CopyCurrent()));
	connect(m_Menu_CopySource, SIGNAL(triggered()), this, SLOT(SLOT_Menu_CopySource()));
	connect(m_Menu_AlphaAdd, SIGNAL(triggered()), this, SLOT(SLOT_Menu_AlphaAdd()));
	connect(m_Menu_AlphaSubtract, SIGNAL(triggered()), this, SLOT(SLOT_Menu_AlphaSub()));
	connect(m_Menu_Restore, SIGNAL(triggered()), this, SLOT(SLOT_Menu_Restore()));
	connect(m_Menu_Save, SIGNAL(triggered()), this, SLOT(SLOT_Menu_Save()));
	connect(m_Menu_CollapseAll, SIGNAL(triggered()), this, SLOT(SLOT_Menu_CollapseAll()));
	connect(m_Menu_DeleteThis, SIGNAL(triggered()), this, SLOT(SLOT_Menu_DeleteThis()));
	connect(m_Menu_MiniRestore, SIGNAL(triggered()), this, SLOT(SLOT_Menu_MiniRestore()));

}

pe_qDisplayDialog::~pe_qDisplayDialog()
{
}

void pe_qDisplayDialog::EnterDragOpacity()
{
	m_bDraggingOpt = true;
	setWindowOpacity(0.5);
}

void pe_qDisplayDialog::ExitDragOpacity()
{
	if (m_bDraggingOpt!=true)
	{
		return;
	}
	m_bDraggingOpt = false;
	SetOpacity(m_opt);
}

void pe_qDisplayDialog::SnapshotDisplayImage(QRect Region, QPixmap& Pixmap)
{
	m_Pixmap = Pixmap;
	m_RenderPixmap = m_Pixmap;
	m_PixmapResSize = QSize(Region.width(), Region.height());
	m_DialogLastSize = m_PixmapResSize;
	setFixedSize(Region.width(), Region.height());
	ShowDirectly();
}

void pe_qDisplayDialog::OpenDisplayImage(QRect Region, QPixmap& Pixmap)
{
	m_Pixmap = Pixmap;
	m_RenderPixmap = m_Pixmap;
	m_PixmapResSize = QSize(Region.width(), Region.height());
	m_DialogLastSize = m_PixmapResSize;
	ShowAction();
}

void pe_qDisplayDialog::SnipCacheForPickColor()
{
	m_bBorder = false;
	update();
	QPixmap pixmap = QPixmap::grabWidget(this, rect());
	m_CacheImage = pixmap.toImage();
	m_bBorder = true;
	update();
}

void pe_qDisplayDialog::ShowDirectly()
{
	this->setFixedSize(m_DialogLastSize);
	emit SIGNAL_onOpened(m_ID);
	show();
}

void pe_qDisplayDialog::CloseDirectly()
{
	emit SIGNAL_onClosed(m_ID);
	hide();
}

void pe_qDisplayDialog::SetShowSize(QSize size)
{
	if (size==this->size())
	{
		return;
	}
	m_DialogLastSize = size;
	move(this->pos() + QPoint(0,height()/2));
	hide();
	ShowAction();
}

void pe_qDisplayDialog::RestoreDisplay()
{
	m_RenderPixmap = m_Pixmap;
	SetShowSize(m_PixmapResSize);
	m_Status = DISPLAYDIALOG_STATUS_DRAGMODE;
	update();
}

void pe_qDisplayDialog::RestoreMini()
{
	this->setFixedSize(m_NormalLastSize);
	m_Status = DISPLAYDIALOG_STATUS_DRAGMODE;
	update();
}

void pe_qDisplayDialog::RestoreMoved()
{
	move(m_MovingStartPoint);
}

bool pe_qDisplayDialog::ShowAction()
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

bool pe_qDisplayDialog::CloseAction()
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

void pe_qDisplayDialog::CloseDialog()
{
	this->close();
	emit SIGNAL_onClosed(m_ID);
}

void pe_qDisplayDialog::SetOpacity(int opt)
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

void pe_qDisplayDialog::DoCut()
{
	
	int sx, sy, ex, ey;
	sx = m_CutStartPoint.x() < m_CutStopPoint.x() ? m_CutStartPoint.x() : m_CutStopPoint.x();
	sy = m_CutStartPoint.y() < m_CutStopPoint.y() ? m_CutStartPoint.y() : m_CutStopPoint.y();
	ex = m_CutStartPoint.x() > m_CutStopPoint.x() ? m_CutStartPoint.x() : m_CutStopPoint.x();
	ey = m_CutStartPoint.y() > m_CutStopPoint.y() ? m_CutStartPoint.y() : m_CutStopPoint.y();
	sx = sx < 0 ? 0 : sx;
	sy = sy < 0 ? 0 : sy;
	ex = ex > width() ? width() : ex;
	ey = ey > height() ? height() : ey;
	QRect CutRect(QPoint(sx, sy), QPoint(ex, ey));
	m_bBorder = false;
	update();
	m_RenderPixmap = QPixmap::grabWidget(this,CutRect);
	this->move(this->pos() + QPoint(sx, sy));
	this->setFixedSize(CutRect.width(), CutRect.height());
	m_bBorder = true;
	update();
}

void pe_qDisplayDialog::NormalMode()
{
	m_ColorHelperDialog->setVisible(false);
	m_Status = DISPLAYDIALOG_STATUS_DRAGMODE;
	this->setCursor(m_cursor_move);
}

void pe_qDisplayDialog::EnterCutMode()
{
	if (m_Status == DISPLAYDIALOG_STATUS_MINIMODE)
	{
		return;
	}

	m_Status = DISPLAYDIALOG_STATUS_CUTMODE;
	this->setCursor(m_cursor_cut);
}

void pe_qDisplayDialog::EnterPaintMode()
{
	if (m_Status == DISPLAYDIALOG_STATUS_MINIMODE)
	{
		return;
	}

	m_Status = DISPLAYDIALOG_STATUS_PAINTMODE;
	this->setCursor(Qt::CrossCursor);
}

void pe_qDisplayDialog::EnterMiniMode(QPoint ClickedPoint)
{
	m_NormalLastSize = this->size();
	m_Status = DISPLAYDIALOG_STATUS_MINIMODE;
	this->setFixedSize(QSize(DISPLAYDIALOG_MINI_SIZE, DISPLAYDIALOG_MINI_SIZE));
	move(ClickedPoint - QPoint(DISPLAYDIALOG_MINI_SIZE >> 1, DISPLAYDIALOG_MINI_SIZE >> 1));
	update();
}

void pe_qDisplayDialog::ExitCutMode()
{
	NormalMode();
}

void pe_qDisplayDialog::ExitPaintMode()
{
	NormalMode();
	m_BrushDialog->hide();
}

void pe_qDisplayDialog::EnterPickMode()
{
	if (m_Status == DISPLAYDIALOG_STATUS_MINIMODE)
	{
		return;
	}

	m_ColorHelperDialog->setVisible(false);
	SnipCacheForPickColor();
	m_Status = DISPLAYDIALOG_STATUS_PICKCOLORMODE;
	this->setCursor(m_cursor_pick);
}

void pe_qDisplayDialog::ExitPickMode()
{
	NormalMode();
}

void pe_qDisplayDialog::MoveColorHelperDialog(QPoint point)
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

void pe_qDisplayDialog::SetBorderColor(QColor clr)
{
	m_BorderColor = clr;
	update();
}

void pe_qDisplayDialog::ClipboardCurrentImage()
{
	m_bBorder = false;
	update();
	QApplication::clipboard()->setPixmap(QPixmap::grabWidget(this, rect()));
	m_bBorder = true;
}

void pe_qDisplayDialog::ClipboardSourceImage()
{
	QApplication::clipboard()->setPixmap(m_Pixmap);
}

void pe_qDisplayDialog::SaveImage()
{
	emit SIGNAL_onSave(m_ID);
}

void pe_qDisplayDialog::PushLastState()
{
	m_LastStatus = m_Status;
	NormalMode();
}

void pe_qDisplayDialog::RestoreLastState()
{
	switch (m_LastStatus)
	{
	case DISPLAYDIALOG_STATUS_DRAGMODE:
	break;
	case DISPLAYDIALOG_STATUS_CUTMODE:
		EnterCutMode();
	break;
	case DISPLAYDIALOG_STATUS_PICKCOLORMODE:
		EnterPickMode();
	break;
	default:
		break;
	}
	m_LastStatus = DISPLAYDIALOG_STATUS_DRAGMODE;
}

void pe_qDisplayDialog::ExecMenu()
{
	m_menu->clear();

	if (m_Status != DISPLAYDIALOG_STATUS_MINIMODE)
	{
		m_menu->addAction(m_Menu_Pick);
		m_menu->addAction(m_Menu_Cut);
		m_menu->addAction(m_Menu_AlphaAdd);
		m_menu->addAction(m_Menu_AlphaSubtract);
		m_menu->addAction(m_Menu_CollapseAll);
		m_menu->addAction(m_Menu_CopySource);
		m_menu->addAction(m_Menu_CopyCurrent);
		m_menu->addAction(m_Menu_Restore);
		m_menu->addAction(m_Menu_Save);
		m_menu->addAction(m_Menu_Flip_Horizontal);
		m_menu->addAction(m_Menu_Flip_Vertical);
		m_menu->addAction(m_Menu_Pen);
	}
	else
	{
		m_menu->addAction(m_Menu_MiniRestore);
	}
	
	m_menu->addAction(m_Menu_Close);
	m_menu->addAction(m_Menu_DeleteThis);
	//m_menu->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	m_menu->exec(QCursor::pos());
}

QPixmap pe_qDisplayDialog::GetCurrentDisplayPixmap()
{
	QPixmap pixmap;
	m_bBorder = false;
	update();
	pixmap=QPixmap::grabWidget(this, rect());
	m_bBorder = true;
	update();
	return pixmap;
}

QPixmap pe_qDisplayDialog::GetSourcePixmap()
{
	return m_Pixmap;
}

QColor pe_qDisplayDialog::GetWidgetColor(QPoint Point)
{
	return m_CacheImage.pixel(Point);
}

void pe_qDisplayDialog::paintEvent(QPaintEvent *event)
{

	if (m_Status == DISPLAYDIALOG_STATUS_MINIMODE)
	{
		QPainter pt(this);
		pt.drawPixmap(QRect(0, 0, width(), height()), m_Pixmap);
	}
	else if (!m_RenderPixmap.isNull())
	{
		QPainter pt(this);
		pt.drawPixmap(QRect(0, 0, width(), height()), m_RenderPixmap);
	}

	
	switch (m_Status)
	{
	case  DISPLAYDIALOG_STATUS_DRAGMODE:
	{
	}
	break;

	case DISPLAYDIALOG_STATUS_MINIMODE:
	{

	}
	break;

	case  DISPLAYDIALOG_STATUS_CUTMODE:
	{
		if (m_bCutting) {
			QPainter pt(this);
			int sx, sy, ex, ey;
			sx = m_CutStartPoint.x() < m_CutStopPoint.x() ? m_CutStartPoint.x() : m_CutStopPoint.x();
			sy = m_CutStartPoint.y() < m_CutStopPoint.y() ? m_CutStartPoint.y() : m_CutStopPoint.y();
			ex = m_CutStartPoint.x() > m_CutStopPoint.x() ? m_CutStartPoint.x() : m_CutStopPoint.x();
			ey = m_CutStartPoint.y() > m_CutStopPoint.y() ? m_CutStartPoint.y() : m_CutStopPoint.y();
			sx = sx < 0 ? 0 : sx;
			sy = sy < 0 ? 0 : sy;
			ex = ex > width() ? width() : ex;
			ey = ey > height() ? height() : ey;
			
			
			QRect CutRect(QPoint(sx, sy), QPoint(ex, ey));
			QPen  pen(QColor(0x0fff0f));
			pen.setWidth(2);
			pt.setPen(pen);
			pt.drawRect(CutRect);
		}
	}
	break;
	case DISPLAYDIALOG_STATUS_PICKCOLORMODE:
	{

	}
	break;
	default:
		break;
	}
	if (m_bBorder) {
		QPainter pt(this);
		QPen bPen(m_BorderColor);
		if (isActiveWindow())
		{
			bPen.setColor(QColor(0, 0, 255));
		}
		pt.setPen(bPen);
		bPen.setWidth(4);
		pt.drawRect(QRect(0,0,width()-1,height()-1));
		
	}
}

void pe_qDisplayDialog::mousePressEvent(QMouseEvent *event)
{
	if(!this->isActiveWindow())
		this->activateWindow();
	float scale;
	switch (m_Status)
	{
	case DISPLAYDIALOG_STATUS_DRAGMODE:
	case DISPLAYDIALOG_STATUS_MINIMODE:
	{
		switch (event->button()) {
		case Qt::LeftButton:
			
			m_bMoving = true;
			m_MovingStartPoint = this->pos();
			m_OffsetVector = event->globalPos() - this->frameGeometry().topLeft();
			break;
		case Qt::RightButton:
			ExecMenu();
			break;
		}
	}
	break;
	case DISPLAYDIALOG_STATUS_PAINTMODE:
		{
			switch (event->button()) {
			case Qt::LeftButton:
				scale=m_RenderPixmap.height()*1.0/this->height();
				m_lastDrawPoint = event->pos()*scale;
				m_bpainting=true;
				break;
			case Qt::RightButton:
				ExitPaintMode();
				break;
			}
		}
		break;

	case DISPLAYDIALOG_STATUS_CUTMODE:
	{
		if (!m_bCutting) {
			if (event->button() == Qt::LeftButton)
			{
				m_bCutting = true;
				m_CutStartPoint = event->pos();
				m_CutStopPoint = m_CutStartPoint;
			}
			if (event->button()==Qt::RightButton)
			{
				ExitCutMode();
			}
		}
	}
	break;
	case  DISPLAYDIALOG_STATUS_PICKCOLORMODE:
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

void pe_qDisplayDialog::mouseMoveEvent(QMouseEvent *event) {
	switch (m_Status)
	{
	case DISPLAYDIALOG_STATUS_DRAGMODE:
	case DISPLAYDIALOG_STATUS_MINIMODE:
	{
		if (m_bMoving)
		{
			this->move(event->globalPos() - m_OffsetVector);
			emit SIGNAL_onMoving(event->globalPos(), this);
		}
	}
	break;
	case DISPLAYDIALOG_STATUS_CUTMODE:
	{
		if (m_bCutting)
		{
			m_CutStopPoint = event->pos();
			update();
		}
	}
	break;

	case DISPLAYDIALOG_STATUS_PAINTMODE:
	{
		QImage image=m_RenderPixmap.toImage();
		QPainter painter(&image);
		QPen pen(m_BrushDialog->GetColor());
		float scale=m_RenderPixmap.height()*1.0/this->height();
		pen.setWidthF(m_BrushDialog->GetLineWidth());
		painter.setPen(pen);
		painter.drawLine(m_lastDrawPoint,event->pos()*scale);
		m_lastDrawPoint=event->pos()*scale;
		m_RenderPixmap=QPixmap::fromImage(image);
		this->update();
	}
	break;
	case  DISPLAYDIALOG_STATUS_PICKCOLORMODE:
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

void pe_qDisplayDialog::mouseDoubleClickEvent(QMouseEvent * event)
{
	if (m_Status==DISPLAYDIALOG_STATUS_MINIMODE)
	{
		RestoreMini();
		return;
	}

	if (m_Status== DISPLAYDIALOG_STATUS_DRAGMODE)
	{
		EnterMiniMode(event->globalPos());
		return;
	}
	
}

void pe_qDisplayDialog::keyPressEvent(QKeyEvent *event)
{
	if (event->key()==Qt::Key_Space)
	{
		PushLastState();
	}
	if (event->key()==Qt::Key_Alt)
	{
		EnterPickMode();
	}
	if (event->key()==Qt::Key_Escape)
	{
		switch (m_Status)
		{
		case DISPLAYDIALOG_STATUS_DRAGMODE:
		case DISPLAYDIALOG_STATUS_MINIMODE:
			CloseAction();
			break;
		case DISPLAYDIALOG_STATUS_CUTMODE:
			ExitCutMode();
			break;
		case DISPLAYDIALOG_STATUS_PICKCOLORMODE:
			ExitPickMode();
			break;
		case DISPLAYDIALOG_STATUS_PAINTMODE:
			ExitPaintMode();
			break;
		}
	}
}

void pe_qDisplayDialog::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Alt)
	{
		if (m_Status == DISPLAYDIALOG_STATUS_PICKCOLORMODE)
			ExitPickMode();
	}
	if (event->key() == Qt::Key_Space)
	{
		RestoreLastState();
	}
}

void pe_qDisplayDialog::mouseReleaseEvent(QMouseEvent *event)
{
	switch (m_Status)
	{
	case DISPLAYDIALOG_STATUS_DRAGMODE:
	case DISPLAYDIALOG_STATUS_MINIMODE:
	{
		if (event->button() == Qt::LeftButton)
		{
			m_bMoving = false;
			this->releaseMouse();
			this->setCursor(QCursor(Qt::ArrowCursor));
			ExitDragOpacity();
			emit SIGNAL_onDraggedDone(event->globalPos(), this);
		}
	}
	break;

	case DISPLAYDIALOG_STATUS_PAINTMODE:
		{
			if (m_bpainting)
			{
				m_bpainting=false;
			}
		}
		break;
	case DISPLAYDIALOG_STATUS_CUTMODE:
		{
			if (event->button() == Qt::LeftButton)
			{
				if (m_bCutting)
				{
					m_CutStopPoint = event->pos();
				}
				m_bCutting = false;
				update();
				DoCut();
				ExitCutMode();
				update();
				this->releaseMouse();
				this->setCursor(QCursor(Qt::ArrowCursor));
			}
		}
		break;
	case DISPLAYDIALOG_STATUS_PICKCOLORMODE:
	{

	}
	break;
	}
}

void pe_qDisplayDialog::wheelEvent(QWheelEvent *event)
{
	if (m_Status== DISPLAYDIALOG_STATUS_MINIMODE)
	{
		return;
	}
	float IncrementScale = event->delta()/2000.f;
	int Incw = width()*IncrementScale;
	int Inch = height()*IncrementScale;
	QPoint PosOft = QPoint(Incw/2,Inch/2);
	this->move(this->pos()-PosOft);
	this->setFixedSize(Incw+width(), Inch+height());
}



void pe_qDisplayDialog::resizeEvent(QResizeEvent *event)
{
	switch (m_Status)
	{
	case DISPLAYDIALOG_STATUS_PICKCOLORMODE:
	{
		SnipCacheForPickColor();
	}
	break;
	default:
		break;
	}
	
}


void pe_qDisplayDialog::enterEvent(QEvent *event)
{
	update();
}

void pe_qDisplayDialog::leaveEvent(QEvent *event)
{
	update();
}


void pe_qDisplayDialog::SLOT_OpeningTimerAnimation()
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

void pe_qDisplayDialog::SLOT_ClosingTimerAnimation()
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

void pe_qDisplayDialog::SLOT_Shortcut_Save()
{
	SaveImage();
}

void pe_qDisplayDialog::SLOT_Shortcut_CutMode()
{
	switch (m_Status)
	{
	case DISPLAYDIALOG_STATUS_DRAGMODE:
		EnterCutMode();
		break;
	case DISPLAYDIALOG_STATUS_CUTMODE:
		ExitCutMode();
		break;
	case DISPLAYDIALOG_STATUS_PICKCOLORMODE:
		ExitPickMode();
		ExitCutMode();
		break;
	}
}

void pe_qDisplayDialog::SLOT_Shortcut_Restore()
{
	if (m_Status!=DISPLAYDIALOG_STATUS_MINIMODE)
	{
		ExitCutMode();
		ExitPickMode();
		RestoreDisplay();
	}
}

void pe_qDisplayDialog::SLOT_Shortcut_ClipboardCurrentImage()
{
	ClipboardCurrentImage();
}

void pe_qDisplayDialog::SLOT_Shortcut_ClipboardSourceImage()
{
	ClipboardSourceImage();
}

void pe_qDisplayDialog::SLOT_Menu_Close()
{
	CloseAction();
}

void pe_qDisplayDialog::SLOT_Menu_Cut()
{
	if (m_Status != DISPLAYDIALOG_STATUS_MINIMODE)
	{
		EnterCutMode();
	}
}



void pe_qDisplayDialog::SLOT_Menu_Flip_H()
{
	if (!m_RenderPixmap.isNull())
	{
		QImage img=m_RenderPixmap.toImage();
		img=img.mirrored(true,false);
		m_RenderPixmap=QPixmap::fromImage(img);
		this->update();
	}
}

void pe_qDisplayDialog::SLOT_Menu_Flip_V()
{
	if (!m_RenderPixmap.isNull())
	{
		QImage img=m_RenderPixmap.toImage();
		img=img.mirrored(false,true);
		m_RenderPixmap=QPixmap::fromImage(img);
		this->update();
	}
}

void pe_qDisplayDialog::SLOT_Menu_Pen()
{
	EnterPaintMode();
	m_BrushDialog->show();
}

void pe_qDisplayDialog::SLOT_Menu_Pick()
{
	if (m_Status != DISPLAYDIALOG_STATUS_MINIMODE)
	{
		EnterPickMode();
	}
}

void pe_qDisplayDialog::SLOT_Menu_CopyCurrent()
{
	ClipboardCurrentImage();
}

void pe_qDisplayDialog::SLOT_Menu_CopySource()
{
	ClipboardSourceImage();
}

void pe_qDisplayDialog::SLOT_Menu_AlphaAdd()
{
	SIGNAL_onAlphaIncreaseChanged(m_ID,10);
}

void pe_qDisplayDialog::SLOT_Menu_AlphaSub()
{
	SIGNAL_onAlphaIncreaseChanged(m_ID,-10);
}

void pe_qDisplayDialog::SLOT_Menu_Restore()
{
	RestoreDisplay();
}

void pe_qDisplayDialog::SLOT_Menu_Save()
{
	SaveImage();
}

void pe_qDisplayDialog::SLOT_Menu_CollapseAll()
{
	emit SIGNAL_onCollapseAll();
}

void pe_qDisplayDialog::SLOT_Menu_DeleteThis()
{
	emit SIGNAL_onDeleteThis(m_ID);
}

void pe_qDisplayDialog::SLOT_Menu_MiniRestore()
{
	this->RestoreMini();
}

