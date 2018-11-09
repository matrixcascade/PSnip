#include "pe_snapshotlist.h"
QSnapshotItem *sNewItem ;
pe_SnapshotList::pe_SnapshotList(QWidget * parent) : QScrollArea(parent) {

	m_ShowMode=QSNAPSHOTLIST_LISTITEM_SHOWMODE_ALL;
	setWidgetResizable(true);
	scrollAreaWidgetContents = new QWidget();
	scrollAreaWidgetContents->setObjectName(("pelsscrollAreaWidgetContents"));
	verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
	verticalLayout->setSpacing(0);
	verticalLayout->setContentsMargins(0, 0, 0, 0);
	verticalLayout->setObjectName(("pelsverticalLayout"));
	widget = new QWidget(scrollAreaWidgetContents);
	widget->setObjectName(("pelswidget"));
	verticalLayout->addWidget(widget);
	this->setWidget(scrollAreaWidgetContents);
	
	//////////////////////////////////////////////////////////////////////////
	//Various Initialization
	m_AllocID = 1;

	//////////////////////////////////////////////////////////////////////////
	//Table Initialize
	for (int i=0;i<QSNAPSHOTLIST_LISTITEM_MAXCOUNT;i++)
	{
		m_RemoveTable[i] = -1;
	}

	QPalette pal(scrollAreaWidgetContents->palette());

	pal.setColor(QPalette::Background, Qt::white);
	scrollAreaWidgetContents->setAutoFillBackground(true);
	scrollAreaWidgetContents->setPalette(pal);

	//////////////////////////////////////////////////////////////////////////
	//Connection declare
	connect(&m_ListitemsUpdateAnimationTimer, SIGNAL(timeout()), this, SLOT(SLOT_UpdateAnimationTime()));
}

pe_SnapshotList::~pe_SnapshotList() {
	
}


void pe_SnapshotList::Clear()
{
	for (int i = 0; i < QSNAPSHOTLIST_LISTITEM_MAXCOUNT; i++)
	{
		if (m_SnapshotItems[i].item != NULL)
		{
			delete m_SnapshotItems[i].item;
		}
		m_SnapshotItems[i] = SnapshotItemInfo();
	}
	
}

unsigned int pe_SnapshotList::addSnapshotItem(QPixmap pixmap, bool IsCollector)
{
	if (getSnapshotItemsCount()>=QSNAPSHOTLIST_LISTITEM_MAXCOUNT)
	{
		return -1;
	}
	if (m_AllocID<0)
	{
		return -1;
	}
	SnapshotItemInfo Temp[QSNAPSHOTLIST_LISTITEM_MAXCOUNT];
	memcpy(Temp+1, m_SnapshotItems, getSnapshotItemsCount()*sizeof(SnapshotItemInfo));
	memcpy(m_SnapshotItems, Temp, sizeof(Temp));
	QSnapshotItem *NewItem = new QSnapshotItem(widget);
	NewItem->move(-NewItem->width(), 0);
	NewItem->setVisible(true);
	NewItem->SetMiniPixmap(pixmap);
	if (NewItem==NULL)
	{
		return -1;
	}
	m_SnapshotItems[0].item = NewItem;
	m_SnapshotItems[0].ID = m_AllocID++;
	NewItem->SetID(m_SnapshotItems[0].ID);
	if (IsCollector)
	{
		NewItem->AsCollectionItem();
		if (m_ShowMode!=QSNAPSHOTLIST_LISTITEM_SHOWMODE_ALL&&m_ShowMode!=QSNAPSHOTLIST_LISTITEM_SHOWMODE_COLLECTOR)
		{
			NewItem->setVisible(false);
		}
	}
	else
	{
		if (m_ShowMode != QSNAPSHOTLIST_LISTITEM_SHOWMODE_ALL&&m_ShowMode != QSNAPSHOTLIST_LISTITEM_SHOWMODE_IMAGEITEMS)
		{
			NewItem->setVisible(false);
		}
	}
	

	//connections
	connect(NewItem, SIGNAL(SIGNAL_onAlphaChange(int, int)), this, SLOT(SLOT_onItemAlphaChange(int, int)));
	connect(NewItem, SIGNAL(SIGNAL_onOpen(int)), this, SLOT(SLOT_onItemOpen(int)));
	connect(NewItem, SIGNAL(SIGNAL_onClose(int)), this, SLOT(SLOT_onItemClose(int)));
	connect(NewItem, SIGNAL(SIGNAL_onCut(int)), this, SLOT(SLOT_onItemCut(int)));
	connect(NewItem, SIGNAL(SIGNAL_onPick(int)), this, SLOT(SLOT_onItemPick(int)));
	connect(NewItem, SIGNAL(SIGNAL_onSave(int)), this, SLOT(SLOT_onItemSave(int)));
	connect(NewItem, SIGNAL(SIGNAL_onScale(int)), this, SLOT(SLOT_onItemScale(int)));
	connect(NewItem, SIGNAL(SIGNAL_onRestore(int)), this, SLOT(SLOT_onItemRestore(int)));
	connect(NewItem, SIGNAL(SIGNAL_onDelete(int)), this, SLOT(SLOT_onItemDelete(int)));
	connect(NewItem, SIGNAL(SIGNAL_onMousedown(int)), this, SLOT(SLOT_onItemMousedown(int)));
	connect(NewItem, SIGNAL(SIGNAL_onMouseFocus(int)), this, SLOT(SLOT_onItemMouseFocus(int)));
	connect(NewItem, SIGNAL(SIGNAL_onMouseLostFocus(int)), this, SLOT(SLOT_onItemMouseLostFocus(int)));
	m_ListitemsUpdateAnimationTimer.start(10);
	return m_AllocID-1;
}


