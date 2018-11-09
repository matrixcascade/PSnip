#include "psnip.h"

PSnip::PSnip(QObject *parent)
	: QObject(parent)
{
	m_LastManagerVisible=true;
	m_HotkeyScreenShot=NULL;
	m_HotkeyManager = NULL;
	m_HotkeyViewerDialog = NULL;
	m_ScreenDialog = NULL;
	m_ManagerDialog = NULL;
	m_Controller_flag_bHiddenDialogforSnapshot=true;
}

PSnip::~PSnip()
{

}

bool PSnip::Initialize()
{
	//Data initialize
	m_ScreenDialog = new pe_qScreenDialog(NULL);
	if (!m_ScreenDialog)
	{
		return false;
	}
	
	m_ManagerDialog = new pe_ManagerDialog(NULL);
	if (!m_ManagerDialog)
	{
		return false;
	}
	
	m_DonateDlg = new DonateDialog(m_ManagerDialog);
	connect(m_ManagerDialog->m_ConfigWidget.pushButton_Donate, SIGNAL(clicked()), m_DonateDlg, SLOT(show()));
	QString cfgPath = qApp->applicationDirPath() + PSNIP_CFG_FILEPATH;
	
	if (!m_cfg.LoadCfgFile(cfgPath.toStdString().c_str()))
	{
		m_cfg.Add(PSnip_stCfg());
		if (!m_cfg.Update())
		{
			QMessageBox::information(m_ManagerDialog, tr("warning"), tr("Could not update configure informations!"), QMessageBox::Ok);
		}
	}
	//Languages
	SwitchLanguage(m_cfg.GetConfigStruct(0).Langugae);
	//Network
	

	connect(&m_UrlDownloader, SIGNAL(SIGNAL_DownloadCompleted()),	this, SLOT(SLOT_NewWork_URLDownloadCompleted()));
	//HK shortCut Initialize
	if (!SetHotKeyScreenShot(m_cfg.GetConfigStruct(0).stHKScreenShot))
	{
		QMessageBox::information(m_ManagerDialog, tr("Error"), tr("Hot key initialized failed!"), QMessageBox::Ok);
		return false;
	}

	if (!SetHotKeyManagerDialog(m_cfg.GetConfigStruct(0).stHKManagerDialog))
	{
		QMessageBox::information(m_ManagerDialog, tr("Error"), tr("Hot key initialized failed!"), QMessageBox::Ok);
		return false;
	}

	if (!SetHotKeyViewerDialog(m_cfg.GetConfigStruct(0).stHKCollectionViewerDialog))
	{
		QMessageBox::information(m_ManagerDialog, tr("Error"), tr("Hot key initialized failed!"), QMessageBox::Ok);
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//Logo
	m_CollectionLogo = QPixmap(":/PSnip/Resources/Collection.png");
	///////////////////////////////////////////////////////////////////////////
	//Local HK shortcut
	QAction *clipborderSk = new QAction(m_ManagerDialog);
	clipborderSk->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
	m_ManagerDialog->addAction(clipborderSk);
	connect(clipborderSk, SIGNAL(triggered()), this, SLOT(SLOT_PasteClipboardImage()));

	//////////////////////////////////////////////////////////////////////////
	//Mini icon
	m_trayIcon = new QSystemTrayIcon(this);
	m_trayIcon->setIcon(QIcon(":/PSnip/Resources/Mini.png"));
	QString Helper= tr("PSnip 1.0");
	Helper += "\n";
	Helper += tr("Press (") + m_cfg.GetConfigStruct(0).stHKScreenShot + tr(") to take screenshot");
	Helper += "\n";
	Helper += tr("Press (") + m_cfg.GetConfigStruct(0).stHKCollectionViewerDialog + tr(") to create a collector");
	m_trayIcon->setToolTip(Helper);
	m_trayIcon->show();
	m_MiniMenu = new QMenu(m_ManagerDialog);

	m_MiniShowManager = new QAction(this);
	m_MiniShowManager->setText(tr("Show Manager"));
	connect(m_MiniShowManager, SIGNAL(triggered()), this, SLOT(SLOT_Dialog_ShowManagerDialog()));
		
	m_MiniScreenShot = new QAction(this);
	m_MiniScreenShot->setText(tr("ScreenShot ")+QString(m_cfg.GetConfigStruct(0).stHKScreenShot));
	connect(m_MiniScreenShot, SIGNAL(triggered()), this, SLOT(SLOT_HotKey_ScreenShot()));
		
	m_MiniCreateCollector = new QAction(this);
	m_MiniCreateCollector->setText(tr("New Collector ") + QString(m_cfg.GetConfigStruct(0).stHKCollectionViewerDialog));
	connect(m_MiniCreateCollector, SIGNAL(triggered()), this, SLOT(SLOT_ManagerDialog_onCollectorListView()));

	m_miniOpenproject = new QAction(this);
	m_miniOpenproject->setText(tr("Open Project"));
	connect(m_miniOpenproject, SIGNAL(triggered()), this, SLOT(SLOT_ManagerDialog_onOpenProject()));

	m_miniSaveProject = new QAction(this);
	m_miniSaveProject->setText(tr("Save Project"));
	connect(m_miniSaveProject, SIGNAL(triggered()), this, SLOT(SLOT_ManagerDialog_onSaveProject()));

	m_miniExit = new QAction(this);
	m_miniExit->setText(tr("Quit"));
	connect(m_miniExit, SIGNAL(triggered()), this, SLOT(SLOT_Quit()));

	m_MiniMenu->addAction(m_MiniScreenShot);
	m_MiniMenu->addAction(m_MiniCreateCollector);
	m_MiniMenu->addAction(m_MiniShowManager);
	m_MiniMenu->addAction(m_miniOpenproject);
	m_MiniMenu->addAction(m_miniSaveProject);
	m_MiniMenu->addAction(m_miniExit);
	
	m_trayIcon->setContextMenu(m_MiniMenu);
	connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(SLOT_OnMiniActivated(QSystemTrayIcon::ActivationReason)));
	
	
	//Connectors
	connect(m_ScreenDialog, SIGNAL(SIGNAL_OnCaptureDone()), this, SLOT(SLOT_Snip_CaptureDone()));
	connect(m_ScreenDialog, SIGNAL(SIGNAL_OnSelectDone()), this, SLOT(SLOT_Select_SelectDone()));
	connect(m_ScreenDialog, SIGNAL(SIGNAL_OnCaptureExit()), this, SLOT(SLOT_SnapShot_ExitSnip()));
	connect(m_ScreenDialog, SIGNAL(SIGNAL_OnCaptureReady()), this, SLOT(SLOT_SnapShot_BeginSnip()));
	connect(m_ManagerDialog->m_SnapshotList, SIGNAL(SIGNAL_onItemDisplaySwitch(int)), this, SLOT(SLOT_SnapItem_DisplaySwitch(int)));
	connect(m_ManagerDialog->m_SnapshotList, SIGNAL(SIGNAL_onItemDelete(int)), this, SLOT(SLOT_SnapItem_DeleteItem(int)));
	connect(m_ManagerDialog->m_SnapshotList, SIGNAL(SIGNAL_onItemRestore(int)), this, SLOT(SLOT_SnapItem_RestoreDialogSize(int)));
	connect(m_ManagerDialog->m_SnapshotList, SIGNAL(SIGNAL_onItemCut(int)), this, SLOT(SLOT_DisplayDialogCut(int)));
	connect(m_ManagerDialog->m_SnapshotList, SIGNAL(SIGNAL_onItemAlphaChange(int,int)), this, SLOT(SLOT_SnapItem_SetAlphaValue(int,int)));
	connect(m_ManagerDialog->m_SnapshotList, SIGNAL(SIGNAL_onItemMouseFocus(int)), this, SLOT(SLOT_SnapItem_MouseFocus(int)));
	connect(m_ManagerDialog->m_SnapshotList, SIGNAL(SIGNAL_onItemMouseLostFocus(int)), this, SLOT(SLOT_SnapItem_MouseLostFocus(int)));
	connect(m_ManagerDialog->m_SnapshotList, SIGNAL(SIGNAL_onItemSave(int)), this, SLOT(SLOT_File_SaveSourceImage(int)));
	connect(m_ManagerDialog->m_SnapshotList, SIGNAL(SIGNAL_onItemPick(int)), this, SLOT(SLOT_SnapItem_EnterPickMode(int)));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_DropPixmap(QPixmap)), this, SLOT(SLOT_DragEvent_DragImage(QPixmap)));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_DropImageFile(QString)), this, SLOT(SLOT_DragEvent_DragFile(QString)));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_DropURL(QString)), this, SLOT(SLOT_DragEvent_DragUrl(QString)));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_onListView()), this, SLOT(SLOT_ManagerDialog_onImageItemsListView()));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_onConfigView()), this, SLOT(SLOT_ManagerDialog_onConfigView()));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_onTakeSnip()), this, SLOT(SLOT_ManagerDialog_onTakeSnapShot()));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_onCollapseAll()), this, SLOT(SLOT_ManagerDialog_onCollapseAll()));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_onCollectorView()), this, SLOT(SLOT_ManagerDialog_onCollectorListView()));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_onTakeSelect()), this, SLOT(SLOT_ManagerDialog_onTakeSelectArea()));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_onSaveProject()), this, SLOT(SLOT_ManagerDialog_onSaveProject()));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_onOpenProject()), this, SLOT(SLOT_ManagerDialog_onOpenProject()));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_onClipboard()), this, SLOT(SLOT_PasteClipboardImage()));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_onImportFile()), this, SLOT(SLOT_ManagerDialog_onImport()));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_onApplyConfig()), this, SLOT(SLOT_ManagerDialog_OnConfigApply()));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_onRestoreConfig()), this, SLOT(SLOT_ManagerDialog_OnConfigRestore()));
	connect(m_ManagerDialog, SIGNAL(SIGNAL_onLanguageSwitch(unsigned char)), this, SLOT(SLOT_SwitchLanguage(unsigned char)));
	ViewOnSnapshotImageList();
	m_ManagerDialog->show();
	HideProgressMsg();
	return true;
}

