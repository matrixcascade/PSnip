#ifndef PSNIP_H
#define PSNIP_H

#include <QObject>
#include <vector>
#include <QList>
#include <QMimeData>
#include <QFileInfo>
#include <QFileDialog>
#include <QSystemTrayIcon>
#include <QTranslator>

//////////////////////////////////////////////////////////////////////////
// PSnip data exchange center class
// Support:C++ 11 complier;Qt 5.3.1 framework
// 4/25/2016  Author:DBinary 
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Extern classes 
//-----------Plugins-------------//
#include "pe_snapshotlist.h"

//----------HotKey for windows----------------//
#include "qxtglobalshortcut/qxtglobal.h"
#include "qxtglobalshortcut/qxtglobalshortcut.h"

//----------Configuration manager-------------//
#include "pe_ConfigFile.h"

//----------Screen snipping dialog------------//
#include "pe_qscreendialog.h"

//----------Viewer collection dialog-----------//
#include "pe_qdisplayCollectiondialog.h"

//-----------Manager dialog--------------------//
#include "ui_pe_imageslistdialog.h"
#include "ui_pe_ConfigWidget.h"

//-----------Display dialog--------------------//
#include "pe_qdisplaydialog.h"

//----------Donate dialog----------------------//
#include "ui_pe_Donate.h"

//-----------URL Downloader--------------------//
#include "qURLDownloader.hpp"

//Network/////////////////////////////////////////////////////////////////
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUrl>

//-----------events----------------------------//
#include <QEvent>
#include <QDropEvent>

#include "Urlmon.h"
#include "Wininet.h"

#pragma comment(lib,"Urlmon.lib")
#pragma comment(lib,"Wininet.lib")

class QUrlDownloader : public QThread {
	Q_OBJECT
public:
	QUrlDownloader(QObject * parent = NULL);
	~QUrlDownloader();
	int Download(QString url, QString downloadFile);
	void run() override;
signals:
	void SIGNAL_DownloadCompleted();
private:
	QString m_url;
	QString m_downloadFile;
};

class pe_ManagerDialog :public QDialog
{
	Q_OBJECT
public:
	pe_ManagerDialog(QDialog *parent = NULL);
	
	~pe_ManagerDialog()
	{

	}
	void mouseMoveEvent(QMouseEvent *event)		override;
	void mousePressEvent(QMouseEvent *event)	override;
	void mouseReleaseEvent(QMouseEvent *event)	override;
	void paintEvent(QPaintEvent * event)		override;
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dropEvent(QDropEvent *event)			override;
	void dragMoveEvent(QDragMoveEvent *event)   override;
	void dragLeaveEvent(QDragLeaveEvent *event) override;
	void showEvent(QShowEvent *event)			override;
	void contextMenuEvent(QContextMenuEvent * event) override;
	void SetStaytoTop(bool Enable);
	void ExecMenu(QPoint);
	void ResetLanguage();
	bool m_bWindowStayOnTop;
	bool m_bMoving;
	QPoint m_OffsetVector;
	Ui_pe_imagesListDialog m_ImageDialog;
	Ui_pe_ConfigWidget  m_ConfigWidget;
	QMenu			*m_menu;
	QAction			*m_Menu_SaveProject, *m_Menu_OpenProject, *m_Menu_ClipboardPaste, *m_Menu_CollapseAll, *m_Menu_importFile;
	pe_SnapshotList *m_SnapshotList;
	QWidget             *m_CfgWidgetContent;

	public slots:
	void SLOT_Menu_OnSavedProject() { emit SIGNAL_onSaveProject(); };
	void SLOT_Menu_OnOpenProject() { emit SIGNAL_onOpenProject(); };
	void SLOT_Menu_OnClipboardPaste() { emit SIGNAL_onClipboard(); };
	void SLOT_Menu_OnCollapseAll() { emit SIGNAL_onCollapseAll(); };
	void SLOT_Menu_importFile() { emit SIGNAL_onImportFile(); };

