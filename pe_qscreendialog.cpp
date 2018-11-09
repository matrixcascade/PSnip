#include "pe_qscreendialog.h"

pe_qScreenDialog::pe_qScreenDialog(QDialog *parent)
	: QDialog(parent)
{
	setModal(true);
	QPixmap imgCursorbl(":/ScreenShot/Resources/bl.png");
	QPixmap imgCursorbr(":/ScreenShot/Resources/br.png");
	QPixmap imgCursortl(":/ScreenShot/Resources/tl.png");
	QPixmap imgCursortr(":/ScreenShot/Resources/tr.png");
	m_cursor_bl = QCursor(imgCursorbl, 31, 0);
	m_cursor_br = QCursor(imgCursorbr, 0, 0);
	m_cursor_tl=  QCursor(imgCursortl, 31, 31);
	m_cursor_tr= QCursor(imgCursortr, 0, 31);

}

pe_qScreenDialog::~pe_qScreenDialog()
{

}

void pe_qScreenDialog::paintEvent(QPaintEvent *event)
{
	drawGreyBackground();
	drawSelectArea();
}

void pe_qScreenDialog::keyPressEvent(QKeyEvent *event)
{
	if (event->key()==Qt::Key_Escape)
	{
		emit SIGNAL_OnCaptureExit();
		hide();
	}
}

void pe_qScreenDialog::mousePressEvent(QMouseEvent *event)
{
	if(!this->isActiveWindow())
		this->activateWindow();
	if (!m_IsDragging) {
		if (event->button() == Qt::LeftButton)
		{
			m_IsDragging = true;
			m_StartPoint = event->pos();
		}
		else if (event->button() == Qt::RightButton)
		{
			m_IsDragging = false;
			emit SIGNAL_OnCaptureExit();
			hide();
			updateCursor();
			update();
		}
	}
}

void pe_qScreenDialog::mouseMoveEvent(QMouseEvent *event)
{
	if (m_IsDragging)
	{
		m_EndPoint = event->pos();
		updateCursor();
		update();
	}
}

void pe_qScreenDialog::mouseReleaseEvent(QMouseEvent *event)
{
	if (m_IsDragging)
	{
		m_EndPoint = event->pos();
		m_IsDragging = false;
		update();
		if (m_SRect.width() <= 8 || m_SRect.height() <= 8)
		{
			return;
		}
		if (m_SRect.width() + m_SRect.height() <= 32)
		{
			return;
		}
		m_FinalPixmap = getSnapshotPixmap();
		switch (m_SelectType)
		{
		case SCREENDIALOG_SELECTMODE_SNIP:
			emit SIGNAL_OnCaptureDone();
			break;
		case SCREENDIALOG_SELECTMODE_REGION:
			emit SIGNAL_OnSelectDone();
			break;
		default:
			break;
		}
		
		emit SIGNAL_OnCaptureExit();
		hide();
		updateCursor();
	}
	update();
}

void pe_qScreenDialog::drawGreyBackground()
{
	QPainter pt(this);
	pt.drawImage(QPoint(0, 0), m_GreyImage);

}

void pe_qScreenDialog::drawSelectArea()
{
	QPainter pt(this);
	int sx, sy, ex, ey;
	sx = m_StartPoint.x() < m_EndPoint.x() ? m_StartPoint.x() : m_EndPoint.x();
	sy= m_StartPoint.y() < m_EndPoint.y() ? m_StartPoint.y() : m_EndPoint.y();
	ex = m_StartPoint.x() > m_EndPoint.x() ? m_StartPoint.x() : m_EndPoint.x();
	ey = m_StartPoint.y() > m_EndPoint.y() ? m_StartPoint.y() : m_EndPoint.y();

	m_SRect = QRect(QPoint(sx,sy),QPoint(ex,ey));
	if (m_SelectType == SCREENDIALOG_SELECTMODE_SNIP) {
		pt.drawPixmap(QPoint(sx, sy), m_ResScreenPixmap, m_SRect);
		QPen  pen(QColor(0xff8c00));
		pen.setWidth(2);
		pt.setPen(pen);
		if (m_IsDragging)
		{
			pt.drawRect(m_SRect);
			return;
		}
	}
	else if (m_SelectType == SCREENDIALOG_SELECTMODE_REGION)
	{
		if (m_IsDragging)
		{
			pt.fillRect(m_SRect,QBrush(QColor(0,0,0,128)));
			return;
		}
		QPen  pen(QColor(0xff8c00));
		pen.setWidth(2);
		pt.setPen(pen);
		if (m_IsDragging)
		{
			pt.drawRect(m_SRect);
			return;
		}
	}
}