void PSnip::AddPixmapToItem(QPixmap pixmap)
{
	this->ViewOnSnapshotImageList();
	QRect rect = QRect(0,0,pixmap.width(),pixmap.height());
	AddImageItem(rect.x(), rect.y(), rect.width(), rect.height(), pixmap);

	PSnip_stItemInfo *pItem = GetLastCreatedItemInfoPointer();
	if (!pItem)
	{
		return;
	}
	pItem->pDisplayDialog->SnapshotDisplayImage(rect, pixmap);
	pItem->pDisplayDialog->activateWindow();
	pItem->visibled = true;
}

void PSnip::Clear()
{
	m_ProjPath.clear();
	m_ManagerDialog->m_SnapshotList->Clear();
	for (int i=0;i<m_DataInfo.size();i++)
	{
		if (m_DataInfo.at(i).IsCollectionDialog)
		{
			m_DataInfo.at(i).pDisplayCollectionDialog->deleteLater();
		}
		else
		{
			m_DataInfo.at(i).pDisplayDialog->deleteLater();
		}
	}
	this->m_DataInfo.clear();
}

bool PSnip::SetHotKeyScreenShot(QString ht)
{
	if (ht.length()>HK_SHORTCUT_MAX_LEN-1)
	{
		return false;
	}
	if (QKeySequence(ht).isEmpty())
	{
		return false;
	}
	if (m_HotkeyScreenShot)
	{
		m_HotkeyScreenShot->disconnect(this);
		delete m_HotkeyScreenShot;
	}
	strcpy(m_cfg.GetConfigStruct(0).stHKScreenShot, ht.toStdString().c_str());
	m_cfg.Update();

	m_HotkeyScreenShot = new QxtGlobalShortcut(QKeySequence(ht), this);
	connect(m_HotkeyScreenShot, SIGNAL(activated()), this, SLOT(SLOT_HotKey_ScreenShot()));
	return true;
}

bool PSnip::SetHotKeyManagerDialog(QString ht)
{
	if (ht.length() > HK_SHORTCUT_MAX_LEN - 1)
	{
		return false;
	}
	if (QKeySequence(ht).isEmpty())
	{
		return false;
	}
	if (m_HotkeyManager)
	{
		m_HotkeyManager->disconnect(this);
		delete m_HotkeyManager;
	}
	strcpy(m_cfg.GetConfigStruct(0).stHKManagerDialog, ht.toStdString().c_str());
	m_cfg.Update();

	m_HotkeyManager = new QxtGlobalShortcut(QKeySequence(ht), this);
	connect(m_HotkeyManager, SIGNAL(activated()), this, SLOT(SLOT_Dialog_ShowManagerDialog()));
	return true;
}

bool PSnip::SetHotKeyViewerDialog(QString ht)
{
	if (ht.length() > HK_SHORTCUT_MAX_LEN - 1)
	{
		return false;
	}
	if (QKeySequence(ht).isEmpty())
	{
		return false;
	}
	if (m_HotkeyViewerDialog)
	{
		m_HotkeyViewerDialog->disconnect(this);
		delete m_HotkeyViewerDialog;
	}
	strcpy(m_cfg.GetConfigStruct(0).stHKCollectionViewerDialog, ht.toStdString().c_str());
	m_cfg.Update();

	m_HotkeyViewerDialog = new QxtGlobalShortcut(QKeySequence(ht), this);
	connect(m_HotkeyViewerDialog, SIGNAL(activated()), this, SLOT(SLOT_HotKey_ScreenSelect()));
	return true;
}

QByteArray GetPixmapBytesStream(QPixmap &px)
{
	if (!px.save("Cache","PNG"))return 0;
	QFile file("Cache");
	if (!file.open(QIODevice::ReadOnly))
		return QByteArray();
	QByteArray Array;
	Array = file.read(file.size());
	file.close();
	file.remove();
	return Array;
}


bool PSnip::SaveProject()
{
	pe_File file;
	bool ManagerDialogIsVisibled = false;
	if (m_ManagerDialog->isVisible())
	{
		ManagerDialogIsVisibled = true;
	}
	m_ManagerDialog->show();
	QString fileName = QFileDialog::getSaveFileName(m_ManagerDialog,
		tr("Psnip Projects"),
		"",
		tr("Psnip Projects (*.Pss)"));
	if(!ManagerDialogIsVisibled)
	m_ManagerDialog->hide();
	if (fileName.isEmpty())
	{
		return true;
	}
	return SaveProject(fileName);
}

