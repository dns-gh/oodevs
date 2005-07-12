//*****************************************************************************
//
// $Created: AGN 03-04-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_AgentKnowledgeListItem.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_MTH_AgentKnowledgeListItem.h $
//
//*****************************************************************************

#ifndef __MOS_MTH_AgentKnowledgeListItem_h_
#define __MOS_MTH_AgentKnowledgeListItem_h_

#include <QListView.h>
#include "MOS_Types.h"

#include "MOS_AgentKnowledge.h"

//*****************************************************************************
// Created: AGN 03-04-23
//*****************************************************************************
class MOS_MTH_AgentKnowledgeListItem
: public QCheckListItem
{
    MT_COPYNOTALLOWED( MOS_MTH_AgentKnowledgeListItem )

public:
    MOS_MTH_AgentKnowledgeListItem( MOS_AgentKnowledge& represented, QListView* pParent, Type checkType, const std::string& strLabel );
    MOS_MTH_AgentKnowledgeListItem( MOS_AgentKnowledge& represented, QCheckListItem* pParent, Type checkType, const std::string& strLabel );
    virtual ~MOS_MTH_AgentKnowledgeListItem();

    MIL_AgentID GetMOSID() const;
    MOS_AgentKnowledge& GetRepresented();

private:
    MOS_AgentKnowledge& represented_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_MTH_AgentKnowledgeListItem.inl"
#endif


#endif // __MOS_MTH_AgentKnowledgeListItem_h_