	void SLOT_onButtonUnpin() { SetStaytoTop(true); }
	void SLOT_onButtonStay() { SetStaytoTop(false); }
	void SLOT_onButtonListView() { emit SIGNAL_onListView(); }
	void SLOT_onButtonConfigView() { emit SIGNAL_onConfigView(); }
	void SLOT_onButtonTakeSnip() { emit SIGNAL_onTakeSnip(); }
	void SLOT_onButtonSave() { emit SIGNAL_onSaveProject(); }
	void SLOT_onButtonOpen() { emit SIGNAL_onOpenProject(); }
	void SLOT_onButtonCollapseAll() { emit SIGNAL_onCollapseAll(); }
	void SLOT_onButtonCollectorView() { emit SIGNAL_onCollectorView(); }
	void SLOT_onButtonTakeSelect() {emit SIGNAL_onTakeSelect();}
	void SLOT_onButtonConfigApply() { emit SIGNAL_onApplyConfig(); }
	void SLOT_onButtonConfigRestore() { emit SIGNAL_onRestoreConfig(); }
	void SLOT_onConfigChanged() { m_ConfigWidget.pushButton_Apply->setEnabled(true); }
	void SLOT_onSwitchLanguage();
signals:
	void SIGNAL_DropImageFile(QString);
	void SIGNAL_DropPixmap(QPixmap);
	void SIGNAL_DropURL(QString);
	void SIGNAL_onListView();
	void SIGNAL_onCollectorView();
	void SIGNAL_onTakeSelect();
	void SIGNAL_onConfigView();
	void SIGNAL_onTakeSnip();
	void SIGNAL_onSaveProject();
	void SIGNAL_onOpenProject();
	void SIGNAL_onCollapseAll();
	void SIGNAL_onClipboard();
	void SIGNAL_onImportFile();
	void SIGNAL_onApplyConfig();
	void SIGNAL_onRestoreConfig();
	void SIGNAL_onLanguageSwitch(unsigned char);
};

class DonateDialog:public QDialog
{
	Q_OBJECT
public:
	DonateDialog(QWidget *parent = NULL) : QDialog(parent) 
	{
		setModal(true);
		donatedlg.setupUi(this);
	};
	~DonateDialog() {};

private:
	Ui_DialogDonate donatedlg;
};


//----------Others----------------------------//
#include <QMessageBox>
//////////////////////////////////////////////////////////////////////////
//Information Configurations
//////////////////////////////////////////////////////////////////////////
#define  HK_SCREENSHOT "Ctrl+Tab"
#define  HK_SHOWMANAGER "Ctrl+M"
#define  HK_VIEWERDIALOG "Ctrl+W"
#define  PSNIP_CFG_FILEPATH   "/PSNIP.cfg"
#define  HK_SHORTCUT_MAX_LEN   32
#define PSNIP_LANGUAGE_EN	  0
#define PSNIP_LANGUAGE_ZH	  1
#define PSNIP_LANGUAGE_JAPAN  2
struct PSnip_stCfg
{
	PSnip_stCfg()
	{
		strcpy_s(stHKScreenShot,HK_SCREENSHOT);
		strcpy_s(stHKManagerDialog,HK_SHOWMANAGER);
		strcpy_s(stHKCollectionViewerDialog,HK_VIEWERDIALOG);
		Langugae=PSNIP_LANGUAGE_ZH;
	}
	char stHKScreenShot[HK_SHORTCUT_MAX_LEN];
	char stHKManagerDialog[HK_SHORTCUT_MAX_LEN];
	char stHKCollectionViewerDialog[HK_SHORTCUT_MAX_LEN];
	unsigned char Langugae;
};