bool pe_SnapshotList::removeSnapshotItem(int ID)
{
	if (ID<0)
	{
		return false;
	}
	int Index = 0;
	while (m_RemoveTable[Index]!=-1&&Index<QSNAPSHOTLIST_LISTITEM_MAXCOUNT)
	{
		if (m_SnapshotItems[m_RemoveTable[Index]].ID==ID)
		{
			return true;
		}
		Index++;
	}
	for (int i = 0; i<QSNAPSHOTLIST_LISTITEM_MAXCOUNT; i++)
	{
		if (m_SnapshotItems[i].ID==ID)
		{
			for (int j = 0; i<QSNAPSHOTLIST_LISTITEM_MAXCOUNT; j++)
			{
				if (m_RemoveTable[j]==-1)
				{
					m_RemoveTable[j] = i;
					m_ListitemsUpdateAnimationTimer.start(10);
					return true;
				}
			}
			return false;
		}
	}
	return false;
}

unsigned int pe_SnapshotList::getSnapshotItemsCount()
{
	for (int i=0;i<QSNAPSHOTLIST_LISTITEM_MAXCOUNT;i++)
	{
		if (m_SnapshotItems[i].item==NULL)
		{
			return i;
		}
	}
	return QSNAPSHOTLIST_LISTITEM_MAXCOUNT;
}


QSnapshotItem * pe_SnapshotList::GetLastCreatedSnapshotItem()
{
	return m_SnapshotItems[0].item;
}

QSnapshotItem * pe_SnapshotList::GetSnapshotItemByID(int ID)
{
	int i = 0;
	while (m_SnapshotItems[i].item!=NULL&&i<QSNAPSHOTLIST_LISTITEM_MAXCOUNT)
	{
		if (m_SnapshotItems[i].ID==ID)
		{
			return m_SnapshotItems[i].item;
		}
		i++;
	}
	return NULL;
}

void pe_SnapshotList::ShowMode_All()
{
	int i = 0;
	while (m_SnapshotItems[i].item != NULL&&i < QSNAPSHOTLIST_LISTITEM_MAXCOUNT)
	{
		m_SnapshotItems[i].item->setVisible(true);
		i++;
	}
	m_ShowMode = QSNAPSHOTLIST_LISTITEM_SHOWMODE_ALL;
	m_ListitemsUpdateAnimationTimer.start(10);
}

