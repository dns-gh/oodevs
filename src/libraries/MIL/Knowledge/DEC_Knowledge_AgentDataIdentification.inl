// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentDataIdentification.inl $
// $Author: Nld $
// $Modtime: 21/04/05 19:01 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_AgentDataIdentification.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::HasChanged
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_AgentDataIdentification::HasChanged() const
{
    return bAgentTypeUpdated_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::GetTimeLastUpdate
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
uint DEC_Knowledge_AgentDataIdentification::GetTimeLastUpdate() const
{
    return nTimeLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::GetAgentType
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
const MIL_AgentType_ABC* DEC_Knowledge_AgentDataIdentification::GetAgentType() const
{
    return pAgentType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataIdentification::Extrapolate
// Created: JVT 2004-11-29
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_AgentDataIdentification::Extrapolate( const MIL_Agent_ABC& /*agentKnown*/ )
{
    // NOTHING
}
