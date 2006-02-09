//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogSupplyConsign_ListView_Item.h $
// $Author: Age $
// $Modtime: 6/04/05 10:40 $
// $Revision: 2 $
// $Workfile: LogSupplyConsign_ListView_Item.h $
//
//*****************************************************************************

#ifndef __LogSupplyConsign_ListView_Item_h_
#define __LogSupplyConsign_ListView_Item_h_

#include <QListView.h>

#include "Types.h"

class LogSupplyConsign;

//*****************************************************************************
// Created: AGN 03-07-21
//*****************************************************************************
class LogSupplyConsign_ListView_Item : public QListViewItem
{
    MT_COPYNOTALLOWED( LogSupplyConsign_ListView_Item )

public:
    enum
    {
        e_rtti = 15
    };

public:
    LogSupplyConsign_ListView_Item( QListView* pParent, LogSupplyConsign& consign );
    virtual ~LogSupplyConsign_ListView_Item();

    virtual int rtti() const;

    void Update();

    void Assign( LogSupplyConsign& consign );

private:
    //! @name Tools
    //@{
    QString tr( const QString& str ) const;
    //@}

    LogSupplyConsign* pConsign_;

    QListViewItem* pListViewItemPion_;
    
    QListViewItem* pListViewItemLogAutomateHandling_;
    QListViewItem* pListViewItemLogAutomateProvidingConvoyResources_;
    QListViewItem* pListViewItemLogPionConvoying_;
    QListViewItem* pListViewItemState_;
    QListViewItem* pListViewItemDotations_;
};

#include "LogSupplyConsign_ListView_Item.inl"

#endif // __LogSupplyConsign_ListView_Item_h_