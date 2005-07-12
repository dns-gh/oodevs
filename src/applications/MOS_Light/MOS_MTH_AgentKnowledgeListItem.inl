//*****************************************************************************
//
// $Created: AGN 03-04-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_AgentKnowledgeListItem.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 3 $
// $Workfile: MOS_MTH_AgentKnowledgeListItem.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeListItem::GetMOSID
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
inline
MIL_AgentID MOS_MTH_AgentKnowledgeListItem::GetMOSID() const
{
    return represented_.GetID();
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeListItem::GetRepresented
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
inline
MOS_AgentKnowledge& MOS_MTH_AgentKnowledgeListItem::GetRepresented()
{
    return represented_;
}