void pe_qScreenDialog::updateCursor()
{
	if (m_StartPoint.x()<m_EndPoint.x())
	{
		if (m_StartPoint.y()>m_EndPoint.y())
		{
			this->setCursor(m_cursor_bl);
		}
		else if (m_StartPoint.y() < m_EndPoint.y())
		{
			this->setCursor(m_cursor_tl);
		}
		else
		{
			this->setCursor(Qt::CrossCursor);
		}
	}
	else if(m_StartPoint.x()>m_EndPoint.x())
	{
		if (m_StartPoint.y() > m_EndPoint.y())
		{
			this->setCursor(m_cursor_br);
		}
		else if (m_StartPoint.y() < m_EndPoint.y())
		{
			this->setCursor(m_cursor_tr);
		}
		else
		{
			this->setCursor(Qt::CrossCursor);
		}
	}
	else
	{
		this->setCursor(Qt::CrossCursor);
	}
}

void pe_qScreenDialog::BeginSnip()
{
	m_IsDragging = false;
	m_ResScreenPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
	m_HelperPixmap = m_ResScreenPixmap;

	m_StartPoint = QPoint(0, 0);
	m_EndPoint = QPoint(0, 0);
	m_SRect = QRect(0, 0, 0, 0);

	this->showFullScreen();
	this->setCursor(Qt::CrossCursor);
	m_GreyImage = convertQPixmapToGreyQImage(m_ResScreenPixmap);
	m_SelectType = SCREENDIALOG_SELECTMODE_SNIP;
	this->activateWindow();
	emit SIGNAL_OnCaptureReady();
}

void pe_qScreenDialog::BeginSelectViewRegion()
{
	m_IsDragging = false;
	m_ResScreenPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
	m_HelperPixmap = m_ResScreenPixmap;

	m_StartPoint = QPoint(0, 0);
	m_EndPoint = QPoint(0, 0);
	m_SRect = QRect(0, 0, 0, 0);

	this->showFullScreen();
	this->setCursor(Qt::CrossCursor);
	m_GreyImage =m_ResScreenPixmap.toImage();
	m_SelectType = SCREENDIALOG_SELECTMODE_REGION;
	this->activateWindow();
	emit SIGNAL_OnCaptureReady();
}

QPixmap pe_qScreenDialog::getSnapshotPixmap()
{
	return QPixmap::grabWidget(this, m_SRect);
}

QImage pe_qScreenDialog::convertQPixmapToGreyQImage(QPixmap& rpm)
{
	QImage grayImage = rpm.toImage();
	if (grayImage.format()!=QImage::Format_RGB32)
	{
		return grayImage;
	}
	unsigned char *data = grayImage.bits();
	int width = grayImage.width();
	int height = grayImage.height();

	int bytePerLine = grayImage.bytesPerLine();
	int XRGBPerLine = bytePerLine / 4;
	unsigned char x,r, g, b,gery;
	int LineOft;
	for (int i = 0; i < height; i++)
	{
		LineOft = 0;
		for (int j=0;j<XRGBPerLine;j++)
		{
			x = *(data + 3+LineOft);
			r = *(data + 2+LineOft);
			g = *(data + 1+LineOft);
			b = *(data + 0+LineOft);
			gery = (r * 299 + g * 587 + b * 114) / 1000;
			data[3+LineOft] = x;
			data[2+LineOft] = gery;
			data[1+LineOft] = gery;
			data[0+LineOft] = gery;

			LineOft += 4;
		}
		data += bytePerLine;
	}
	return grayImage;
}

QRect pe_qScreenDialog::getRect()
{
	return m_SRect;
}

