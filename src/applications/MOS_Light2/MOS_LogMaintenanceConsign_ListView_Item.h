//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogMaintenanceConsign_ListView_Item.h $
// $Author: Age $
// $Modtime: 6/04/05 10:30 $
// $Revision: 2 $
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

    void Assign( MOS_LogMaintenanceConsign& consign );

private:
    //! @name Tools
    //@{
    QString tr( const QString& str ) const;
    //@}

    MOS_LogMaintenanceConsign* pConsign_;

    QListViewItem* pListViewItemPion_;
    QListViewItem* pListViewItemLogPionHandling_;
    QListViewItem* pListViewItemEquipmentType_;
    QListViewItem* pListViewItemBreakdownType_;
    QListViewItem* pListViewItemState_;
};

#include "MOS_LogMaintenanceConsign_ListView_Item.inl"

#endif // __MOS_LogMaintenanceConsign_ListView_Item_h_