bool PSnip::SaveProject(QString fileName)
{
	pe_File file;
	if (fileName.isEmpty())
	{
		return true;
	}
	ShowProgressMsg();
	if (!file.Open((char *)fileName.toStdString().c_str(), "wb"))
		goto ERR;

	if (!file.WriteToken(PSNIP_PROJECT_HEADER_GUID, sizeof(PSNIP_PROJECT_HEADER_GUID)))
		goto ERR;

	SLOT_Progress(tr("Project saving.."), m_DataInfo.size(), 0);
	for (int i = 0; i < m_DataInfo.size(); i++)
	{
		if (m_DataInfo.at(i).IsCollectionDialog)
		{
			FileCollectorHeader CollectorHeader;
			CollectorHeader.alpha = (unsigned char)m_DataInfo.at(i).pDisplayCollectionDialog->GetOpacity();
			CollectorHeader.count = m_DataInfo.at(i).pDisplayCollectionDialog->m_Pixmaps.size();
			CollectorHeader.height = m_DataInfo.at(i).pDisplayCollectionDialog->m_DialogLastSize.height();
			CollectorHeader.width = m_DataInfo.at(i).pDisplayCollectionDialog->m_DialogLastSize.width();
			CollectorHeader.Index = m_DataInfo.at(i).pDisplayCollectionDialog->GetCurrentDisplayIndex();
			QSnapshotItem *Item = m_ManagerDialog->m_SnapshotList->GetSnapshotItemByID(m_DataInfo[i].ID);
			if (Item == NULL)
			{
				goto ERR;
			}
			SLOT_Progress(tr("Collections saving:") + Item->GetName() + " " + QString::number(i) + "/" + QString::number(m_DataInfo.size()), m_DataInfo.size(), i);
			strcpy(CollectorHeader.UTF8_Name, Item->GetName().toUtf8());
			CollectorHeader.visibled = (unsigned char)m_DataInfo.at(i).visibled;
			CollectorHeader.x = m_DataInfo.at(i).pDisplayCollectionDialog->geometry().x();
			CollectorHeader.y = m_DataInfo.at(i).pDisplayCollectionDialog->geometry().y();
			unsigned char Type = PSNIP_PROJECT_TYPE_COLLACTOR;
			if (!file.WriteToken(&Type, 1))goto ERR;
			if (!file.WriteToken(&CollectorHeader, sizeof(CollectorHeader))) goto ERR;

			for (int j = 0; j < CollectorHeader.count; j++)
			{
				FileCollectorImageHeader ColrImgHeader;
				SLOT_Progress(Item->GetName() + tr(" Image ") + QString::number(j) + "/" + QString::number(CollectorHeader.count), CollectorHeader.count, i);
				QByteArray Array = GetPixmapBytesStream(m_DataInfo.at(i).pDisplayCollectionDialog->m_Pixmaps.at(j).ResPixmap);
				if (Array.size() == 0)
				{
					goto ERR;
				}
				ColrImgHeader.ImageSize = Array.size();
				ColrImgHeader.scale = m_DataInfo.at(i).pDisplayCollectionDialog->m_Pixmaps.at(j).Scale;
				ColrImgHeader.px = m_DataInfo.at(i).pDisplayCollectionDialog->m_Pixmaps.at(j).StartPoint.x();
				ColrImgHeader.py = m_DataInfo.at(i).pDisplayCollectionDialog->m_Pixmaps.at(j).StartPoint.y();

				if (!file.WriteToken(&ColrImgHeader, sizeof(ColrImgHeader))) goto ERR;

				if (!file.WriteToken(
					Array.data()
					, Array.size()))
					goto ERR;
			}
		}
		else
		{
			FileImageItemHeader ImgItem;
			ImgItem.alpha = (unsigned char)m_DataInfo.at(i).pDisplayDialog->GetOpacity();
			ImgItem.height = m_DataInfo.at(i).pDisplayDialog->m_DialogLastSize.height();
			ImgItem.width = m_DataInfo.at(i).pDisplayDialog->m_DialogLastSize.width();
			QByteArray Array = GetPixmapBytesStream(m_DataInfo.at(i).resPixmap);
			if (Array.size() == 0)
			{
				goto ERR;
			}
			ImgItem.ImageSize = Array.size();
			ImgItem.visibled = m_DataInfo.at(i).visibled;
			ImgItem.x = m_DataInfo.at(i).pDisplayDialog->geometry().x();
			ImgItem.y = m_DataInfo.at(i).pDisplayDialog->geometry().y();
			ImgItem.mode = m_DataInfo.at(i).pDisplayDialog->m_Status;
			QSnapshotItem *Item = m_ManagerDialog->m_SnapshotList->GetSnapshotItemByID(m_DataInfo[i].ID);
			if (Item == NULL)
			{
				goto ERR;
			}
			SLOT_Progress(tr("Snip saving:") + Item->GetName() + " " + QString::number(i) + "/" + QString::number(m_DataInfo.size()), m_DataInfo.size(), i);
			strcpy(ImgItem.UTF8_Name, Item->GetName().toUtf8());
			unsigned char Type = PSNIP_PROJECT_TYPE_IMAGE;
			if (!file.WriteToken(&Type, 1))goto ERR;
			file.Write(&ImgItem, sizeof(ImgItem));

			if (!file.WriteToken(
				Array.data()
				, Array.size()))
				goto ERR;
		}
	}
	file.Close();
	//m_trayIcon->showMessage(tr("Succeeded"), tr("The project has been saved successfully"), QSystemTrayIcon::NoIcon);
	HideProgressMsg();
	return true;
ERR:
	m_trayIcon->showMessage(tr("ERROR"), tr("Could not saved this project"), QSystemTrayIcon::Warning);
	HideProgressMsg();
	file.Close();
	return false;
}

bool PSnip::OpenProject()
{
	m_ManagerDialog->show();
	QString fileName = QFileDialog::getOpenFileName(m_ManagerDialog,
		tr("Psnip Projects"),
		"",
		tr("Psnip Projects (*.Pss)"));
	if (fileName.isEmpty())
	{
		return true;
	}
	if (!LoadProject(fileName))
	{
		return false;
	}
	return true;
}

bool PSnip::LoadProject(QString Path)
{
	Clear();
	m_ProjPath = Path;
	pe_File File;
	if (!File.Open((char *)Path.toStdString().c_str()))
		return false;

	char GUID_Str[sizeof PSNIP_PROJECT_HEADER_GUID];
	File.ReadToken(GUID_Str, sizeof GUID_Str);
	if (memcmp(GUID_Str,PSNIP_PROJECT_HEADER_GUID,sizeof PSNIP_PROJECT_HEADER_GUID)!=0)
	{
		return false;
	}
	ShowProgressMsg();
	unsigned char Type;
	while (true)
	{
		if(!File.ReadToken(&Type, 1))
			break;

		SLOT_Progress(tr("Solving...."), 1, 0);
		switch (Type)
		{
		case PSNIP_PROJECT_TYPE_COLLACTOR:
		{
			FileCollectorHeader CollectorHeader;
			if (!File.ReadToken(&CollectorHeader,sizeof(FileCollectorHeader)))
			{
				goto ERR;
			}
			this->NewCollectionDialog(CollectorHeader.x, CollectorHeader.y, CollectorHeader.width, CollectorHeader.height);
			GetLastCreatedItemInfoPointer()->pDisplayCollectionDialog->SetOpacity(CollectorHeader.alpha);
			m_ManagerDialog->m_SnapshotList->GetLastCreatedSnapshotItem()->SetName(QString::fromUtf8(CollectorHeader.UTF8_Name));
			if (CollectorHeader.visibled)
			{
				GetLastCreatedItemInfoPointer()->pDisplayCollectionDialog->ShowDirectly();
			}
			else
			{
				GetLastCreatedItemInfoPointer()->pDisplayCollectionDialog->CloseDirectly();
			}
			for (int i=0;i<CollectorHeader.count;i++)
			{
				SLOT_Progress(tr("loading:") + QString::fromUtf8(CollectorHeader.UTF8_Name)+" "+QString::number(i)+"/"+QString::number(CollectorHeader.count), CollectorHeader.count, i);
				FileCollectorImageHeader ImgHeader;
				if (!File.ReadToken(&ImgHeader, sizeof(ImgHeader)))
					goto ERR;

				QByteArray Qbytes;
				Qbytes.resize(ImgHeader.ImageSize);
				if (!File.ReadToken(Qbytes.data(), ImgHeader.ImageSize))
					goto ERR;

				QPixmap pixmap;
				pixmap.loadFromData(Qbytes);
				if (pixmap.isNull())
				{
					goto ERR;
				}
				CollectionPixmapInfo Info;
				Info.ResPixmap = pixmap;
				Info.Scale = ImgHeader.scale;
				Info.StartPoint = QPoint(ImgHeader.px, ImgHeader.py);
				GetLastCreatedItemInfoPointer()->pDisplayCollectionDialog->AddCollectionPixInfo(Info);
			}
			GetLastCreatedItemInfoPointer()->pDisplayCollectionDialog->SetCurrentDisplayIndex(CollectorHeader.Index);
		}
		break;
		case  PSNIP_PROJECT_TYPE_IMAGE:
		{
			FileImageItemHeader ImgHeader;
			if (!File.ReadToken(&ImgHeader, sizeof(FileImageItemHeader)))
			{
				goto ERR;
			}
			SLOT_Progress(tr("loading:")+ QString::fromUtf8(ImgHeader.UTF8_Name), 1, 0);
			QByteArray Qbytes;
			Qbytes.resize(ImgHeader.ImageSize);
			if (!File.ReadToken(Qbytes.data(), ImgHeader.ImageSize))
				goto ERR;

			QPixmap pixmap;
			pixmap.loadFromData(Qbytes);
			if (pixmap.isNull())
			{
				goto ERR;
			}

			this->AddImageItem(ImgHeader.x, ImgHeader.y, ImgHeader.width, ImgHeader.height, pixmap,ImgHeader.alpha);
			m_ManagerDialog->m_SnapshotList->GetLastCreatedSnapshotItem()->SetName(QString::fromUtf8(ImgHeader.UTF8_Name));

			PSnip_stItemInfo *pItem = GetLastCreatedItemInfoPointer();
			if (!pItem)
			{
				goto ERR;
			}
			pItem->pDisplayDialog->SnapshotDisplayImage(QRect(ImgHeader.x, ImgHeader.y, ImgHeader.width, ImgHeader.height), pixmap);
			if (ImgHeader.visibled)
			{
				GetLastCreatedItemInfoPointer()->pDisplayDialog->ShowDirectly();
				GetLastCreatedItemInfoPointer()->pDisplayDialog->activateWindow();
			}
			else
			{
				GetLastCreatedItemInfoPointer()->pDisplayDialog->CloseDirectly();
			}
			GetLastCreatedItemInfoPointer()->pDisplayDialog->m_Status = ImgHeader.mode;
			if (ImgHeader.mode == DISPLAYDIALOG_STATUS_MINIMODE)
				GetLastCreatedItemInfoPointer()->pDisplayDialog->EnterMiniMode(GetLastCreatedItemInfoPointer()->pDisplayDialog->pos() + QPoint(DISPLAYDIALOG_MINI_SIZE / 2, DISPLAYDIALOG_MINI_SIZE / 2));

		}
		break;
		default:
			break;
		}
	}
	File.Close();
	HideProgressMsg();
	return true;
	ERR:
	File.Close();
	HideProgressMsg();
	return false;
}

