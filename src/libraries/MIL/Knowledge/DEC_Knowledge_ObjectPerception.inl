// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectPerception.inl $
// $Author: Jvt $
// $Modtime: 17/03/05 12:34 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_ObjectPerception.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::GetObjectPerceived
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& DEC_Knowledge_ObjectPerception::GetObjectPerceived() const
{
    assert( pObjectPerceived_ );
    return *pObjectPerceived_; 
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::GetObjectPerceiving
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
inline
const MIL_AgentPion& DEC_Knowledge_ObjectPerception::GetAgentPerceiving() const
{
    assert( pAgentPerceiving_ );
    return *pAgentPerceiving_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::IsPerceived
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_ObjectPerception::IsPerceived() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::GetCurrentPerceptionLevel
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
const PHY_PerceptionLevel& DEC_Knowledge_ObjectPerception::GetCurrentPerceptionLevel() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_ObjectPerception::Clean()
{
    return !IsPerceived();
}
