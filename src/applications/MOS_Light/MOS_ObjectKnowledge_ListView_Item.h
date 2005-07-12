//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ObjectKnowledge_ListView_Item.h $
// $Author: Nld $
// $Modtime: 27/04/05 15:03 $
// $Revision: 11 $
// $Workfile: MOS_ObjectKnowledge_ListView_Item.h $
//
//*****************************************************************************

#ifndef __MOS_ObjectKnowledge_ListView_Item_h_
#define __MOS_ObjectKnowledge_ListView_Item_h_

#include <QListView.h>

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

class MOS_ObjectKnowledge;


//*****************************************************************************
// Created: AGN 03-07-21
//*****************************************************************************
class MOS_ObjectKnowledge_ListView_Item
: public QListViewItem
{
    MT_COPYNOTALLOWED( MOS_ObjectKnowledge_ListView_Item )

public:
    enum
    {
        e_rtti = 13
    };

public:
    MOS_ObjectKnowledge_ListView_Item( QListView* pParent, MOS_ObjectKnowledge& refObject );
    virtual ~MOS_ObjectKnowledge_ListView_Item();

    virtual int rtti() const;

    void Update();

    MOS_ObjectKnowledge& GetRepresented();

private:
    MOS_ObjectKnowledge& represented_;

    QListViewItem* pListViewItemRealObject_;
    QListViewItem* pListViewItemRelevance_;
    QListViewItem* pListViewItemPourcentageConstruction_;
    QListViewItem* pListViewItemPourcentageValorisation_;
    QListViewItem* pListViewItemPourcentageContournement_;
    QListViewItem* pListViewItemEnPreparation_;
    QListViewItem* pListViewItemIsPerceived_;
    QListViewItem* pListViewItemPerceivedBy_;
    
    QListViewItem* pListViewItemSiteFranchissementLargeur_;
    QListViewItem* pListViewItemSiteFranchissementProfondeur_;
    QListViewItem* pListViewItemSiteFranchissementVitesseCourant_;
    QListViewItem* pListViewItemSiteFranchissementBergesAAmenager_;
    QListViewItem* pListViewItemNuageNBCAgentNbcID_;
    QListViewItem* pListViewItemRotaDanger_;
    QListViewItem* pListViewItemRotaNbcAgents_;
    QListViewItem* pListViewItemItiLogEquipped_;
    QListViewItem* pListViewItemItiLogFlow_;
    QListViewItem* pListViewItemItiLogWidth_;
    QListViewItem* pListViewItemItiLogLength_;
    QListViewItem* pListViewItemItiLogMaxWeight_;

private:
    static std::string ConvertObjectTypeToString( ASN1T_EnumObjectType );
};


#ifdef MOS_USE_INLINE
#	include "MOS_ObjectKnowledge_ListView_Item.inl"
#endif


#endif // __MOS_ObjectKnowledge_ListView_Item_h_