#ifndef QSNAPSHOTITEM_H
#define QSNAPSHOTITEM_H

#include <QWidget>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QPainter>
#include <QMargins>
#include <QMouseEvent>
#include <QMatrix>
#include <QEvent>
#include <QTimer>

#include "modifybutton.h"

#define  QSNAPSHOTITEM_CURSOR_OFT   5
#define  QSNAPSHOTITEM_CURSOR_STEP  8
#define  QSNAPSHOTITEM_MINIPIXMAP_SIZE_WIDTH 64
#define  QSNAPSHOTITEM_MINIPIXMAP_SIZE_HEIGHT 64
#define  QSNAPSHOTITEM_MINIPIXMAP_SIZE_XOFFSET 16
class QSnapshotItem : public QWidget
{
	Q_OBJECT

public:
	QSnapshotItem(QWidget *parent = 0);
	~QSnapshotItem();

	void paintEvent(QPaintEvent *e)			override;
	void mouseMoveEvent(QMouseEvent *e)		override;
	void mousePressEvent(QMouseEvent * e)		override;
	void mouseReleaseEvent(QMouseEvent *e)		override;
	void leaveEvent(QEvent *e)				override;
	void SetMiniPixmap(QPixmap pixmap);
	//Function
	bool IsOpenMode() { return m_bOpenMode; }
	void SetOpenMode(bool b) { m_bOpenMode = b; update(); }
	void SetID(int ID) { m_ID = ID; }
	void SetName(QString Name) { label_diaplayname->SetText(Name); }
	QString GetName() {	return label_diaplayname->GetText();}
	bool IsCollector() { return m_IsCollector; }
	void AsCollectionItem();
	//various
	QHBoxLayout *horizontalLayout_7;
	QVBoxLayout *verticalLayout_3;
	QHBoxLayout *horizontalLayout_8;
	QModifyButton *label_diaplayname;
	QSpacerItem *horizontalSpacer_6;
	QHBoxLayout *horizontalLayout_9;
	QPushButton *pushButton_delete;
	QPushButton *pushButton_export;
	QPushButton *pushButton_close;
	QPushButton *pushButton_restore;
	QPushButton *pushButton_cut;
	QPushButton *pushButton_save;
	QPushButton *pushButton_scale;
	QPushButton *pushButton_pick;
	QSlider		*horizontalSlider;
	QSpacerItem *horizontalSpacer_7;
	QTimer   m_AnimationOpenTimer;
	QTimer   m_AnimationCloseTimer;
	QTimer   m_AnimationCursorTimer;
	QColor   m_CurrColor;
	QColor   m_DestColor;
	bool	 m_bOpenMode;
	bool     m_bMouseCursorOverContain;
	bool     m_IsCollector;
	int      m_OffsetMargins;
	int      m_CursorCurXOft;
	int      m_CursorDestOft;
	QPixmap  m_CursorPixmapRight;
	QPixmap  m_CursorPixmapLeft;
	QPixmap  m_MiniPixmap;

	int      m_ID;
public slots:
	void SLOT_onButtonOpen();
	void SLOT_onButtonClose();
	void SLOT_onButtonDelete();
	void SLOT_onButtonCut();
	void SLOT_onButtonScale();
	void SLOT_onButtonPick();
	void SLOT_onButtonSave();
	void SLOT_onButtonRestore();
	void SLOT_onSliderChanged(int);

	void SLOT_TimerAnimationOpen();
	void SLOT_TimerAnimationClose();
	void SLOT_TimerAnimationCursor();
signals:
	void SIGNAL_onMousedown(int);
	void SIGNAL_onOpen(int);
	void SIGNAL_onClose(int);
	void SIGNAL_onDelete(int);
	void SIGNAL_onCut(int);
	void SIGNAL_onScale(int);
	void SIGNAL_onPick(int);
	void SIGNAL_onSave(int);
	void SIGNAL_onRestore(int);
	void SIGNAL_onAlphaChange(int,int);
	void SIGNAL_onMouseFocus(int);
	void SIGNAL_onMouseLostFocus(int);
};



#endif // QSNAPSHOTITEM_H
