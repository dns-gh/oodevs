//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogMedicalConsign_ListView_Item.h $
// $Author: Age $
// $Modtime: 6/04/05 10:32 $
// $Revision: 2 $
// $Workfile: LogMedicalConsign_ListView_Item.h $
//
//*****************************************************************************

#ifndef __LogMedicalConsign_ListView_Item_h_
#define __LogMedicalConsign_ListView_Item_h_

#include <QListView.h>

#include "Types.h"

class LogMedicalConsign;

//*****************************************************************************
// Created: AGN 03-07-21
//*****************************************************************************
class LogMedicalConsign_ListView_Item : public QListViewItem
{
    MT_COPYNOTALLOWED( LogMedicalConsign_ListView_Item )

public:
    enum
    {
        e_rtti = 15
    };

public:
             LogMedicalConsign_ListView_Item( QListView* pParent, LogMedicalConsign& consign );
    virtual ~LogMedicalConsign_ListView_Item();

    virtual int rtti() const;

    void Update();

    void Assign( LogMedicalConsign& consign );

private:
    //! @name Tools
    //@{
    QString tr( const QString& str ) const;
    //@}

    LogMedicalConsign* pConsign_;

    QListViewItem* pListViewItemPion_;
    QListViewItem* pListViewItemLogPionHandling_;
    QListViewItem* pListViewItemWound_;
    QListViewItem* pListViewItemMental_;
    QListViewItem* pListViewItemNBC_;
    QListViewItem* pListViewItemState_;
    QListViewItem* pListViewItemDiagnosed_;
};

#include "LogMedicalConsign_ListView_Item.inl"

#endif // __LogMedicalConsign_ListView_Item_h_