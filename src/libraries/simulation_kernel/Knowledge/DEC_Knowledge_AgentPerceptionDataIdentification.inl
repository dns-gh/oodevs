// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Knowledge/DEC_Knowledge_AgentPerceptionDataIdentification.inl $
// $Author: Nld $
// $Modtime: 10/11/04 11:47 $
// $Revision: 1 $
// $Workfile: DEC_Knowledge_AgentPerceptionDataIdentification.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataIdentification::GetTimeLastUpdate
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
uint DEC_Knowledge_AgentPerceptionDataIdentification::GetTimeLastUpdate() const
{
    return nTimeLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataIdentification::GetAgentType
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
const MIL_AgentType_ABC* DEC_Knowledge_AgentPerceptionDataIdentification::GetAgentType() const
{
    return pAgentType_;
}
