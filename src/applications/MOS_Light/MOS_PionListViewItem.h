//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_PionListViewItem.h $
// $Author: Nld $
// $Modtime: 5/05/03 19:11 $
// $Revision: 4 $
// $Workfile: MOS_PionListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_PionListViewItem_h_
#define __MOS_PionListViewItem_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_AgentListViewItem_ABC.h"

class QListView;
class QListViewItem;
class MOS_Agent;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_PionListViewItem : public MOS_AgentListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_PionListViewItem );

public:
    MOS_PionListViewItem( MOS_Agent& agent, QListView& listView );
    MOS_PionListViewItem( MOS_Agent& agent, QListView& item, QListViewItem& itemAfter );
    MOS_PionListViewItem( MOS_Agent& agent, QListViewItem& item );
    MOS_PionListViewItem( MOS_Agent& agent, QListViewItem& item, QListViewItem& itemAfter );
   
    
    virtual ~MOS_PionListViewItem();

private:
    T_PointPtrVector            pointVector_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_PionListViewItem.inl"
#endif

#endif // __MOS_PionListViewItem_h_
