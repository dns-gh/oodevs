//*****************************************************************************
//
// $Created: AGN 03-07-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DynaObject_ListViewItem.h $
// $Author: Nld $
// $Modtime: 27/04/05 14:54 $
// $Revision: 7 $
// $Workfile: MOS_DynaObject_ListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_DynaObject_ListViewItem_h_
#define __MOS_DynaObject_ListViewItem_h_

#include "MOS_Types.h"
#include <QListView.h>

class MOS_DynaObject;

//*****************************************************************************
// Created: AGN 03-07-18
//*****************************************************************************
class MOS_DynaObject_ListViewItem
: public QListViewItem
{
    MT_COPYNOTALLOWED( MOS_DynaObject_ListViewItem )

public:
    enum
    {
        e_rtti = 12
    };
public:
    MOS_DynaObject_ListViewItem( QListViewItem* pParent, MOS_DynaObject& refObject );
    virtual ~MOS_DynaObject_ListViewItem();

    void Update();

    virtual int rtti() const;

     const MOS_DynaObject& GetObject() const;

private:
    MOS_DynaObject& refObject_;
    QListViewItem* pConstructionPercentageItem_;
    QListViewItem* pValorizationPercentageItem_;
    QListViewItem* pBypassConstructionPercentageItem_;
    QListViewItem* pPreparationItem_;

    // Attributs spécifiques sites de franchissement
    QListViewItem* pSiteFranchissementLargeurItem_;
    QListViewItem* pSiteFranchissementProfondeurItem_;
    QListViewItem* pSiteFranchissementVitesseCourantItem_;
    QListViewItem* pSiteFranchissementBergesAAmenagerItem_;
    
    QListViewItem* pNuageNBCAgentNBCItem_;
    QListViewItem* pRotaNbcAgentsItem_;
    QListViewItem* pRotaDangerItem_;
    QListViewItem* pItineraireLogistiqueEquippedItem_;
    QListViewItem* pItineraireLogistiqueFlowItem_;
    QListViewItem* pItineraireLogistiqueWidthItem_;
    QListViewItem* pItineraireLogistiqueLengthItem_;
    QListViewItem* pItineraireLogistiqueMaxWeightItem_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_DynaObject_ListViewItem.inl"
#endif


#endif // __MOS_DynaObject_ListViewItem_h_