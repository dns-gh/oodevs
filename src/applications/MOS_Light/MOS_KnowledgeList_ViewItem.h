//*****************************************************************************
//
// $Created: AGN 03-05-22 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_KnowledgeList_ViewItem.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_KnowledgeList_ViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_KnowledgeList_ViewItem_h_
#define __MOS_KnowledgeList_ViewItem_h_

#include <QListView.h>
#include "MOS_Types.h"

#include "MOS_AgentKnowledge.h"

//*****************************************************************************
// Created: AGN 03-04-23
//*****************************************************************************
class MOS_KnowledgeList_ViewItem
: public QListViewItem
{
    MT_COPYNOTALLOWED( MOS_KnowledgeList_ViewItem )

public:
    MOS_KnowledgeList_ViewItem( MOS_AgentKnowledge& represented, QListViewItem* pParent, const std::string& strLabel );
    MOS_KnowledgeList_ViewItem( MOS_AgentKnowledge& represented, QListView* pParent, const std::string& strLabel );
    ~MOS_KnowledgeList_ViewItem();

    MIL_AgentID GetMOSID() const;
    MOS_AgentKnowledge& GetRepresented();

private:
    MOS_AgentKnowledge& represented_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_KnowledgeList_ViewItem.inl"
#endif


#endif // __MOS_KnowledgeList_ViewItem_h_