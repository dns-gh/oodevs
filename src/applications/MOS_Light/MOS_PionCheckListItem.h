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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_PionCheckListItem.h $
// $Author: Fbd $
// $Modtime: 21/01/03 16:23 $
// $Revision: 3 $
// $Workfile: MOS_PionCheckListItem.h $
//
//*****************************************************************************

#ifndef __MOS_PionCheckListItem_h_
#define __MOS_PionCheckListItem_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_AgentCheckListItem_ABC.h"

class QListView;
class QCheckListItem;
class MOS_Agent;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_PionCheckListItem : public MOS_AgentCheckListItem_ABC
{
    MT_COPYNOTALLOWED( MOS_PionCheckListItem );

public:
    MOS_PionCheckListItem( MOS_Agent& Pion, QListView& listView                                , QCheckListItem::Type nCheckable = QCheckListItem::CheckBox );
    MOS_PionCheckListItem( MOS_Agent& Pion, QCheckListItem& item                               , QCheckListItem::Type nCheckable = QCheckListItem::CheckBox );
    MOS_PionCheckListItem( MOS_Agent& Pion, QListView& item         , QCheckListItem& itemAfter, QCheckListItem::Type nCheckable = QCheckListItem::CheckBox );
    MOS_PionCheckListItem( MOS_Agent& Pion, QCheckListItem& item    , QCheckListItem& itemAfter, QCheckListItem::Type nCheckable = QCheckListItem::CheckBox );
   
    
    virtual ~MOS_PionCheckListItem();

private:
};


#ifdef MOS_USE_INLINE
#   include "MOS_PionCheckListItem.inl"
#endif

#endif // __MOS_PionCheckListItem_h_
