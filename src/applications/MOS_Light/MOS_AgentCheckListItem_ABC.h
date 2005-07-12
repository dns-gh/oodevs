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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentCheckListItem_ABC.h $
// $Author: Fbd $
// $Modtime: 21/01/03 16:21 $
// $Revision: 3 $
// $Workfile: MOS_AgentCheckListItem_ABC.h $
//
//*****************************************************************************

#ifndef __MOS_AgentCheckListItem_ABC_h_
#define __MOS_AgentCheckListItem_ABC_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_CheckListItem_ABC.h"

class MOS_Agent;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_AgentCheckListItem_ABC : public MOS_CheckListItem_ABC
{
    MT_COPYNOTALLOWED( MOS_AgentCheckListItem_ABC );

public:
    MOS_AgentCheckListItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListView& listView,                                   QCheckListItem::Type nCheckable = QCheckListItem::CheckBox );
    MOS_AgentCheckListItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListViewItem& itemParent,                             QCheckListItem::Type nCheckable = QCheckListItem::CheckBox );
    MOS_AgentCheckListItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListViewItem& itemParent, QListViewItem& itemAfter,   QCheckListItem::Type nCheckable = QCheckListItem::CheckBox );
    MOS_AgentCheckListItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListView& listView       , QListViewItem& itemParent, QCheckListItem::Type nCheckable = QCheckListItem::CheckBox );
    
    virtual ~MOS_AgentCheckListItem_ABC();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_Agent&                      GetAgent    () const;
    const std::string               GetName     () const;
    void                            SetName     ( const std::string& sName );
    //@}

private:
    MOS_Agent& agent_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_AgentCheckListItem_ABC.inl"
#endif

#endif // __MOS_AgentCheckListItem_ABC_h_
