#ifndef PE_QDISPLAYCOLLECTIONDIALOG_H
#define PE_QDISPLAYCOLLECTIONDIALOG_H

//////////////////////////////////////////////////////////////////////////
//Color helper dialog
//////////////////////////////////////////////////////////////////////////
#include "pe_ColorHelperDialog.h"
#include <vector>
#include <QMimeData>
#include <QImage>

//Network/////////////////////////////////////////////////////////////////
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUrl>

#define  DISPLAYCOLLECTIONDIALOG_STATUS_NORMALMODE		0
#define  DISPLAYCOLLECTIONDIALOG_STATUS_PICKCOLORMODE	1
#define  DISPLAYCOLLECTIONDIALOG_STATUS_ADJUSTMODE      2

#define  DISPLAYCOLLECTIONDIALOG_SCALE_MAXTIMES         8
#define  DISPLAYCOLLECTIONDIALOG_SCALE_MINITIMES        0.05
using namespace std;

struct CollectionPixmapInfo
{
	QPixmap ResPixmap;
	QPixmap RenderPixmap;
	QPoint  StartPoint;
	float   Scale;
};

class pe_qDisplayCollectionDialog : public QDialog
{
	Q_OBJECT

public:
	pe_qDisplayCollectionDialog(QWidget *parent);
	~pe_qDisplayCollectionDialog();
	void RemoveCurrent();
	void ScaleCurrentImage(float scale,bool AutoMoving=true);
	void SwitchNext();
	void SwitchPrevious();
	void RestoreCurrentDisplay();
	void AddPixmap(QPixmap);
	void AddCollectionPixInfo(CollectionPixmapInfo);
	void SetCurrentDisplayIndex(int);
	int  GetCurrentDisplayIndex();
	void SnipCacheForPickColor();
	void ShowDirectly();
	void CloseDirectly();
	void SetShowSize(QSize size);
	bool ShowAction();
	bool CloseAction();
	void CloseDialog();
	void SetOpacity(int);
	void NormalMode();
	void EnterPickMode();
	void ExitPickMode();
	void EnterAdjustMode();
	void ExitAdjustMode();
	void MoveColorHelperDialog(QPoint);
	void SetBorderColor(QColor);
	void ClipboardCurrentImage();
	void ClipboardSourceImage();
	void SaveImage();
	void PushLastState();
	void RestoreLastState();
	void ExecMenu();
	void EnterPreview(QPixmap PreviewPixmap);
	void ExitPreview();
	int  GetOpacity() { return m_opt; }
	QPixmap GetCurrentDisplayPixmap();
	QColor  GetWidgetColor(QPoint Point);


	//////////////////////////////////////////////////////////////////////////
	//Painter feature
	void PaintStandby();
	void PaintPreview();
	void PaintBoard();
	void PaintImage();

	void SetId(int ID) { m_ID = ID; }
	void paintEvent(QPaintEvent *event)					override;
	void mousePressEvent(QMouseEvent *event)			override;
	void mouseReleaseEvent(QMouseEvent *event)			override;
	void mouseMoveEvent(QMouseEvent *event)				override;
	void keyPressEvent(QKeyEvent *event)				override;
	void keyReleaseEvent(QKeyEvent *event)				override;
	void wheelEvent(QWheelEvent *event)					override;
	void resizeEvent(QResizeEvent *event)				override;
	void enterEvent(QEvent *event)						override;
	void leaveEvent(QEvent *event)						override;

	void dragEnterEvent(QDragEnterEvent *event) override;
	void dropEvent(QDropEvent *event)			override;
	void dragMoveEvent(QDragMoveEvent *event)   override;
	void dragLeaveEvent(QDragLeaveEvent *event) override;

	public slots:
	void SLOT_OpeningTimerAnimation();
	void SLOT_ClosingTimerAnimation();
	void SLOT_Shortcut_Next();
	void SLOT_Shortcut_Remove();
	void SLOT_Shortcut_PickColor() {};
	void SLOT_Shortcut_ClipboardCurrentImage();
	void SLOT_Shortcut_ClipboardSourceImage();
	void SLOT_Shortcut_Restore();
	QSize GetLastDialogSize() { return m_DialogLastSize; }
	
	void SLOT_Menu_Remove();
	void SLOT_Menu_Next();
	void SLOT_Menu_Previous();
	void SLOT_Menu_Close();
	void SLOT_Menu_Pick();
	void SLOT_Menu_CopyCurrent();
	void SLOT_Menu_CopySource();
	void SLOT_Menu_AlphaAdd();
	void SLOT_Menu_AlphaSub();
	void SLOT_Menu_Restore();
	void SLOT_Menu_Save();
	void SLOT_Menu_AdjustModeSwitch();
	void SLOT_Menu_CollapseAll();
	void SLOT_Menu_DeleteThis();

	void SLOT_DragEvent_DragImage(QPixmap);
	void SLOT_DragEvent_DragFile(QString);
	void SLOT_DragEvent_DragUrl(QString);

	void SLOT_NewWork_URLDownloadCompleted(QNetworkReply *reply);

	void SLOT_PasteClipboardImage();
signals:
	void SIGNAL_onClosed(int);
	void SIGNAL_onOpened(int);
	void SIGNAL_onSave(int);
	void SIGNAL_onAlphaIncreaseChanged(int,int);
	void SIGNAL_DropImageFile(QString);
	void SIGNAL_DropPixmap(QPixmap);
	void SIGNAL_DropURL(QString);
	void SIGNAL_onCollapseAll();
	void SIGNAL_onMessageError(QString);
	void SIGNAL_onDeleteThis(int);
public:
	//Network Download
	QNetworkAccessManager *m_Networkmanager;
	int     m_CurrentDisplayIndex;
	int		m_opt;
	int     m_ID;
	bool    m_bMouseMoving, m_bBorder;
	bool    m_bIsOnPreview;
	QPixmap m_GuiderPixmap;
	QPixmap m_PreviewPixmap;
	QColor  m_BorderColor;
	int     m_Status, m_LastStatus;
	QPoint  m_OffsetVector, m_lastUpdateVector;
	QSize   m_DialogLastSize;
	QTimer  m_OpeningTimer;
	QTimer  m_CloseTimer;
	QCursor m_cursor_move,m_cursor_pick;
	pe_SubColorHelperDialog *m_ColorHelperDialog;
	QImage  m_CacheImage;
	vector<CollectionPixmapInfo> m_Pixmaps;
	QMenu   *m_menu;
	QAction *m_Menu_Close, *m_Menu_Pick, *m_Menu_CopyCurrent, *m_Menu_AlphaAdd;
	QAction *m_Menu_AlphaSubtract, *m_Menu_Restore, *m_Menu_Next, *m_Menu_CopySource;
	QAction *m_Menu_Previous,*m_Menu_RemoveCurrent, *m_Menu_ClipboardPaste,*m_Menu_AdjustMode, *m_Menu_CollapseAll;
	QAction *m_Menu_DeleteThis;

}; 

#endif 
