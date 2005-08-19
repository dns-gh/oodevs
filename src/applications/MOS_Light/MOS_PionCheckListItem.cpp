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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_PionCheckListItem.cpp $
// $Author: Nld $
// $Modtime: 20/02/03 15:34 $
// $Revision: 4 $
// $Workfile: MOS_PionCheckListItem.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"
#include "MOS_PionCheckListItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_PionCheckListItem.inl"
#endif

#include "MOS_Agent.h"
#include "MOS_App.h"
#include "MOS_World.h"
//-----------------------------------------------------------------------------
// Name: MOS_PionCheckListItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_PionCheckListItem::MOS_PionCheckListItem( MOS_Agent& agent, QListView& listView, QCheckListItem& itemAfter, QCheckListItem::Type nCheckable )
    : MOS_AgentCheckListItem_ABC( eItemTypePion, agent, listView, itemAfter, nCheckable )
{
    setRenameEnabled( 0, false );
}

//-----------------------------------------------------------------------------
// Name: MOS_PionCheckListItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_PionCheckListItem::MOS_PionCheckListItem( MOS_Agent& agent, QCheckListItem& item, QCheckListItem& itemAfter, QCheckListItem::Type nCheckable )
    : MOS_AgentCheckListItem_ABC( eItemTypePion, agent, item, itemAfter, nCheckable )
{
    setRenameEnabled( 0, false );
}

//-----------------------------------------------------------------------------
// Name: MOS_PionCheckListItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_PionCheckListItem::MOS_PionCheckListItem( MOS_Agent& agent, QCheckListItem& item, QCheckListItem::Type nCheckable )
    : MOS_AgentCheckListItem_ABC( eItemTypePion, agent, item, nCheckable )
{
    setRenameEnabled( 0, false );
}

//-----------------------------------------------------------------------------
// Name: MOS_PionCheckListItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_PionCheckListItem::MOS_PionCheckListItem( MOS_Agent& agent, QListView& listView, QCheckListItem::Type nCheckable )
    : MOS_AgentCheckListItem_ABC( eItemTypePion, agent, listView, nCheckable )
{
    setRenameEnabled( 0, false );
}


//-----------------------------------------------------------------------------
// Name: MOS_PionCheckListItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_PionCheckListItem::~MOS_PionCheckListItem()
{
}

