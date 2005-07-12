//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentKnowledge.inl $
// $Author: Ape $
// $Modtime: 28/05/04 15:43 $
// $Revision: 3 $
// $Workfile: MOS_AgentKnowledge.inl $
//
//*****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge::GetID
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_AgentKnowledge::GetID() const
{
    return nID_;    
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge::IsUpdated
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_AgentKnowledge::IsValid( E_AttributeUpdated nAttr ) const
{
    return nAttrUpdated_ & nAttr;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge::GetPosition
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_INLINE
const MT_Vector2D& MOS_AgentKnowledge::GetPosition() const
{
    return vPosition_;    
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge::GetRealAgent
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Agent& MOS_AgentKnowledge::GetRealAgent() const
{
    return realAgent_;    
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledge::GetOwner
/** @return 
*/
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Gtia& MOS_AgentKnowledge::GetOwner() const
{
    return owner_;
}