void PSnip::ImportImage()
{
	QString fileName = QFileDialog::getOpenFileName(m_ManagerDialog,
		tr("Image Save"),
		"",
		tr("Image Files (*.png *.jpg *.bmp *.jpeg *.ppm *.tiff *.xbm *.xpm)"));
	if (fileName.isEmpty())
	{
		return;
	}
	QPixmap pixmap(fileName);
	AddPixmapToItem(pixmap);
	return;
}

void PSnip::UpdateConfig()
{
	m_ManagerDialog->m_ConfigWidget.lineEdit_ScreenShot->setText(QString(m_cfg.GetConfigStruct(0).stHKScreenShot));
	m_ManagerDialog->m_ConfigWidget.lineEdit_ManagerDialog->setText(QString(m_cfg.GetConfigStruct(0).stHKManagerDialog));
	m_ManagerDialog->m_ConfigWidget.lineEdit_CollectorView->setText(QString(m_cfg.GetConfigStruct(0).stHKCollectionViewerDialog));
	m_ManagerDialog->m_ConfigWidget.pushButton_Apply->setEnabled(false);
}

void PSnip::ApplyConfig()
{
	strcpy(m_cfg.GetConfigStruct(0).stHKScreenShot, m_ManagerDialog->m_ConfigWidget.lineEdit_ScreenShot->text().toStdString().c_str());
	strcpy(m_cfg.GetConfigStruct(0).stHKManagerDialog, m_ManagerDialog->m_ConfigWidget.lineEdit_ManagerDialog->text().toStdString().c_str());
	strcpy(m_cfg.GetConfigStruct(0).stHKCollectionViewerDialog, m_ManagerDialog->m_ConfigWidget.lineEdit_CollectorView->text().toStdString().c_str());
	m_cfg.Update();
	QMessageBox::information(m_ManagerDialog, tr("Succeeded"), tr("New hotkeys has been saved,you need restart the application to apply them"), QMessageBox::Ok);
}

void PSnip::RestoreConfig()
{
	m_cfg.Clear();
	m_cfg.Add(PSnip_stCfg());
	m_cfg.Update();
	SetHotKeyManagerDialog(m_cfg.GetConfigStruct(0).stHKManagerDialog);
	SetHotKeyScreenShot(m_cfg.GetConfigStruct(0).stHKScreenShot);
	SetHotKeyViewerDialog(m_cfg.GetConfigStruct(0).stHKCollectionViewerDialog);
	UpdateConfig();
}

pe_qDisplayDialog * PSnip::GetDisplayDialogById(int ID)
{
	PSnip_stItemInfo *pssInfo = GetSnapshotImageInfoByID(ID);
	if (pssInfo)
	{
		return pssInfo->pDisplayDialog;
	}
	return NULL;
}

PSnip_stItemInfo * PSnip::GetSnapshotImageInfoByID(int Id)
{
	for (int i = 0; i < m_DataInfo.size(); i++)
	{
		if (m_DataInfo[i].ID == Id)
		{
			return &m_DataInfo[i];
		}
	}
	return NULL;
}

void PSnip::RestoreAllDisplayDialog()
{
	for (int i = 0; i < m_DataInfo.size(); i++)
	{
		if (m_DataInfo.at(i).visibled)
		{
			if (m_DataInfo[i].IsCollectionDialog)
				m_DataInfo.at(i).pDisplayCollectionDialog->show();
			else
				m_DataInfo.at(i).pDisplayDialog->show();
		}
	}
}

void PSnip::SwitchLanguage(unsigned char l)
{
	QTranslator *Trslt=new QTranslator(qApp);
	m_cfg.GetConfigStruct(0).Langugae = l;
	m_cfg.Update();
	QString path =qApp->applicationDirPath();
	switch (l)
	{
	case PSNIP_LANGUAGE_EN:
		Trslt->load(path + "/psnip_en.qm");
		break;
	case PSNIP_LANGUAGE_ZH:
		Trslt->load(path + "/psnip_zh.qm");
		break;
	case PSNIP_LANGUAGE_JAPAN:
		Trslt->load(path + "/psnip_jp.qm");
		break;
	default:
		break;
	}
	qApp->installTranslator(Trslt);
	m_ManagerDialog->ResetLanguage();

}

void PSnip::HideAllDisplayDialog()
{
	for (int i=0;i<m_DataInfo.size();i++)
	{
		if (m_DataInfo.at(i).visibled)
		{
			if(m_DataInfo[i].IsCollectionDialog)
			m_DataInfo.at(i).pDisplayCollectionDialog->hide();
			else
			m_DataInfo.at(i).pDisplayDialog->hide();
		}
	}
}

void PSnip::ViewOnSnapshotImageList()
{
	m_ManagerDialog->m_SnapshotList->show();
	m_ManagerDialog->m_SnapshotList->ShowMode_ImageItems();
	m_ManagerDialog->m_CfgWidgetContent->hide();
}

void PSnip::ViewOnSnapShotCollectorList()
{
	m_ManagerDialog->m_SnapshotList->show();
	m_ManagerDialog->m_SnapshotList->ShowMode_Collector();
	m_ManagerDialog->m_CfgWidgetContent->hide();
}

void PSnip::ViewOnConfiguration()
{
	m_ManagerDialog->m_SnapshotList->hide();
	m_ManagerDialog->m_CfgWidgetContent->show();
	UpdateConfig();
}

void PSnip::TakeSnip()
{
	if (m_ManagerDialog->isVisible())
	{
		m_LastManagerVisible = true;
	}
	else
	{
		m_LastManagerVisible = false;
	}
	m_ManagerDialog->setVisible(false);

	if (m_Controller_flag_bHiddenDialogforSnapshot)
	{
		HideAllDisplayDialog();
	}
	ViewOnSnapshotImageList();
	m_ScreenDialog->BeginSnip();
}

void PSnip::TakeView()
{
	if (m_ManagerDialog->isVisible())
	{
		m_LastManagerVisible = true;
	}
	else
	{
		m_LastManagerVisible = false;
	}
	m_ManagerDialog->setVisible(false);

	if (m_Controller_flag_bHiddenDialogforSnapshot)
	{
		HideAllDisplayDialog();
	}
	ViewOnSnapShotCollectorList();
	m_ScreenDialog->BeginSelectViewRegion();
}

void PSnip::CollapseAll()
{
	for (int i=0;i<m_DataInfo.size();i++)
	{
		if (m_DataInfo[i].visibled)
		{
			if(m_DataInfo[i].IsCollectionDialog)
			m_DataInfo[i].pDisplayCollectionDialog->CloseAction();
			else
			m_DataInfo[i].pDisplayDialog->CloseAction();
		}
	}
}

void PSnip::ShowManagerDialog()
{
	m_ManagerDialog->show();
}

void PSnip::RemoveItemInfo(int ID)
{
	PSnip_stItemInfo *pInfo = GetSnapshotImageInfoByID(ID);
	if (!pInfo)
	{
		return;
	}
	
	if (pInfo->pDisplayDialog)
	{
		pInfo->pDisplayDialog->deleteLater();
		pInfo->pDisplayCollectionDialog = NULL;
	}
	if (pInfo->pDisplayCollectionDialog)
	{
		pInfo->pDisplayCollectionDialog->deleteLater();
		pInfo->pDisplayCollectionDialog = NULL;
	}
	m_ManagerDialog->m_SnapshotList->removeSnapshotItem(ID);

	for (int i = 0; i < m_DataInfo.size(); i++)
	{
		if (m_DataInfo[i].ID == ID)
		{
			m_DataInfo.erase(m_DataInfo.begin() + i);
			break;
		}
	}
}

