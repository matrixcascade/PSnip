#ifndef PE_QDISPLAYDIALOG_H
#define PE_QDISPLAYDIALOG_H

//////////////////////////////////////////////////////////////////////////
//Color helper dialog
//////////////////////////////////////////////////////////////////////////
#include "pe_ColorHelperDialog.h"


#define  DISPLAYDIALOG_STATUS_DRAGMODE		0
#define  DISPLAYDIALOG_STATUS_CUTMODE		1
#define  DISPLAYDIALOG_STATUS_PICKCOLORMODE	2
#define  DISPLAYDIALOG_STATUS_MINIMODE		3

#define  DISPLAYDIALOG_MINI_SIZE			64

class pe_qDisplayDialog : public QDialog
{
	Q_OBJECT

public:
	pe_qDisplayDialog(QWidget *parent);
	~pe_qDisplayDialog();
	
	void EnterDragOpacity();
	void ExitDragOpacity();
	void SnapshotDisplayImage(QRect Region,QPixmap& Pixmap);
	void OpenDisplayImage(QRect Region,QPixmap& Pixmap);
	void SnipCacheForPickColor();
	void ShowDirectly();
	void CloseDirectly();
	void SetShowSize(QSize size);
	void RestoreDisplay();
	void RestoreMini();
	void RestoreMoved();
	bool ShowAction();
	bool CloseAction();
	void CloseDialog();
	void SetOpacity(int);
	void DoCut();
	void NormalMode();
	void EnterCutMode();
	void EnterMiniMode(QPoint ClickedPoint);
	void ExitCutMode();
	void EnterPickMode();
	void ExitPickMode();
	void MoveColorHelperDialog(QPoint);
	void SetBorderColor(QColor);
	void ClipboardCurrentImage();
	void ClipboardSourceImage();
	void SaveImage();
	void PushLastState();
	void RestoreLastState();
	void ExecMenu();
	QPixmap GetCurrentDisplayPixmap();
	QPixmap GetSourcePixmap();
	QColor  GetWidgetColor(QPoint Point);
	unsigned char GetOpacity() { return m_opt; }
	void SetId(int ID) { m_ID = ID; }
	void paintEvent(QPaintEvent *event)					override;
	void mousePressEvent(QMouseEvent *event)			override;
	void mouseReleaseEvent(QMouseEvent *event)			override;
	void mouseMoveEvent(QMouseEvent *event)				override;
	void mouseDoubleClickEvent(QMouseEvent *event)		override;
	void keyPressEvent(QKeyEvent *event)				override;
	void keyReleaseEvent(QKeyEvent *event)				override;
	void wheelEvent(QWheelEvent *event)					override;
	void resizeEvent(QResizeEvent *event)				override;
	void enterEvent(QEvent *event)						override;
	void leaveEvent(QEvent *event)						override;
	public slots:
	void SLOT_OpeningTimerAnimation();
	void SLOT_ClosingTimerAnimation();
	void SLOT_Shortcut_Save();;
	void SLOT_Shortcut_CutMode();
	void SLOT_Shortcut_Restore();
	void SLOT_Shortcut_PickColor() {};
	void SLOT_Shortcut_ClipboardCurrentImage();
	void SLOT_Shortcut_ClipboardSourceImage();
	QSize GetLastDialogSize() { return m_DialogLastSize; }
	
	void SLOT_Menu_Close();
	void SLOT_Menu_Cut();
	void SLOT_Menu_Pick();
	void SLOT_Menu_CopyCurrent();
	void SLOT_Menu_CopySource();
	void SLOT_Menu_AlphaAdd();
	void SLOT_Menu_AlphaSub();
	void SLOT_Menu_Restore();
	void SLOT_Menu_Save();
	void SLOT_Menu_CollapseAll();
	void SLOT_Menu_DeleteThis();
	void SLOT_Menu_MiniRestore();
signals:
	void SIGNAL_onClosed(int);
	void SIGNAL_onOpened(int);
	void SIGNAL_onSave(int);
	void SIGNAL_onCollapseAll();
	void SIGNAL_onAlphaIncreaseChanged(int,int);
	void SIGNAL_onMoving(QPoint, pe_qDisplayDialog*);
	void SIGNAL_onDraggedDone(QPoint, pe_qDisplayDialog*);
	void SIGNAL_onDeleteThis(int);

public:
	int		m_opt;
	int     m_ID;
	QPixmap m_Pixmap;
	bool    m_bMoving, m_bCutting, m_bBorder, m_bDraggingOpt;
	QColor  m_BorderColor;
	int     m_Status, m_LastStatus;
	QPoint  m_MovingStartPoint;
	QPoint  m_CutStartPoint;
	QPoint  m_CutStopPoint;
	QPoint  m_OffsetVector;
	QSize   m_PixmapResSize;
	QSize   m_DialogLastSize;
	QSize   m_NormalLastSize;
	QTimer  m_OpeningTimer;
	QTimer  m_CloseTimer;
	QCursor m_cursor_move,m_cursor_cut,m_cursor_pick;
	QPixmap m_RenderPixmap;
	pe_SubColorHelperDialog *m_ColorHelperDialog;
	QImage  m_CacheImage;
	float   m_LastOpacity;
	QMenu   *m_menu;
	QAction *m_Menu_CollapseAll, *m_Menu_MiniRestore,*m_Menu_Close, *m_Menu_Cut, *m_Menu_Pick, *m_Menu_CopyCurrent, *m_Menu_AlphaAdd;
	QAction *m_Menu_AlphaSubtract, *m_Menu_Restore, *m_Menu_Save, *m_Menu_CopySource,*m_Menu_DeleteThis;

};

#endif // PE_QDISPLAYDIALOG_H