void pe_SnapshotList::ShowMode_ImageItems()
{
	int i = 0;
	while (m_SnapshotItems[i].item != NULL&&i < QSNAPSHOTLIST_LISTITEM_MAXCOUNT)
	{
		if(m_SnapshotItems[i].item->IsCollector())
		m_SnapshotItems[i].item->setVisible(false);
		else
		m_SnapshotItems[i].item->setVisible(true);
		i++;
	}
	m_ShowMode = QSNAPSHOTLIST_LISTITEM_SHOWMODE_IMAGEITEMS;
	m_ListitemsUpdateAnimationTimer.start(10);
}

void pe_SnapshotList::ShowMode_Collector()
{
	int i = 0;
	while (m_SnapshotItems[i].item != NULL&&i < QSNAPSHOTLIST_LISTITEM_MAXCOUNT)
	{
		if (m_SnapshotItems[i].item->IsCollector())
			m_SnapshotItems[i].item->setVisible(true);
		else
			m_SnapshotItems[i].item->setVisible(false);
		i++;
	}
	m_ShowMode = QSNAPSHOTLIST_LISTITEM_SHOWMODE_COLLECTOR;
	m_ListitemsUpdateAnimationTimer.start(10);
}

void pe_SnapshotList::SLOT_UpdateAnimationTime()
{
	//Remove Actions
	for (int i = 0; i<QSNAPSHOTLIST_LISTITEM_MAXCOUNT; i++)
	{
		if (m_RemoveTable[i]!=-1)
		{
			QSnapshotItem *ItemReference= m_SnapshotItems[m_RemoveTable[i]].item;
			if (ItemReference->pos().x()>=-ItemReference->width())
			{
				ItemReference->move(ItemReference->x() - QSNAPSHOTLIST_LISTITEM_HORIZONTAL_STEP, ItemReference->y());
				this->update();
				return;
			}
			else
			{
				RemoveSnapShotItemFromBufferTable(m_RemoveTable[i]);
				while (m_RemoveTable[i+1]!=-1)
				{
					m_RemoveTable[i] = m_RemoveTable[i + 1];
					i++;
				}
				m_RemoveTable[i] = -1;
				this->update();
				return;
			}
		}
		else
		{
			break;
		}
	}
	//	Items Move actions
	/*resize widget*/
	int Height=0;
	for (int i=0;i<QSNAPSHOTLIST_LISTITEM_MAXCOUNT;i++)
	{
		if (!m_SnapshotItems[i].item)
		{
			break;
		}
		if (!m_SnapshotItems[i].item->isVisible())
		{
			continue;
		}
		Height += m_SnapshotItems[i].item->height();
	}
	if (this->height()<Height)
	{
		widget->setFixedHeight(Height);
	}
	else
	{
		if (this->widget->height()!=height())
		{
			widget->setFixedHeight(height()-8);
		}
	}
	//Items vertical movement
	int OffsetY = 0;
	bool HasMovement=false;
	for (int i = 0; i < QSNAPSHOTLIST_LISTITEM_MAXCOUNT; i++)
	{
		if (!m_SnapshotItems[i].item)
		{
			break;
		}
		if (!m_SnapshotItems[i].item->isVisible())
		{
			continue;
		}
			if (m_SnapshotItems[i].item->y() - OffsetY==0)
			{
			OffsetY += m_SnapshotItems[i].item->height();
			continue;
			}
		
			if (abs(m_SnapshotItems[i].item->y() - OffsetY) >= QSNAPSHOTLIST_LISTITEM_VERTICAL_STEP)
			{
				if (m_SnapshotItems[i].item->y() - OffsetY > 0)
				{
					m_SnapshotItems[i].item->move(m_SnapshotItems[i].item->x(), m_SnapshotItems[i].item->y() - QSNAPSHOTLIST_LISTITEM_VERTICAL_STEP);
				}
				else
				{
					m_SnapshotItems[i].item->move(m_SnapshotItems[i].item->x(), m_SnapshotItems[i].item->y() + QSNAPSHOTLIST_LISTITEM_VERTICAL_STEP);
				}
				HasMovement = true;
			}
			else
			{
				if (m_SnapshotItems[i].item->y() != OffsetY)
				{
					m_SnapshotItems[i].item->move(m_SnapshotItems[i].item->x(), OffsetY);
					HasMovement = true;
				}
			}
			OffsetY += m_SnapshotItems[i].item->height();
	}
	if (HasMovement)
	{
		this->update();
		return;
	}
	HasMovement = false;
	//Horizontal movement
	for (int i = 0; i < QSNAPSHOTLIST_LISTITEM_MAXCOUNT; i++)
	{
		if (!m_SnapshotItems[i].item)
		{
			break;
		}
		if (!m_SnapshotItems[i].item->isVisible())
		{
			continue;
		}
		if (m_SnapshotItems[i].item->x()==0)
		{
			continue;
		}
		if (m_SnapshotItems[i].item->x() <= -QSNAPSHOTLIST_LISTITEM_HORIZONTAL_STEP)
		{
			m_SnapshotItems[i].item->move(m_SnapshotItems[i].item->x() + QSNAPSHOTLIST_LISTITEM_HORIZONTAL_STEP, m_SnapshotItems[i].item->y());
			HasMovement = true;
		}
		else
		{
			m_SnapshotItems[i].item->move(0, m_SnapshotItems[i].item->y());
		}

	}
	this->update();

	if(!HasMovement)
	m_ListitemsUpdateAnimationTimer.stop();
}