//////////////////////////////////////////////////////////////////////////
//Image Data informations
//////////////////////////////////////////////////////////////////////////
struct PSnip_stItemInfo
{
	PSnip_stItemInfo()
	{
		X=0;
		Y=0;
		resWidth=0;
		resHeight=0;
		Alpha=255;
		visibled=false;
		IsCollectionDialog=false;
		pDisplayCollectionDialog=NULL;
		pDisplayDialog=NULL;
	}
	int     ID;
	qint32	X, Y;
	qint32	resWidth, resHeight;
	qint32  Alpha;
	bool    visibled;
	bool    IsCollectionDialog;
	QPixmap resPixmap;
	pe_qDisplayCollectionDialog *pDisplayCollectionDialog;
	pe_qDisplayDialog *pDisplayDialog;
	
};

////////////////////////////////////////////////////////////////////////////
///FileData structures
#define PSNIP_PROJECT_HEADER_GUID ("3854297F-414A-4458-BF7D-F6923C2DB37B")
#define PSNIP_PROJECT_TYPE_IMAGE 0
#define PSNIP_PROJECT_TYPE_COLLACTOR 1

struct FileImageItemHeader
{
	unsigned char	alpha;
	unsigned char   visibled;
	char			UTF8_Name[64];
	qint32			x, y, width, height;
	qint32			ImageSize;
	qint32          mode;
};

struct FileCollectorHeader
{
	unsigned char	alpha;
	unsigned char   visibled;
	char			UTF8_Name[64];
	qint32          count;
	qint32			x, y, width, height;
	qint32          Index;
};

struct FileCollectorImageHeader
{
	quint32 ImageSize;
	float   scale;
	int		px, py;
};

using namespace std;
class PSnip : public QObject
{
	Q_OBJECT

public:
	PSnip(QObject *parent = 0);
	~PSnip();
	//////////////////////////////////////////////////////////////////////////
	//Initialization
	bool Initialize();
	void AddPixmapToItem(QPixmap);
	void Clear();
	//////////////////////////////////////////////////////////////////////////
	bool SetHotKeyScreenShot(QString ht);
	bool SetHotKeyManagerDialog(QString ht);
	bool SetHotKeyViewerDialog(QString ht);

	//////////////////////////////////////////////////////////////////////////
	//Project File
	bool SaveProject();
	bool OpenProject();
	bool LoadProject(QString Path);
	bool SaveProject(QString Path);
	void ImportImage();


	//////////////////////////////////////////////////////////////////////////
	//Configurations
	void UpdateConfig();
	void ApplyConfig();
	void RestoreConfig();


