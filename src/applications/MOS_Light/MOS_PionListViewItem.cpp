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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_PionListViewItem.cpp $
// $Author: Nld $
// $Modtime: 5/05/03 18:23 $
// $Revision: 5 $
// $Workfile: MOS_PionListViewItem.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_PionListViewItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_PionListViewItem.inl"
#endif

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_Agent.h"
//-----------------------------------------------------------------------------
// Name: MOS_PionListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_PionListViewItem::MOS_PionListViewItem( MOS_Agent& agent, QListView& listView, QListViewItem& itemAfter )
    : MOS_AgentListViewItem_ABC( eItemTypePion, agent, listView, itemAfter )
{
    setRenameEnabled( 0, false );
    setDragEnabled( true );
    setDropEnabled( false );
}

//-----------------------------------------------------------------------------
// Name: MOS_PionListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_PionListViewItem::MOS_PionListViewItem( MOS_Agent& agent, QListViewItem& item )
    : MOS_AgentListViewItem_ABC( eItemTypePion, agent, item )
{
    setRenameEnabled( 0, false );
    setDragEnabled( true );
    setDropEnabled( false );
}

//-----------------------------------------------------------------------------
// Name: MOS_PionListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_PionListViewItem::MOS_PionListViewItem( MOS_Agent& agent, QListView& listView )
    : MOS_AgentListViewItem_ABC( eItemTypePion, agent, listView )
{
    setRenameEnabled( 0, false );
    setDragEnabled( true );
    setDropEnabled( false );
}

//-----------------------------------------------------------------------------
// Name: MOS_PionListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_PionListViewItem::MOS_PionListViewItem( MOS_Agent& agent, QListViewItem& item, QListViewItem& itemAfter )
    : MOS_AgentListViewItem_ABC( eItemTypePion, agent, item, itemAfter )
{
    setRenameEnabled( 0, false );
    setDragEnabled( true );
    setDropEnabled( false );
}

//-----------------------------------------------------------------------------
// Name: MOS_PionListViewItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_PionListViewItem::~MOS_PionListViewItem()
{
}