void PSnip::AddImageItem(qint32 x, qint32 y, qint32 width, qint32 height, QPixmap& Pixmap, qint32 Alpha /*= 255*/)
{
	PSnip_stItemInfo Newinfo;
	Newinfo.X = x;
	Newinfo.Y = y;
	Newinfo.resHeight = Pixmap.height();
	Newinfo.resWidth = Pixmap.width();
	Newinfo.resPixmap = Pixmap;
	
	Newinfo.Alpha = Alpha;
	
	//////////////////////////////////////////////////////////////////////////
	//Update to GUI(imageList) &Get id
	Newinfo.ID=m_ManagerDialog->m_SnapshotList->addSnapshotItem(Pixmap);
	QSnapshotItem *item = m_ManagerDialog->m_SnapshotList->GetLastCreatedSnapshotItem();
	if (item)
	{
		item->SetName(tr("Reference"));
	}
	if (Newinfo.ID==-1)
	{
		QMessageBox::information(m_ManagerDialog, tr("Error"), tr("Could not increase more Image Item."), QMessageBox::Ok);
		return;
	}
	Newinfo.pDisplayDialog = new pe_qDisplayDialog(NULL);
	Newinfo.pDisplayDialog->setGeometry(x, y, width, height);
	Newinfo.pDisplayDialog->SetId(Newinfo.ID);
	Newinfo.visibled = false;
	Newinfo.pDisplayCollectionDialog = NULL;
	
	connect(Newinfo.pDisplayDialog, SIGNAL(SIGNAL_onMoving(QPoint, pe_qDisplayDialog *)), this, SLOT(SLOT_DisplayDialog_onMoving(QPoint,pe_qDisplayDialog *)));
	connect(Newinfo.pDisplayDialog, SIGNAL(SIGNAL_onDraggedDone(QPoint, pe_qDisplayDialog *)), this, SLOT(SLOT_DisplayDialog_onDraggedDone(QPoint, pe_qDisplayDialog *)));
	connect(Newinfo.pDisplayDialog, SIGNAL(SIGNAL_onClosed(int)), this, SLOT(SLOT_ItemDialogOnClosed(int)));
	connect(Newinfo.pDisplayDialog, SIGNAL(SIGNAL_onOpened(int)), this, SLOT(SLOT_ItemDialogOnOpened	(int)));
	connect(Newinfo.pDisplayDialog, SIGNAL(SIGNAL_onSave(int)), this, SLOT(SLOT_File_SaveDisplayDialogImage(int)));
	connect(Newinfo.pDisplayDialog, SIGNAL(SIGNAL_onAlphaIncreaseChanged(int,int)), this, SLOT(SLOT_ItemDialogOnAplhaChanged(int,int)));
	connect(Newinfo.pDisplayDialog, SIGNAL(SIGNAL_onCollapseAll()), this, SLOT(SLOT_ManagerDialog_onCollapseAll()));
	connect(Newinfo.pDisplayDialog, SIGNAL(SIGNAL_onDeleteThis(int)), this, SLOT(SLOT_SnapItem_DeleteItem(int)));
	m_DataInfo.push_back(Newinfo);
}


void PSnip::NewCollectionDialog(qint32 x, qint32 y, qint32 widht, qint32 height)
{
	PSnip_stItemInfo Newinfo;
	Newinfo.X = x;
	Newinfo.Y = y;
	Newinfo.resHeight = height;
	Newinfo.resWidth = widht;
	Newinfo.Alpha = 255;

	//////////////////////////////////////////////////////////////////////////
	//Update to GUI(imageList) &Get id
	Newinfo.ID = m_ManagerDialog->m_SnapshotList->addSnapshotItem(m_CollectionLogo,true);
	Newinfo.IsCollectionDialog = true;
	QSnapshotItem *item = m_ManagerDialog->m_SnapshotList->GetLastCreatedSnapshotItem();
	if (item)
	{
		item->SetName(tr("Collector"));
	}
	if (Newinfo.ID == -1)
	{
		QMessageBox::information(m_ManagerDialog, tr("Error"), tr("Could not increase more Image Item."), QMessageBox::Ok);
		return;
	}
	Newinfo.pDisplayCollectionDialog = new pe_qDisplayCollectionDialog(NULL);
	Newinfo.pDisplayCollectionDialog->setGeometry(x, y, widht, height);
	Newinfo.pDisplayCollectionDialog->SetShowSize(QSize(widht, height));
	Newinfo.pDisplayCollectionDialog->SetId(Newinfo.ID);
	Newinfo.visibled = false;
	Newinfo.pDisplayDialog = NULL;
	connect(Newinfo.pDisplayCollectionDialog, SIGNAL(SIGNAL_onClosed(int)), this, SLOT(SLOT_ItemDialogOnClosed(int)));
	connect(Newinfo.pDisplayCollectionDialog, SIGNAL(SIGNAL_onOpened(int)), this, SLOT(SLOT_ItemDialogOnOpened(int)));
	connect(Newinfo.pDisplayCollectionDialog, SIGNAL(SIGNAL_onAlphaIncreaseChanged(int, int)), this, SLOT(SLOT_ItemDialogOnAplhaChanged(int, int)));
	connect(Newinfo.pDisplayCollectionDialog, SIGNAL(SIGNAL_onMessageError(QString)), this, SLOT(SLOT_onMessageError(QString)));
	connect(Newinfo.pDisplayCollectionDialog, SIGNAL(SIGNAL_onCollapseAll()), this, SLOT(SLOT_ManagerDialog_onCollapseAll()));
	connect(Newinfo.pDisplayCollectionDialog, SIGNAL(SIGNAL_onDeleteThis(int)), this, SLOT(SLOT_SnapItem_DeleteItem(int)));
	m_DataInfo.push_back(Newinfo);
}

void PSnip::ShowProgressMsg()
{
	m_ManagerDialog->m_ImageDialog.widget_Progress_Msg->show();
	m_ManagerDialog->m_ImageDialog.progressBar_Mission->show();
}

void PSnip::HideProgressMsgProgressBar()
{
	m_ManagerDialog->m_ImageDialog.progressBar_Mission->hide();
}

void PSnip::HideProgressMsg()
{
	m_ManagerDialog->m_ImageDialog.widget_Progress_Msg->hide();
}

void PSnip::SLOT_ItemDialogOnClosed(int ID)
{
	PSnip_stItemInfo *Info = GetSnapshotImageInfoByID(ID);
	if (!Info)
	{
		return;
	}
	Info->visibled = false;
	QSnapshotItem *pItem = m_ManagerDialog->m_SnapshotList->GetSnapshotItemByID(ID);
	if (pItem)
	{
		pItem->SetOpenMode(true);
	}
}



void PSnip::SLOT_ItemDialogOnOpened(int ID)
{
	PSnip_stItemInfo *Info = GetSnapshotImageInfoByID(ID);
	if (!Info)
	{
		return;
	}
	Info->visibled = true;
	QSnapshotItem *pItem = m_ManagerDialog->m_SnapshotList->GetSnapshotItemByID(ID);
	if (pItem)
	{
		pItem->SetOpenMode(false);
	}
}

void PSnip::SLOT_ItemDialogOnAplhaChanged(int ID,int val)
{
	QSnapshotItem *pItem = m_ManagerDialog->m_SnapshotList->GetSnapshotItemByID(ID);
	if (pItem)
	{
		pItem->horizontalSlider->setValue(pItem->horizontalSlider->value() + val);
	}
}

void PSnip::SLOT_DisplayDialogCut(int ID)
{
	PSnip_stItemInfo *Info = GetSnapshotImageInfoByID(ID);
	if (!Info)
	{
		return;
	}
	if (Info->IsCollectionDialog)
	{
		return;
	}
	Info->pDisplayDialog->SLOT_Shortcut_CutMode();
	Info->pDisplayDialog->activateWindow();

}

void PSnip::SLOT_SnapShot_BeginSnip()
{
	HideAllDisplayDialog();
}

void PSnip::SLOT_SnapShot_ExitSnip()
{
	if (m_LastManagerVisible)
	{
		m_ManagerDialog->setVisible(true);
	}
	RestoreAllDisplayDialog();
}

void PSnip::SLOT_PasteClipboardImage()
{
	QPixmap pixmap = QApplication::clipboard()->pixmap();
	if (!pixmap.isNull())
	{
		AddPixmapToItem(pixmap);
	}
}

void PSnip::SLOT_DragEvent_DragImage(QPixmap dragPixmap)
{
	QPixmap pixmap = dragPixmap;
	if (!pixmap.isNull())
	{
		QRect rect = pixmap.rect();

		AddImageItem(rect.x(), rect.y(), rect.width(), rect.height(), pixmap);

		PSnip_stItemInfo *pItem = GetLastCreatedItemInfoPointer();
		if (!pItem)
		{
			return;
		}
		pItem->pDisplayDialog->SnapshotDisplayImage(rect, pixmap);
		pItem->visibled = true;

	}
	else
	{
		m_trayIcon->showMessage(tr("ERROR"), tr("The image is invalid"), QSystemTrayIcon::Warning);
	}
}

