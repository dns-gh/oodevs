//*****************************************************************************
//
// $Created: AGN 03-04-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_ObjectKnowledgeListItem.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 2 $
// $Workfile: MOS_MTH_ObjectKnowledgeListItem.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeListItem::GetMOSID
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
inline
MIL_AgentID MOS_MTH_ObjectKnowledgeListItem::GetMOSID() const
{
    return represented_.GetID();
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeListItem::GetRepresented
// Created: AGN 03-04-24
//-----------------------------------------------------------------------------
inline
MOS_ObjectKnowledge& MOS_MTH_ObjectKnowledgeListItem::GetRepresented()
{
    return represented_;
}

