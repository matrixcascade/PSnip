#pragma once
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include <qsnapshotitem.h>
#include <QTimer>
#define  QSNAPSHOTLIST_LISTITEM_MAXCOUNT		128
#define  QSNAPSHOTLIST_LISTITEM_VERTICAL_STEP	4
#define  QSNAPSHOTLIST_LISTITEM_HORIZONTAL_STEP 32

#define  QSNAPSHOTLIST_LISTITEM_SHOWMODE_ALL			0 
#define  QSNAPSHOTLIST_LISTITEM_SHOWMODE_IMAGEITEMS		1 
#define  QSNAPSHOTLIST_LISTITEM_SHOWMODE_COLLECTOR	    2 
struct SnapshotItemInfo
{
	unsigned int  ID;
	QSnapshotItem *item;
	SnapshotItemInfo()
	{
		ID = -1;
		item = NULL;
	}
};


class pe_SnapshotList : public QScrollArea {
	Q_OBJECT

public:
	pe_SnapshotList(QWidget * parent = NULL);
	~pe_SnapshotList();

	void			Clear();
	unsigned int	addSnapshotItem(QPixmap pixmap,bool IsCollector=false);
	bool			removeSnapshotItem(int ID);
	unsigned int	getSnapshotItemsCount();
	QSnapshotItem  *GetLastCreatedSnapshotItem();
	QSnapshotItem  *GetSnapshotItemByID(int ID);

	void			ShowMode_All();
	void			ShowMode_ImageItems();
	void			ShowMode_Collector();

	QWidget *scrollAreaWidgetContents;
	QVBoxLayout *verticalLayout;
	QWidget *widget;
	QTimer m_ListitemsUpdateAnimationTimer;
	int m_AllocID;
	int m_ShowMode;
	SnapshotItemInfo m_SnapshotItems[QSNAPSHOTLIST_LISTITEM_MAXCOUNT];
	unsigned int m_RemoveTable[QSNAPSHOTLIST_LISTITEM_MAXCOUNT];
public slots:
	void SLOT_UpdateAnimationTime();
	//override
	void SLOT_onItemOpen(int);
	void SLOT_onItemClose(int);
	void SLOT_onItemDelete(int);
	void SLOT_onItemCut(int);
	void SLOT_onItemScale(int);
	void SLOT_onItemPick(int);
	void SLOT_onItemSave(int);
	void SLOT_onItemRestore(int);
	void SLOT_onItemAlphaChange(int, int);
	void SLOT_onItemMousedown(int);
	void SLOT_onItemMouseFocus(int);
	void SLOT_onItemMouseLostFocus(int);
signals:
	void SIGNAL_onItemDisplaySwitch(int);
	void SIGNAL_onItemOpen(int);
	void SIGNAL_onItemClose(int);
	void SIGNAL_onItemDelete(int);
	void SIGNAL_onItemCut(int);
	void SIGNAL_onItemScale(int);
	void SIGNAL_onItemPick(int);
	void SIGNAL_onItemSave(int);
	void SIGNAL_onItemRestore(int);
	void SIGNAL_onItemAlphaChange(int, int);
	void SIGNAL_onItemMouseFocus(int);
	void SIGNAL_onItemMouseLostFocus(int);
private:
	void RemoveSnapShotItemFromBufferTable(unsigned int Index);
};
