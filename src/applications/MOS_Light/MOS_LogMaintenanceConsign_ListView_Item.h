//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogMaintenanceConsign_ListView_Item.h $
// $Author: Nld $
// $Modtime: 4/01/05 18:46 $
// $Revision: 1 $
// $Workfile: MOS_LogMaintenanceConsign_ListView_Item.h $
//
//*****************************************************************************

#ifndef __MOS_LogMaintenanceConsign_ListView_Item_h_
#define __MOS_LogMaintenanceConsign_ListView_Item_h_

#include <QListView.h>

#include "MOS_Types.h"

class MOS_LogMaintenanceConsign;

//*****************************************************************************
// Created: AGN 03-07-21
//*****************************************************************************
class MOS_LogMaintenanceConsign_ListView_Item : public QListViewItem
{
    MT_COPYNOTALLOWED( MOS_LogMaintenanceConsign_ListView_Item )

public:
    enum
    {
        e_rtti = 14
    };

public:
    MOS_LogMaintenanceConsign_ListView_Item( QListView* pParent, MOS_LogMaintenanceConsign& consign );
    virtual ~MOS_LogMaintenanceConsign_ListView_Item();

    virtual int rtti() const;

    void Update();

private:
    MOS_LogMaintenanceConsign& consign_;

    QListViewItem* pListViewItemPion_;
    QListViewItem* pListViewItemLogPionHandling_;
    QListViewItem* pListViewItemEquipmentType_;
    QListViewItem* pListViewItemBreakdownType_;
    QListViewItem* pListViewItemState_;
};

#include "MOS_LogMaintenanceConsign_ListView_Item.inl"

#endif // __MOS_LogMaintenanceConsign_ListView_Item_h_