void PSnip::SLOT_DragEvent_DragFile(QString Path)
{
	QFileInfo info(Path);
	QString suffix = info.suffix();
	if (suffix.toUpper()==QString("PSS"))
	{
		if (LoadProject(Path))
			return;
	}
	QPixmap pixmap(Path);
	if (!pixmap.isNull())
	{
		QRect rect = pixmap.rect();

		AddImageItem(rect.x(), rect.y(), rect.width(), rect.height(), pixmap);

		PSnip_stItemInfo *pItem = GetLastCreatedItemInfoPointer();
		if (!pItem)
		{
			return;
		}
		pItem->pDisplayDialog->SnapshotDisplayImage(rect, pixmap);
		pItem->visibled = true;
		QString Name = QFileInfo(Path).fileName();
		m_ManagerDialog->m_SnapshotList->GetSnapshotItemByID(pItem->ID)->SetName(Name);
	}
	else
	{
		m_trayIcon->showMessage(tr("ERROR"), tr("The file is invalid"), QSystemTrayIcon::Warning);
	}
}

void PSnip::SLOT_DragEvent_DragUrl(QString url)
{
	ShowProgressMsg();
	HideProgressMsgProgressBar();
	SLOT_Progress(tr("Network image downloading...."), 1, 1);
	m_UrlDownloader.Download(url.toLocal8Bit().data(), "./cache");
}

void PSnip::SLOT_NewWork_URLDownloadCompleted()
{
		QPixmap pixmap("./Cache");
		DeleteFile(L"Cache");

		if (!pixmap.isNull())
		{
			QRect rect = pixmap.rect();

			AddImageItem(rect.x(), rect.y(), rect.width(), rect.height(), pixmap);

			PSnip_stItemInfo *pItem = GetLastCreatedItemInfoPointer();
			if (!pItem)
			{
				return;
			}
			pItem->pDisplayDialog->SnapshotDisplayImage(rect, pixmap);
			pItem->visibled = true;
			m_ManagerDialog->m_SnapshotList->GetSnapshotItemByID(pItem->ID)->SetName(tr("url image"));
		}
		else
		{
			m_trayIcon->showMessage(tr("ERROR"), tr("URL download failed."), QSystemTrayIcon::Warning);
		}

	HideProgressMsg();
}

void PSnip::SLOT_File_SaveDisplayDialogImage(int id)
{
	bool ManagerDialogIsVisibled = false;
	if (m_ManagerDialog->isVisible())
	{
		ManagerDialogIsVisibled = true;
	}
	m_ManagerDialog->show();
	QString fileName = QFileDialog::getSaveFileName(m_ManagerDialog,
		tr("Image Save"),
		"",
		tr("Image Files (*.png *.jpg *.bmp *.jpeg *.ppm *.tiff *.xbm *.xpm)"));

	if (!fileName.isNull())
	{
		PSnip_stItemInfo *Info = GetSnapshotImageInfoByID(id);
		if (Info)
		{
			if (!Info->pDisplayDialog->GetCurrentDisplayPixmap().save(fileName))
				QMessageBox::information(m_ManagerDialog, tr("error"), tr("Could not save image file."), QMessageBox::Ok);
		}
	}
	if (!ManagerDialogIsVisibled)
	{
		m_ManagerDialog->hide();
	}
}

void PSnip::SLOT_File_SaveSourceImage(int id)
{
	bool ManagerDialogIsVisibled = false;
	if (m_ManagerDialog->isVisible())
	{
		ManagerDialogIsVisibled = true;
	}
	m_ManagerDialog->show();
	QString fileName = QFileDialog::getSaveFileName(m_ManagerDialog,
		tr("Image Save"),
		"",
		tr("Image Files (*.png *.jpg *.bmp *.jpeg *.ppm *.tiff *.xbm *.xpm)"));

	if (!fileName.isNull())
	{
		PSnip_stItemInfo *Info = GetSnapshotImageInfoByID(id);
		if (Info)
		{
			if (!Info->resPixmap.save(fileName))
				QMessageBox::information(m_ManagerDialog, tr("error"), tr("Could not save image file."), QMessageBox::Ok);
		}
	}
	if (!ManagerDialogIsVisibled)
	{
		m_ManagerDialog->hide();
	}
}

void PSnip::SLOT_ManagerDialog_onImageItemsListView()
{
	ViewOnSnapshotImageList();
}

void PSnip::SLOT_ManagerDialog_onCollectorListView()
{
	ViewOnSnapShotCollectorList();
}

void PSnip::SLOT_ManagerDialog_onConfigView()
{
	ViewOnConfiguration();
}

void PSnip::SLOT_ManagerDialog_onCollapseAll()
{
	CollapseAll();
}

void PSnip::SLOT_ManagerDialog_onTakeSnapShot()
{
	TakeSnip();
}

void PSnip::SLOT_ManagerDialog_onTakeSelectArea()
{
	TakeView();
}

void PSnip::SLOT_ManagerDialog_onSaveProject()
{
	if (!SaveProject())
	{
		QMessageBox::information(m_ManagerDialog,tr("Error"),tr("Could not saved project file!"),QMessageBox::Ok);
	}
}

void PSnip::SLOT_ManagerDialog_onOpenProject()
{
	if (!OpenProject())
	{
		QMessageBox::information(m_ManagerDialog, tr("Error"), tr("Could not opened project file!"), QMessageBox::Ok);
	}
}

void PSnip::SLOT_ManagerDialog_onImport()
{
	ImportImage();
}

void PSnip::SLOT_ManagerDialog_OnConfigApply()
{
	ApplyConfig();
}

void PSnip::SLOT_ManagerDialog_OnConfigRestore()
{
	RestoreConfig();
}

void PSnip::SLOT_DisplayDialog_onMoving(QPoint point, pe_qDisplayDialog * dlg)
{
	bool HasRegion=false;
	for (int i=0;i<m_DataInfo.size();i++)
	{
		if (m_DataInfo[i].IsCollectionDialog)
		{
			if (m_DataInfo[i].pDisplayCollectionDialog->geometry().contains(point))
			{
				if (m_DataInfo[i].pDisplayCollectionDialog->isVisible()) 
				{
					dlg->EnterDragOpacity();
					m_DataInfo[i].pDisplayCollectionDialog->EnterPreview(dlg->GetSourcePixmap());
					HasRegion = true;
					break;
				}
			}
			else
			{
				m_DataInfo[i].pDisplayCollectionDialog->ExitPreview();
			}
		}
	}
	if (!HasRegion)
	{
		dlg->ExitDragOpacity();
	}
}

void PSnip::SLOT_DisplayDialog_onDraggedDone(QPoint point, pe_qDisplayDialog *dlg)
{
	for (int i = 0; i < m_DataInfo.size(); i++)
	{
		if (m_DataInfo[i].IsCollectionDialog)
		{
			if (m_DataInfo[i].pDisplayCollectionDialog->isVisible())
			{
				if (m_DataInfo[i].pDisplayCollectionDialog->geometry().contains(point))
				{
					dlg->RestoreMoved();
					dlg->CloseDirectly();
					m_DataInfo[i].pDisplayCollectionDialog->AddPixmap(dlg->GetSourcePixmap());
				}
				m_DataInfo[i].pDisplayCollectionDialog->ExitPreview();
			}
		}
	}
}

void PSnip::SLOT_MiniSwitch()
{
	if (m_ManagerDialog->isVisible())
	{
		m_ManagerDialog->hide();
	}
	else
	{
		m_ManagerDialog->show();
		m_ManagerDialog->showNormal();
	}
}

