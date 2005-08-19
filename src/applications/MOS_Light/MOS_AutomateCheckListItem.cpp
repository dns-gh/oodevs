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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AutomateCheckListItem.cpp $
// $Author: Nld $
// $Modtime: 20/02/03 15:34 $
// $Revision: 4 $
// $Workfile: MOS_AutomateCheckListItem.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"
#include "MOS_AutomateCheckListItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AutomateCheckListItem.inl"
#endif

#include "MOS_Agent.h"
#include "MOS_App.h"
#include "MOS_World.h"
//-----------------------------------------------------------------------------
// Name: MOS_AutomateCheckListItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_AutomateCheckListItem::MOS_AutomateCheckListItem( MOS_Agent& agent, QListView& listView, QListViewItem& itemAfter, QCheckListItem::Type nCheckable )
    : MOS_AgentCheckListItem_ABC( eItemTypeAutomate, agent, listView, itemAfter, nCheckable )
{
    setRenameEnabled( 0, false );
    setOpen( true );
}

//-----------------------------------------------------------------------------
// Name: MOS_AutomateCheckListItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_AutomateCheckListItem::MOS_AutomateCheckListItem( MOS_Agent& agent, QListViewItem& item, QCheckListItem::Type nCheckable )
    : MOS_AgentCheckListItem_ABC( eItemTypeAutomate, agent, item, nCheckable )
{
    setRenameEnabled( 0, false );
    setOpen( true );
}

//-----------------------------------------------------------------------------
// Name: MOS_AutomateCheckListItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_AutomateCheckListItem::MOS_AutomateCheckListItem( MOS_Agent& agent, QListView& listView, QCheckListItem::Type nCheckable )
    : MOS_AgentCheckListItem_ABC( eItemTypeAutomate, agent, listView, nCheckable )
{
    setRenameEnabled( 0, false );
    setOpen( true );
}

//-----------------------------------------------------------------------------
// Name: MOS_AutomateCheckListItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_AutomateCheckListItem::MOS_AutomateCheckListItem( MOS_Agent& agent, QListViewItem& item, QListViewItem& itemAfter, QCheckListItem::Type nCheckable )
    : MOS_AgentCheckListItem_ABC( eItemTypeAutomate, agent, item, itemAfter, nCheckable )
{
    setRenameEnabled( 0, false );
    setOpen( true );
}

//-----------------------------------------------------------------------------
// Name: MOS_AutomateCheckListItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_AutomateCheckListItem::~MOS_AutomateCheckListItem()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_AutomateCheckListItem::CreatePion
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PionCheckListItem& MOS_AutomateCheckListItem::CreatePion( MOS_Agent& agent, MOS_PionCheckListItem* pPionAfter, bool bCanCheck )
{
    QCheckListItem::Type nCheckable = QCheckListItem::Controller;
    if ( bCanCheck )
        nCheckable = QCheckListItem::CheckBox;


    MOS_PionCheckListItem* pPionAutomate = 0;
    if ( PionVector_.size() )
    {
        if ( pPionAfter )
            pPionAutomate = new MOS_PionCheckListItem( agent, *this, *pPionAfter, nCheckable );
        else
            pPionAutomate = new MOS_PionCheckListItem( agent, *this, **PionVector_.rbegin(), nCheckable );
    }
    else
        pPionAutomate = new MOS_PionCheckListItem( agent, *this, nCheckable );

    RegisterPion( *pPionAutomate, pPionAfter );
    return ( *pPionAutomate );
}


//-----------------------------------------------------------------------------
// Name: MOS_AutomateCheckListItem::DeletePion
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_AutomateCheckListItem::DeletePion( MOS_Agent& agent )
{
    MOS_PionCheckListItem* pPion = GetPion( agent );
    assert( pPion );
    UnregisterPion( *pPion );
    delete pPion;
}
//-----------------------------------------------------------------------------
// Name: MOS_AutomateCheckListItem::GetPion
// Created: FBD 03-01-07
//-----------------------------------------------------------------------------
MOS_PionCheckListItem* MOS_AutomateCheckListItem::GetPion( MOS_Agent& agent ) const
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