void pe_SnapshotList::SLOT_onItemOpen(int ItemID)
{
	emit SIGNAL_onItemOpen(ItemID);
}

void pe_SnapshotList::SLOT_onItemClose(int ItemID)
{
	emit SIGNAL_onItemClose(ItemID);
}
void pe_SnapshotList::SLOT_onItemDelete(int ItemID)
{
	emit SIGNAL_onItemDelete(ItemID);
}
void pe_SnapshotList::SLOT_onItemCut(int ItemID)
{
	emit SIGNAL_onItemCut(ItemID);
}
void pe_SnapshotList::SLOT_onItemScale(int ItemID)
{
	emit SIGNAL_onItemScale(ItemID);
}
void pe_SnapshotList::SLOT_onItemPick(int ItemID)
{
	emit SIGNAL_onItemPick(ItemID);
}
void pe_SnapshotList::SLOT_onItemSave(int ItemID)
{
	emit SIGNAL_onItemSave(ItemID);
}
void pe_SnapshotList::SLOT_onItemRestore(int ItemID)
{
	emit SIGNAL_onItemRestore(ItemID);
}
void pe_SnapshotList::SLOT_onItemAlphaChange(int ItemID, int AlphaValue)
{
	emit SIGNAL_onItemAlphaChange(ItemID, AlphaValue);
}

void pe_SnapshotList::SLOT_onItemMousedown(int ItemID)
{
	emit SIGNAL_onItemDisplaySwitch(ItemID);
}

void pe_SnapshotList::SLOT_onItemMouseFocus(int ID)
{
	emit SIGNAL_onItemMouseFocus(ID);
}

void pe_SnapshotList::SLOT_onItemMouseLostFocus(int ID)
{
	emit SIGNAL_onItemMouseLostFocus(ID);
}

void pe_SnapshotList::RemoveSnapShotItemFromBufferTable(unsigned int Index)
{
	if (m_SnapshotItems[Index].item!=NULL)
	{
		delete m_SnapshotItems[Index].item;
		m_SnapshotItems[Index].item = NULL;
		m_SnapshotItems[Index].ID = -1;
		while (Index<QSNAPSHOTLIST_LISTITEM_MAXCOUNT&&m_SnapshotItems[Index+1].item!=NULL)
		{
			m_SnapshotItems[Index] = m_SnapshotItems[Index + 1];
			Index++;
		}
		m_SnapshotItems[Index] = SnapshotItemInfo();
	}
}
