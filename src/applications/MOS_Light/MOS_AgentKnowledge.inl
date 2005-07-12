//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentKnowledge.inl $
// $Author: Age $
// $Modtime: 23/11/04 10:55 $
// $Revision: 6 $
// $Workfile: MOS_AgentKnowledge.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge::GetID
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
uint MOS_AgentKnowledge::GetID() const
{
    return nID_;    
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge::IsUpdated
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
bool MOS_AgentKnowledge::IsValid( E_AttributeUpdated nAttr ) const
{
    return nAttrUpdated_ & nAttr;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge::GetPosition
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_AgentKnowledge::GetPosition() const
{
    return vPosition_;    
}
