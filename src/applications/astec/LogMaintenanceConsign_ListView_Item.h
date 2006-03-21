//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogMaintenanceConsign_ListView_Item.h $
// $Author: Age $
// $Modtime: 6/04/05 10:30 $
// $Revision: 2 $
// $Workfile: LogMaintenanceConsign_ListView_Item.h $
//
//*****************************************************************************

#ifndef __LogMaintenanceConsign_ListView_Item_h_
#define __LogMaintenanceConsign_ListView_Item_h_

#include <QListView.h>

#include "Types.h"

class LogMaintenanceConsign;

//*****************************************************************************
// Created: AGN 03-07-21
//*****************************************************************************
class LogMaintenanceConsign_ListView_Item : public QListViewItem
{
    MT_COPYNOTALLOWED( LogMaintenanceConsign_ListView_Item )

public:
    enum
    {
        e_rtti = 14
    };

public:
             LogMaintenanceConsign_ListView_Item( QListView* pParent, LogMaintenanceConsign& consign );
    virtual ~LogMaintenanceConsign_ListView_Item();

    virtual int rtti() const;

    void Update();

    void Assign( LogMaintenanceConsign& consign );

private:
    //! @name Tools
    //@{
    QString tr( const QString& str ) const;
    //@}

    LogMaintenanceConsign* pConsign_;

    QListViewItem* pListViewItemPion_;
    QListViewItem* pListViewItemLogPionHandling_;
    QListViewItem* pListViewItemEquipmentType_;
    QListViewItem* pListViewItemBreakdownType_;
    QListViewItem* pListViewItemState_;
    QListViewItem* pListViewItemDiagnosed_;
};

#include "LogMaintenanceConsign_ListView_Item.inl"

#endif // __LogMaintenanceConsign_ListView_Item_h_