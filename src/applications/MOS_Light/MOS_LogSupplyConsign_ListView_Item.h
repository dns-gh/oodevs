//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogSupplyConsign_ListView_Item.h $
// $Author: Nld $
// $Modtime: 10/02/05 16:05 $
// $Revision: 2 $
// $Workfile: MOS_LogSupplyConsign_ListView_Item.h $
//
//*****************************************************************************

#ifndef __MOS_LogSupplyConsign_ListView_Item_h_
#define __MOS_LogSupplyConsign_ListView_Item_h_

#include <QListView.h>

#include "MOS_Types.h"

class MOS_LogSupplyConsign;

//*****************************************************************************
// Created: AGN 03-07-21
//*****************************************************************************
class MOS_LogSupplyConsign_ListView_Item : public QListViewItem
{
    MT_COPYNOTALLOWED( MOS_LogSupplyConsign_ListView_Item )

public:
    enum
    {
        e_rtti = 15
    };

public:
    MOS_LogSupplyConsign_ListView_Item( QListView* pParent, MOS_LogSupplyConsign& consign );
    virtual ~MOS_LogSupplyConsign_ListView_Item();

    virtual int rtti() const;

    void Update();

private:
    MOS_LogSupplyConsign& consign_;

    QListViewItem* pListViewItemPion_;
    QListViewItem* pListViewItemLogPionHandling_;
    QListViewItem* pListViewItemLogPionConvoying_;
    QListViewItem* pListViewItemState_;
    QListViewItem* pListViewItemDotations_;
};

#include "MOS_LogSupplyConsign_ListView_Item.inl"

#endif // __MOS_LogSupplyConsign_ListView_Item_h_