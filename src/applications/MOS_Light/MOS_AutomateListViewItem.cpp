//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AutomateListViewItem.cpp $
// $Author: Age $
// $Modtime: 2/05/05 12:01 $
// $Revision: 10 $
// $Workfile: MOS_AutomateListViewItem.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"
#include "MOS_AutomateListViewItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AutomateListViewItem.inl"
#endif

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_Agent.h"
#include "MOS_ArmeListViewItem.h"

//-----------------------------------------------------------------------------
// Name: MOS_AutomateListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_AutomateListViewItem::MOS_AutomateListViewItem( MOS_Agent& agent, QListView& listView, QListViewItem& itemAfter )
    : MOS_AgentListViewItem_ABC( eItemTypeAutomate, agent, listView, itemAfter )
{
    setRenameEnabled( 0, false );
    setOpen( true );
    setDropEnabled( true );
    setDragEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: MOS_AutomateListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_AutomateListViewItem::MOS_AutomateListViewItem( MOS_Agent& agent, QListViewItem& item )
    : MOS_AgentListViewItem_ABC( eItemTypeAutomate, agent, item )
{
    setRenameEnabled( 0, false );
    setOpen( true );
    setDropEnabled( true );
    setDragEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: MOS_AutomateListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_AutomateListViewItem::MOS_AutomateListViewItem( MOS_Agent& agent, QListViewItem& item, QListViewItem& itemAfter )
    : MOS_AgentListViewItem_ABC( eItemTypeAutomate, agent, item, itemAfter )
{
    setRenameEnabled( 0, false );
    setOpen( true );
    setDropEnabled( true );
    setDragEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: MOS_AutomateListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_AutomateListViewItem::MOS_AutomateListViewItem( MOS_Agent& agent, QListView& listView )
    : MOS_AgentListViewItem_ABC( eItemTypeAutomate, agent, listView )
{
    setRenameEnabled( 0, false );
    setOpen( true );
    setDropEnabled( true );
    setDragEnabled( true );
}


//-----------------------------------------------------------------------------
// Name: MOS_AutomateListViewItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_AutomateListViewItem::~MOS_AutomateListViewItem()
{
    // signal to the possible parent to delete the reference
    /*if( this->parent() != 0 )
    {
        MOS_ArmeListViewItem* pArmeListView = static_cast< MOS_ArmeListViewItem* >( this->parent() );
        assert( pArmeListView != 0 );
        pArmeListView->UnregisterAutomate( *this );
    }*/
}


//-----------------------------------------------------------------------------
// Name: MOS_AutomateListViewItem::CreatePion
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PionListViewItem& MOS_AutomateListViewItem::CreatePion( MOS_Agent& agent, MOS_PionListViewItem* pPionAfter )
{
    MOS_PionListViewItem* pPionAutomate = 0;
    if ( PionVector_.size() )
    {
        if ( pPionAfter )
            pPionAutomate = new MOS_PionListViewItem( agent, *this, *pPionAfter );
        else
            pPionAutomate = new MOS_PionListViewItem( agent, *this, **PionVector_.rbegin() );
    }
    else
        pPionAutomate = new MOS_PionListViewItem( agent, *this );

    RegisterPion( *pPionAutomate, pPionAfter );
    return ( *pPionAutomate );
}


//-----------------------------------------------------------------------------
// Name: MOS_AutomateListViewItem::DeletePion
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_AutomateListViewItem::DeletePion( MOS_Agent& agent )
{
    MOS_PionListViewItem* pPion = GetPion( agent );
    assert( pPion );
    UnregisterPion( *pPion );

    delete pPion;
}


//-----------------------------------------------------------------------------
// Name: MOS_AutomateCheckListItem::GetPion
// Created: FBD 03-01-07
//-----------------------------------------------------------------------------
MOS_PionListViewItem* MOS_AutomateListViewItem::GetPion( MOS_Agent& agent ) const
{
    for ( CIT_PionPtrVector itPion = PionVector_.begin() ; itPion != PionVector_.end() ; ++itPion )   
    {
        if ( &(*itPion)->GetAgent() == &agent )
            break;    
    }
    if ( itPion != PionVector_.end() )
        return *itPion;

    return 0;
}

//=============================================================================
// DRAG & DROP
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AutomateListViewItem::acceptDrop
// Created: NLD 2003-05-05
//-----------------------------------------------------------------------------
bool MOS_AutomateListViewItem::acceptDrop( const QMimeSource* pMime ) const
{
    if( pMime->format() == std::string( "pion" ) )
        return true;

    return false;
}

