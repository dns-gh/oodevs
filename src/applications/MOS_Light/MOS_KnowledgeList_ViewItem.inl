//*****************************************************************************
//
// $Created: AGN 03-05-22 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_KnowledgeList_ViewItem.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 3 $
// $Workfile: MOS_KnowledgeList_ViewItem.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: MOS_KnowledgeList_ViewItem::GetMOSID
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
inline
MIL_AgentID MOS_KnowledgeList_ViewItem::GetMOSID() const
{
    return represented_.GetID();
}


//-----------------------------------------------------------------------------
// Name: MOS_KnowledgeList_ViewItem::GetRepresented
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
inline
MOS_AgentKnowledge& MOS_KnowledgeList_ViewItem::GetRepresented()
{
    return represented_;
}

