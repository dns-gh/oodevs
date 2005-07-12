//*****************************************************************************
//
// $Created: AGN 03-04-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_ObjectKnowledgeListItem.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_MTH_ObjectKnowledgeListItem.h $
//
//*****************************************************************************

#ifndef __MOS_MTH_ObjectKnowledgeListItem_h_
#define __MOS_MTH_ObjectKnowledgeListItem_h_

#include <QListView.h>
#include "MOS_Types.h"

#include "MOS_ObjectKnowledge.h"

//*****************************************************************************
// Created: AGN 03-04-23
//*****************************************************************************
class MOS_MTH_ObjectKnowledgeListItem
: public QCheckListItem
{
    MT_COPYNOTALLOWED( MOS_MTH_ObjectKnowledgeListItem )

public:
    MOS_MTH_ObjectKnowledgeListItem( MOS_ObjectKnowledge& represented, QListView* pParent, Type checkType, const std::string& strLabel );
    MOS_MTH_ObjectKnowledgeListItem( MOS_ObjectKnowledge& represented, QCheckListItem* pParent, Type checkType, const std::string& strLabel );
    virtual ~MOS_MTH_ObjectKnowledgeListItem();

    MIL_AgentID GetMOSID() const;
    MOS_ObjectKnowledge& GetRepresented();

private:
    MOS_ObjectKnowledge& represented_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_MTH_ObjectKnowledgeListItem.inl"
#endif


#endif // __MOS_MTH_ObjectKnowledgeListItem_h_