	pe_qDisplayDialog *GetDisplayDialogById(int ID);
	PSnip_stItemInfo *GetSnapshotImageInfoByID(int Id);
	//////////////////////////////////////////////////////////////////////////
	//Functions of Activity
	//restore display dialog for snapshot
	void RestoreAllDisplayDialog();
	//Hide display dialog for snapshot
	void SwitchLanguage(unsigned char );
	void HideAllDisplayDialog();
	void ViewOnSnapshotImageList();
	void ViewOnSnapShotCollectorList();
	void ViewOnConfiguration();
	void TakeSnip();
	void TakeView();
	void CollapseAll();
	void ShowManagerDialog();
	void RemoveItemInfo(int ID);
	void AddImageItem(qint32 x, qint32 y, qint32 widht, qint32 height, QPixmap& Pixmap, qint32 Alpha = 255);
	void NewCollectionDialog(qint32 x, qint32 y, qint32 widht, qint32 height);
	void ShowProgressMsg();
	void HideProgressMsgProgressBar();
	void HideProgressMsg();
	PSnip_stItemInfo *GetLastCreatedItemInfoPointer();
	//Qt SLOTs
	public slots:
	void SLOT_HotKey_ScreenShot();
	void SLOT_HotKey_ScreenSelect();
	void SLOT_Snip_CaptureDone();
	void SLOT_Select_SelectDone();
	void SLOT_Dialog_ShowManagerDialog();
	void SLOT_SnapItem_DisplaySwitch(int);
	void SLOT_SnapItem_DeleteItem(int);
	void SLOT_SnapItem_RestoreDialogSize(int);
	void SLOT_SnapItem_SetAlphaValue(int,int);
	void SLOT_SnapItem_MouseFocus(int);
	void SLOT_SnapItem_MouseLostFocus(int);
	void SLOT_SnapItem_EnterPickMode(int);
	void SLOT_ItemDialogOnClosed(int ID);
	void SLOT_ItemDialogOnOpened(int ID);
	void SLOT_ItemDialogOnAplhaChanged(int,int);
	void SLOT_DisplayDialogCut(int);
	void SLOT_SnapShot_BeginSnip();
	void SLOT_SnapShot_ExitSnip();
	void SLOT_PasteClipboardImage();
	void SLOT_DragEvent_DragImage(QPixmap);
	void SLOT_DragEvent_DragFile(QString);
	void SLOT_DragEvent_DragUrl(QString);
	void SLOT_NewWork_URLDownloadCompleted();
	void SLOT_File_SaveDisplayDialogImage(int);
	void SLOT_File_SaveSourceImage(int);
	void SLOT_ManagerDialog_onImageItemsListView();
	void SLOT_ManagerDialog_onCollectorListView();
	void SLOT_ManagerDialog_onConfigView();
	void SLOT_ManagerDialog_onCollapseAll();
	void SLOT_ManagerDialog_onTakeSnapShot();
	void SLOT_ManagerDialog_onTakeSelectArea();
	void SLOT_ManagerDialog_onSaveProject();
	void SLOT_ManagerDialog_onOpenProject();
	void SLOT_ManagerDialog_onImport();
	void SLOT_ManagerDialog_OnConfigApply();
	void SLOT_ManagerDialog_OnConfigRestore();
	void SLOT_DisplayDialog_onMoving(QPoint, pe_qDisplayDialog *);
	void SLOT_DisplayDialog_onDraggedDone(QPoint, pe_qDisplayDialog *);
	void SLOT_OnMiniActivated(QSystemTrayIcon::ActivationReason reason);
	void SLOT_onMessageError(QString);
	void SLOT_MiniSwitch();
	void SLOT_Quit();
	void SLOT_SwitchToEN();
	void SLOT_SwitchToZh();
	void SLOT_Progress(QString Msg, int prog, int current);
	void SLOT_SwitchLanguage(unsigned char la) { SwitchLanguage(la); }
private:
	QPixmap m_CollectionLogo;
	//m_LastManagerIsShown
	bool m_LastManagerVisible;
	//HK(Hot Key) Shortcut
	QxtGlobalShortcut *m_HotkeyScreenShot;
	QxtGlobalShortcut *m_HotkeyManager;
	QxtGlobalShortcut *m_HotkeyViewerDialog;
	//Configuration member
	pe_ConfigFile<PSnip_stCfg> m_cfg;
	//////////////////////////////////////////////////////////////////////////
	//dialogs
	//////////////////////////////////////////////////////////////////////////
	//SnipDialog
	pe_qScreenDialog *m_ScreenDialog;
	//ManagerDialog
	pe_ManagerDialog *m_ManagerDialog;
	//Image list
	vector<PSnip_stItemInfo> m_DataInfo;
	//Network Download
	QUrlDownloader m_UrlDownloader;
	//MiniMenu
	QSystemTrayIcon *m_trayIcon;
	QMenu	*m_MiniMenu;
	QAction *m_MiniShowManager, *m_MiniScreenShot, *m_MiniCreateCollector, *m_miniExit, *m_miniOpenproject, *m_miniSaveProject;
	//////////////////////////////////////////////////////////////////////////
	//Last status marks
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//Controller flags
	//////////////////////////////////////////////////////////////////////////
	bool m_Controller_flag_bHiddenDialogforSnapshot;

	///////////////////////////////////////////////////////////////////////////
	//Donate Dialog
	DonateDialog *m_DonateDlg;

	//////////////////////////////////////////////////////////////////////////
	//Project file path
	QString m_ProjPath;
};

#endif // PSNIP_H
