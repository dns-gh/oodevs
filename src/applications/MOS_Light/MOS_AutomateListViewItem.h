//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AutomateListViewItem.h $
// $Author: Nld $
// $Modtime: 5/05/03 18:24 $
// $Revision: 4 $
// $Workfile: MOS_AutomateListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_AutomateListViewItem_h_
#define __MOS_AutomateListViewItem_h_

#include "MOS_Types.h"
#include "MOS_AgentListViewItem_ABC.h"
#include "MOS_PionListViewItem.h"

class QListView;
class QListViewItem;
class MOS_Agent;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_AutomateListViewItem : public MOS_AgentListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_AutomateListViewItem );

private:
    typedef std::vector< MOS_PionListViewItem* >  T_PionPtrVector;
    typedef const T_PionPtrVector              CT_PionPtrVector;
    typedef T_PionPtrVector::iterator          IT_PionPtrVector;
    typedef T_PionPtrVector::const_iterator    CIT_PionPtrVector;
    typedef T_PionPtrVector::reverse_iterator  RIT_PionPtrVector;


public:
    MOS_AutomateListViewItem( MOS_Agent& agent, QListView& listView );
    MOS_AutomateListViewItem( MOS_Agent& agent, QListView& listView, QListViewItem& itemAfter );
    MOS_AutomateListViewItem( MOS_Agent& agent, QListViewItem& item );
    MOS_AutomateListViewItem( MOS_Agent& agent, QListViewItem& item, QListViewItem& itemAfter );
   
    
    virtual ~MOS_AutomateListViewItem();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_PionListViewItem*   GetPion( MOS_Agent& agent ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Pions*/
    //-------------------------------------------------------------------------
    //@{
    void                   RegisterPion  ( MOS_PionListViewItem& Pion, MOS_PionListViewItem* pPionAfter = 0 );
    void                   UnregisterPion( const MOS_PionListViewItem& Pion );
    MOS_PionListViewItem&  CreatePion    ( MOS_Agent& agent, MOS_PionListViewItem* pPionAfter = 0 );
    void                   DeletePion    ( MOS_Agent& agent );
    //@}

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    bool acceptDrop( const QMimeSource* mime ) const; 
    //@}

private:
    T_PionPtrVector            PionVector_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_AutomateListViewItem.inl"
#endif

#endif // __MOS_AutomateListViewItem_h_