void PSnip::SLOT_Quit()
{
	QMessageBox::StandardButton btnReply;
	bool ManagerDialogIsVisibled = false;
	if (m_ManagerDialog->isVisible())
	{
		ManagerDialogIsVisibled = true;
	}
	m_ManagerDialog->show();
	if (m_DataInfo.size() != 0)
	{
		btnReply = QMessageBox::information(m_ManagerDialog, tr("Query"), tr("Save(Update) this Project to disk?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	}
	if (btnReply==QMessageBox::Cancel)
	{
		if (!ManagerDialogIsVisibled)
			m_ManagerDialog->hide();
		return;
	}
	if (btnReply== QMessageBox::Yes)
	{
		if (m_ProjPath.isNull())
		{
			SaveProject();
		}
		else
		{
			SaveProject(m_ProjPath);
		}
	}
	exit(0);
}

void PSnip::SLOT_SwitchToEN()
{
	SwitchLanguage(PSNIP_LANGUAGE_EN);
}

void PSnip::SLOT_SwitchToZh()
{
	SwitchLanguage(PSNIP_LANGUAGE_ZH);
}

void PSnip::SLOT_Progress(QString Msg, int prog, int current)
{
	m_ManagerDialog->m_ImageDialog.label_progerss->setText(Msg);
	if (m_ManagerDialog->m_ImageDialog.progressBar_Mission->maximum()!=prog)
	{
		m_ManagerDialog->m_ImageDialog.progressBar_Mission->setMaximum(prog);
	}
	if (m_ManagerDialog->m_ImageDialog.progressBar_Mission->value() != current)
	{
		m_ManagerDialog->m_ImageDialog.progressBar_Mission->setValue(current);
	}
	m_ManagerDialog->update();
}

void PSnip::SLOT_OnMiniActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:
		break;
	case QSystemTrayIcon::DoubleClick:
		SLOT_MiniSwitch();
		break;
	case QSystemTrayIcon::MiddleClick:
		break;
	case QSystemTrayIcon::Context:
		break;
	default:
		;
	}
}

void PSnip::SLOT_onMessageError(QString msg)
{
	m_trayIcon->showMessage(tr("Error"), msg, QSystemTrayIcon::NoIcon);
}

PSnip_stItemInfo * PSnip::GetLastCreatedItemInfoPointer()
{
	if (m_DataInfo.size()==0)
	{
		return NULL;
	}
	return &m_DataInfo[m_DataInfo.size()-1];
}



void PSnip::SLOT_HotKey_ScreenShot()
{
	if (m_ScreenDialog!=NULL)
	{
		//Activate ScreenShot
		TakeSnip();
	}
}

void PSnip::SLOT_HotKey_ScreenSelect()
{
	if (m_ScreenDialog != NULL)
	{
		//Activate Screen Select
		TakeView();
	}
}

void PSnip::SLOT_Snip_CaptureDone()
{
	QPixmap pixmap=m_ScreenDialog->getSnapshotPixmap();
	QRect rect = m_ScreenDialog->getRect();
	if (m_LastManagerVisible)
	{
		m_ManagerDialog->setVisible(true);
	}

	AddImageItem(rect.x(), rect.y(), rect.width(), rect.height(), pixmap);

	PSnip_stItemInfo *pItem = GetLastCreatedItemInfoPointer();
	if (!pItem)
	{
		return;
	}
	pItem->pDisplayDialog->SnapshotDisplayImage(rect, pixmap);
	pItem->pDisplayDialog->activateWindow();
	pItem->visibled = true;

	
	
}

void PSnip::SLOT_Select_SelectDone()
{
	QRect rect = m_ScreenDialog->getRect();
	if (m_LastManagerVisible)
	{
		m_ManagerDialog->setVisible(true);
	}

	NewCollectionDialog(rect.x(), rect.y(), rect.width(), rect.height());

	PSnip_stItemInfo *pItem = GetLastCreatedItemInfoPointer();
	if (!pItem)
	{
		return;
	}
	pItem->pDisplayCollectionDialog->ShowDirectly();
	pItem->pDisplayCollectionDialog->activateWindow();
	pItem->visibled = true;
}

void PSnip::SLOT_Dialog_ShowManagerDialog()
{
	ShowManagerDialog();
}

void PSnip::SLOT_SnapItem_DisplaySwitch(int ID)
{
	PSnip_stItemInfo *Info = GetSnapshotImageInfoByID(ID);
	if (!Info)
	{
		return;
	}

	if (Info->visibled)
		{
			if (Info->IsCollectionDialog)
			Info->pDisplayCollectionDialog->CloseAction();
			else
			Info->pDisplayDialog->CloseAction();
		}
		else
		{
			if (Info->IsCollectionDialog)
				Info->pDisplayCollectionDialog->ShowAction();
			else
			Info->pDisplayDialog->ShowAction();
		}
	
}

void PSnip::SLOT_SnapItem_DeleteItem(int ID)
{
	RemoveItemInfo(ID);
}

void PSnip::SLOT_SnapItem_RestoreDialogSize(int ID)
{
	PSnip_stItemInfo *Info = GetSnapshotImageInfoByID(ID);
	if (!Info)
	{
		return;
	}
	if (Info->IsCollectionDialog)
		Info->pDisplayCollectionDialog->RestoreCurrentDisplay();
	else
		Info->pDisplayDialog->RestoreDisplay();
}

void PSnip::SLOT_SnapItem_SetAlphaValue(int id, int value)
{
	PSnip_stItemInfo *info = GetSnapshotImageInfoByID(id);
	if (!info)
	{
		return;
	}
	if (info->visibled)
		if (info->IsCollectionDialog)
			info->pDisplayCollectionDialog->SetOpacity(value);
		else
			info->pDisplayDialog->SetOpacity(value);
}

void PSnip::SLOT_SnapItem_MouseFocus(int id)
{
	PSnip_stItemInfo *info = GetSnapshotImageInfoByID(id);
	if (!info)
	{
		return;
	}
	if (info->visibled)
		if(info->IsCollectionDialog)
		info->pDisplayCollectionDialog->SetBorderColor(QColor(255, 0, 0));
		else
		info->pDisplayDialog->SetBorderColor(QColor(255,0,0));
}

void PSnip::SLOT_SnapItem_MouseLostFocus(int id)
{
	PSnip_stItemInfo *info = GetSnapshotImageInfoByID(id);
	if (!info)
	{
		return;
	}
	if(info->visibled)
		if (info->IsCollectionDialog)
			info->pDisplayCollectionDialog->SetBorderColor(QColor(0, 0, 0));
		else
			info->pDisplayDialog->SetBorderColor(QColor(0, 0, 0));
}

void PSnip::SLOT_SnapItem_EnterPickMode(int id)
{
	PSnip_stItemInfo *info = GetSnapshotImageInfoByID(id);
	if (!info)
	{
		return;
	}
	if (info->visibled)
	{
		if (info->IsCollectionDialog)
		{
			info->pDisplayCollectionDialog->EnterPickMode();
			info->pDisplayCollectionDialog->activateWindow();
		}
		else
		{
			info->pDisplayDialog->EnterPickMode();
			info->pDisplayDialog->activateWindow();
		}
		
	}
}

pe_ManagerDialog::pe_ManagerDialog(QDialog *parent /*= NULL*/) :QDialog(parent)
{
	m_bWindowStayOnTop = true;
	m_bMoving = false;
	m_SnapshotList=NULL;
	m_CfgWidgetContent=NULL;
	
	this->setModal(false);
	
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	
	m_ImageDialog.setupUi(this);
	
	m_SnapshotList = new pe_SnapshotList(m_ImageDialog.widget_Content);
	m_SnapshotList->setVisible(true);
	m_ImageDialog.verticalLayout_WidgetContent->addWidget(m_SnapshotList);
	m_CfgWidgetContent = new QWidget(m_ImageDialog.widget_Content);
	m_ConfigWidget.setupUi(m_CfgWidgetContent);
	m_ImageDialog.verticalLayout_WidgetContent->addWidget(m_CfgWidgetContent);
	m_CfgWidgetContent->setVisible(false);
	
	
	//////////////////////////////////////////////////////////////////////////
	//Connections
	//////////////////////////////////////////////////////////////////////////
	connect(m_ImageDialog.pushButton_Stay, SIGNAL(clicked()), this, SLOT(SLOT_onButtonStay()));
	connect(m_ImageDialog.pushButton_unpin, SIGNAL(clicked()), this, SLOT(SLOT_onButtonUnpin()));
	connect(m_ImageDialog.pushButton_CollapseAll, SIGNAL(clicked()), this, SLOT(SLOT_onButtonCollapseAll()));
	connect(m_ImageDialog.pushButton_ConfigView, SIGNAL(clicked()), this, SLOT(SLOT_onButtonConfigView()));
	connect(m_ImageDialog.pushButton_ImageView, SIGNAL(clicked()), this, SLOT(SLOT_onButtonListView()));
	//connect(m_ImageDialog.pushButton_Save, SIGNAL(clicked()), this, SLOT(SLOT_onButtonSave()));
	connect(m_ImageDialog.pushButton_TakeSnip, SIGNAL(clicked()), this, SLOT(SLOT_onButtonTakeSnip()));
	connect(m_ImageDialog.pushButton_SelectArea, SIGNAL(clicked()), this, SLOT(SLOT_onButtonTakeSelect()));
	connect(m_ImageDialog.pushButton_Collection, SIGNAL(clicked()), this, SLOT(SLOT_onButtonCollectorView()));

	connect(m_ConfigWidget.pushButton_Apply, SIGNAL(clicked()), this, SLOT(SLOT_onButtonConfigApply()));
	connect(m_ConfigWidget.pushButton_Restore, SIGNAL(clicked()), this, SLOT(SLOT_onButtonConfigRestore()));

	connect(m_ConfigWidget.lineEdit_CollectorView, SIGNAL(textChanged(QString)), this, SLOT(SLOT_onConfigChanged()));
	connect(m_ConfigWidget.lineEdit_ManagerDialog, SIGNAL(textChanged(QString)), this, SLOT(SLOT_onConfigChanged()));
	connect(m_ConfigWidget.lineEdit_ScreenShot, SIGNAL(textChanged(QString)), this, SLOT(SLOT_onConfigChanged()));

	connect(m_ConfigWidget.radioButton_la_en, SIGNAL(clicked()), this, SLOT(SLOT_onSwitchLanguage()));
	connect(m_ConfigWidget.radioButton_la_zh, SIGNAL(clicked()), this, SLOT(SLOT_onSwitchLanguage()));
	//////////////////////////////////////////////////////////////////////////
	//Drag enable
	//////////////////////////////////////////////////////////////////////////
	this->setAcceptDrops(true);

	///////////////////////////////////////////////////////////////////////////
	//Menu
	m_menu = new QMenu(this);


	///////////////////////////////////////////////////////////////////////////
	//Actions
	 m_Menu_OpenProject= new QAction(this);
	 m_Menu_ClipboardPaste = new QAction(this);
	 m_Menu_CollapseAll = new QAction(this);
	 m_Menu_SaveProject = new QAction(this);
	 m_Menu_importFile = new QAction(this);

	 m_Menu_OpenProject->setText(tr("Open Project"));
	 m_Menu_ClipboardPaste->setText(tr("Paste Image"));
	 m_Menu_importFile->setText(tr("Import Image file"));
	 m_Menu_SaveProject->setText(tr("Save Project"));
	 m_Menu_CollapseAll->setText(tr("Collapse All"));

	connect(m_Menu_OpenProject, SIGNAL(triggered()), this, SLOT(SLOT_Menu_OnOpenProject()));
	connect(m_Menu_ClipboardPaste, SIGNAL(triggered()), this, SLOT(SLOT_Menu_OnClipboardPaste()));
	connect(m_Menu_importFile, SIGNAL(triggered()), this, SLOT(SLOT_Menu_importFile()));
	connect(m_Menu_SaveProject, SIGNAL(triggered()), this, SLOT(SLOT_Menu_OnSavedProject()));
	connect(m_Menu_CollapseAll, SIGNAL(triggered()), this, SLOT(SLOT_Menu_OnCollapseAll()));

	m_ImageDialog.pushButton_CollapseAll->setToolTip(tr("Collapse all"));
	m_ImageDialog.pushButton_SelectArea->setToolTip(tr("Create collector viewer"));
	m_ImageDialog.pushButton_TakeSnip->setToolTip(tr("Take screen snapshot"));
	m_ImageDialog.pushButton_ImageView->setToolTip(tr("Snipped Images"));
	m_ImageDialog.pushButton_Collection->setToolTip(tr("Collectors"));
	m_ImageDialog.pushButton_ConfigView->setToolTip(tr("Configurations"));

	SetStaytoTop(m_bWindowStayOnTop);
	this->hide();

}

void pe_ManagerDialog::mouseMoveEvent(QMouseEvent *event)
{
	if (m_bMoving)
		this->move(event->globalPos() - m_OffsetVector);
}

void pe_ManagerDialog::mousePressEvent(QMouseEvent *event)
{
	switch (event->button()) {
	case Qt::LeftButton:
		m_bMoving = true;
		m_OffsetVector = event->globalPos() - this->frameGeometry().topLeft();
		break;
	default:
		QDialog::mousePressEvent(event);
	}
}

void pe_ManagerDialog::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMoving = false;
		this->releaseMouse();
	}
}

