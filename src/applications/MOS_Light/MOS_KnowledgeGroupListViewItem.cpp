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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_KnowledgeGroupListViewItem.cpp $
// $Author: Nld $
// $Modtime: 25/10/04 16:48 $
// $Revision: 1 $
// $Workfile: MOS_KnowledgeGroupListViewItem.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_KnowledgeGroupListViewItem.h"

// -----------------------------------------------------------------------------
// Name: MOS_KnowledgeGroupListViewItem constructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
MOS_KnowledgeGroupListViewItem::MOS_KnowledgeGroupListViewItem( MOS_Gtia& gtia, QListViewItem& item, const std::string& strField )
    : MOS_ListViewItem_ABC( eItemTypeKnowledgeGroup, item, strField )
    , gtia_               ( gtia )
{
    setDropEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: MOS_KnowledgeGroupListViewItem destructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
MOS_KnowledgeGroupListViewItem::~MOS_KnowledgeGroupListViewItem()
{

}
