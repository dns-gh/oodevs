// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentDataRecognition.inl $
// $Author: Nld $
// $Modtime: 21/04/05 19:00 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_AgentDataRecognition.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::HasChanged
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_AgentDataRecognition::HasChanged() const
{
    return bOperationalStateChanged_ || bAgentTypeUpdated_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::GetOperationalState
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_AgentDataRecognition::GetOperationalState() const
{
    return rOperationalState_ == std::numeric_limits< MT_Float >::max() ? 1. : rOperationalState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::GetMajorOperationalState
// Created: NLD 2005-11-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_AgentDataRecognition::GetMajorOperationalState() const
{
    return rMajorOperationalState_ == std::numeric_limits< MT_Float >::max() ? 1. : rMajorOperationalState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::GetComposantes
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
const T_KnowledgeComposanteVector& DEC_Knowledge_AgentDataRecognition::GetComposantes() const
{
    return composantes_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::GetArmy
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
const MIL_Army* DEC_Knowledge_AgentDataRecognition::GetArmy() const
{
    return pArmy_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::GetTimeLastUpdate
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
uint DEC_Knowledge_AgentDataRecognition::GetTimeLastUpdate() const
{
    return nTimeLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::IsPC
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_AgentDataRecognition::IsPC() const
{
    return bIsPC_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::GetAgentType
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
const MIL_AgentType_ABC* DEC_Knowledge_AgentDataRecognition::GetAgentType() const
{
    return pAgentType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::Extrapolate
// Created: JVT 2004-11-29
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_AgentDataRecognition::Extrapolate( const MIL_Agent_ABC& /*agentKnown*/ )
{
    // NOTHING
}
