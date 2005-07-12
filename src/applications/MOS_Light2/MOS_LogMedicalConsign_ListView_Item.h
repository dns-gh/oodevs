//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogMedicalConsign_ListView_Item.h $
// $Author: Age $
// $Modtime: 6/04/05 10:32 $
// $Revision: 2 $
// $Workfile: MOS_LogMedicalConsign_ListView_Item.h $
//
//*****************************************************************************

#ifndef __MOS_LogMedicalConsign_ListView_Item_h_
#define __MOS_LogMedicalConsign_ListView_Item_h_

#include <QListView.h>

#include "MOS_Types.h"

class MOS_LogMedicalConsign;

//*****************************************************************************
// Created: AGN 03-07-21
//*****************************************************************************
class MOS_LogMedicalConsign_ListView_Item : public QListViewItem
{
    MT_COPYNOTALLOWED( MOS_LogMedicalConsign_ListView_Item )

public:
    enum
    {
        e_rtti = 15
    };

public:
             MOS_LogMedicalConsign_ListView_Item( QListView* pParent, MOS_LogMedicalConsign& consign );
    virtual ~MOS_LogMedicalConsign_ListView_Item();

    virtual int rtti() const;

    void Update();

    void Assign( MOS_LogMedicalConsign& consign );

private:
    //! @name Tools
    //@{
    QString tr( const QString& str ) const;
    //@}

    MOS_LogMedicalConsign* pConsign_;

    QListViewItem* pListViewItemPion_;
    QListViewItem* pListViewItemLogPionHandling_;
    QListViewItem* pListViewItemWound_;
    QListViewItem* pListViewItemMental_;
    QListViewItem* pListViewItemNBC_;
    QListViewItem* pListViewItemState_;
};

#include "MOS_LogMedicalConsign_ListView_Item.inl"

#endif // __MOS_LogMedicalConsign_ListView_Item_h_