void pe_ManagerDialog::paintEvent(QPaintEvent * event)
{
	QPainter Pt(this);
	QPen pen(QColor(0, 0, 0));
	pen.setWidth(2);
	Pt.fillRect(QRect(0, 0, width() - 1, height() - 1), QColor(255, 255, 255));
	Pt.setPen(pen);
	Pt.drawRect(QRect(0, 0, width(), height()));
}

void pe_ManagerDialog::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void pe_ManagerDialog::dropEvent(QDropEvent *event)
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

void pe_ManagerDialog::dragMoveEvent(QDragMoveEvent *event)
{
	event->acceptProposedAction();
}

void pe_ManagerDialog::dragLeaveEvent(QDragLeaveEvent *event)
{
	event->accept();
}

void pe_ManagerDialog::showEvent(QShowEvent *event)
{
	m_SnapshotList->m_ListitemsUpdateAnimationTimer.start(10);
}

void pe_ManagerDialog::contextMenuEvent(QContextMenuEvent * event)
{
	ExecMenu(event->globalPos());
}

void pe_ManagerDialog::SetStaytoTop(bool Enable)
{
	m_bWindowStayOnTop = Enable;
	if (m_bWindowStayOnTop)
	{
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
		m_ImageDialog.pushButton_Stay->show();
		m_ImageDialog.pushButton_unpin->hide();
	}
	else
	{
		this->setWindowFlags(Qt::FramelessWindowHint);
		m_ImageDialog.pushButton_Stay->hide();
		m_ImageDialog.pushButton_unpin->show();
	}
	show();
}

void pe_ManagerDialog::ExecMenu(QPoint pos)
{
	m_menu->clear();
	m_menu->addAction(m_Menu_OpenProject);
	m_menu->addAction(m_Menu_SaveProject);
	m_menu->addAction(m_Menu_importFile);
	m_menu->addAction(m_Menu_ClipboardPaste);
	m_menu->addAction(m_Menu_CollapseAll);
	m_menu->exec(pos);
}

void pe_ManagerDialog::ResetLanguage()
{
	//Menus
	m_Menu_OpenProject->setText(tr("Open Project"));
	m_Menu_ClipboardPaste->setText(tr("Paste Image"));
	m_Menu_importFile->setText(tr("Import Image file"));
	m_Menu_SaveProject->setText(tr("Save Project"));
	m_Menu_CollapseAll->setText(tr("Collapse All"));
	//Tooltips
	m_ImageDialog.pushButton_CollapseAll->setToolTip(tr("Collapse all"));
	m_ImageDialog.pushButton_SelectArea->setToolTip(tr("Create collector viewer"));
	m_ImageDialog.pushButton_TakeSnip->setToolTip(tr("Take screen snapshot"));
	m_ImageDialog.pushButton_ImageView->setToolTip(tr("Snipped Images"));
	m_ImageDialog.pushButton_Collection->setToolTip(tr("Collectors"));
	m_ImageDialog.pushButton_ConfigView->setToolTip(tr("Configurations"));

	m_ImageDialog.retranslateUi(this);
	m_ConfigWidget.retranslateUi(this->m_CfgWidgetContent);
}

void pe_ManagerDialog::SLOT_onSwitchLanguage()
{
	if (m_ConfigWidget.radioButton_la_en->isChecked())
	{
		emit SIGNAL_onLanguageSwitch(PSNIP_LANGUAGE_EN);
	}
	if (m_ConfigWidget.radioButton_la_zh->isChecked())
	{
		emit SIGNAL_onLanguageSwitch(PSNIP_LANGUAGE_ZH);
	}
}


QUrlDownloader::QUrlDownloader(QObject * parent) : QThread(parent) {

}

QUrlDownloader::~QUrlDownloader() {

}

int QUrlDownloader::Download(QString url, QString downloadFile)
{
	m_url = url;
	m_downloadFile = downloadFile;

	DeleteUrlCacheEntryA(m_url.toLocal8Bit().data());

	start();
	return 0;
}

void QUrlDownloader::run()
{
	URLDownloadToFileA(NULL, m_url.toLocal8Bit().data(), m_downloadFile.toLocal8Bit().data(), 0, NULL);
	emit SIGNAL_DownloadCompleted();
}
