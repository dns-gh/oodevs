//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AutomateCheckListItem.h $
// $Author: Nld $
// $Modtime: 4/03/03 14:47 $
// $Revision: 4 $
// $Workfile: MOS_AutomateCheckListItem.h $
//
//*****************************************************************************

#ifndef __MOS_AutomateCheckListItem_h_
#define __MOS_AutomateCheckListItem_h_

#include "MOS_Types.h"
#include "MOS_AgentCheckListItem_ABC.h"
#include "MOS_PionCheckListItem.h"

class QListView;
class QListViewItem;
class QCheckListItem;
class MOS_Agent;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_AutomateCheckListItem : public MOS_AgentCheckListItem_ABC
{
    MT_COPYNOTALLOWED( MOS_AutomateCheckListItem );

private:
    typedef std::vector< MOS_PionCheckListItem* >   T_PionPtrVector;
    typedef const T_PionPtrVector                   CT_PionPtrVector;
    typedef T_PionPtrVector::iterator               IT_PionPtrVector;
    typedef T_PionPtrVector::const_iterator         CIT_PionPtrVector;
    typedef T_PionPtrVector::reverse_iterator       RIT_PionPtrVector;


public:
    MOS_AutomateCheckListItem( MOS_Agent& agent, QListView& listView                            , QCheckListItem::Type nCheckable = QCheckListItem::CheckBox );
    MOS_AutomateCheckListItem( MOS_Agent& agent, QListView& listView, QListViewItem& itemAfter  , QCheckListItem::Type nCheckable = QCheckListItem::CheckBox );
    MOS_AutomateCheckListItem( MOS_Agent& agent, QListViewItem& item                            , QCheckListItem::Type nCheckable = QCheckListItem::CheckBox );
    MOS_AutomateCheckListItem( MOS_Agent& agent, QListViewItem& item, QListViewItem& itemAfter  , QCheckListItem::Type nCheckable = QCheckListItem::CheckBox );
    
    virtual ~MOS_AutomateCheckListItem();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_PionCheckListItem*  GetPion( MOS_Agent& agent ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Pions*/
    //-------------------------------------------------------------------------
    //@{
    void                   RegisterPion  ( MOS_PionCheckListItem& Pion, MOS_PionCheckListItem* pPionAfter = 0 );
    void                   UnregisterPion( const MOS_PionCheckListItem& Pion );
    MOS_PionCheckListItem& CreatePion    ( MOS_Agent& agent, MOS_PionCheckListItem* pPionAfter = 0, bool bCheckable = true );
    void                   DeletePion    ( MOS_Agent& agent );
    //@}


private:
    T_PionPtrVector            PionVector_;

};


#ifdef MOS_USE_INLINE
#   include "MOS_AutomateCheckListItem.inl"
#endif

#endif // __MOS_AutomateCheckListItem_h_
