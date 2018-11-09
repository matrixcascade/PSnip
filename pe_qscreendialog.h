#ifndef PE_QSCREENDIALOG_H
#define PE_QSCREENDIALOG_H

#include <QDialog>
#include <QshowEvent>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QCursor>

#define  SCREENDIALOG_SELECTMODE_SNIP		1
#define  SCREENDIALOG_SELECTMODE_REGION		2

class pe_qScreenDialog : public QDialog
{
	Q_OBJECT

public:
	pe_qScreenDialog(QDialog *parent);
	~pe_qScreenDialog();

	void paintEvent(QPaintEvent *event)			override;
	void keyPressEvent(QKeyEvent *event)		override;
	void mousePressEvent(QMouseEvent *event)	override;
	void mouseMoveEvent(QMouseEvent *event)		override;
	void mouseReleaseEvent(QMouseEvent *event)	override;

	void drawGreyBackground();
	void drawSelectArea();
	void updateCursor();
	void BeginSnip();
	void BeginSelectViewRegion();

	QPixmap getSnapshotPixmap();
	QImage  convertQPixmapToGreyQImage(QPixmap&);
	QRect  getRect();
signals:
	void SIGNAL_OnSelectDone();
	void SIGNAL_OnCaptureDone();
	void SIGNAL_OnCaptureExit();
	void SIGNAL_OnCaptureReady();
private:
	int     m_SelectType;
	bool	m_IsDragging;
	QPoint	m_StartPoint, m_EndPoint;
	QRect	m_SRect;

	QPixmap m_ResScreenPixmap, m_FinalPixmap, m_HelperPixmap;
	QImage  m_GreyImage;
	QCursor m_cursor_tl, m_cursor_tr, m_cursor_bl, m_cursor_br;
	
};

#endif // PE_QSCREENDIALOG_H
