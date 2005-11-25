// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Knowledge/DEC_Knowledge_AgentPerceptionDataRecognition.inl $
// $Author: Nld $
// $Modtime: 10/11/04 11:46 $
// $Revision: 1 $
// $Workfile: DEC_Knowledge_AgentPerceptionDataRecognition.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::GetTimeLastUpdate
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
uint DEC_Knowledge_AgentPerceptionDataRecognition::GetTimeLastUpdate() const
{
    return nTimeLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::GetArmy
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
const MIL_Army* DEC_Knowledge_AgentPerceptionDataRecognition::GetArmy() const
{
    return pArmy_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::IsPC
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_AgentPerceptionDataRecognition::IsPC() const
{
    return bIsPC_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::GetAgentType
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
const MIL_AgentType_ABC* DEC_Knowledge_AgentPerceptionDataRecognition::GetAgentType() const
{
    return pAgentType_;
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::GetOperationalState
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_AgentPerceptionDataRecognition::GetOperationalState() const
{
    return rOperationalState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::GetComposantes
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
const T_KnowledgeComposanteVector& DEC_Knowledge_AgentPerceptionDataRecognition::GetComposantes() const
{
    return composantes_;
}
