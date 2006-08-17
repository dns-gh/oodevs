// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentPerception.inl $
// $Author: Jvt $
// $Modtime: 17/03/05 16:02 $
// $Revision: 5 $
// $Workfile: DEC_Knowledge_AgentPerception.inl $
//
// *****************************************************************************

#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetAgentPerceived
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
inline
MIL_Agent_ABC& DEC_Knowledge_AgentPerception::GetAgentPerceived() const
{
    assert( pAgentPerceived_ );
    return *pAgentPerceived_; 
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetAgentPerceiving
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
inline
const MIL_AgentPion& DEC_Knowledge_AgentPerception::GetAgentPerceiving() const
{
    assert( pAgentPerceiving_ );
    return *pAgentPerceiving_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetMaxPerceptionLevel
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
const PHY_PerceptionLevel& DEC_Knowledge_AgentPerception::GetMaxPerceptionLevel() const
{
    assert( pMaxPerceptionLevel_ );
    return *pMaxPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetCurrentPerceptionLevel
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
inline
const PHY_PerceptionLevel& DEC_Knowledge_AgentPerception::GetCurrentPerceptionLevel() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetPreviousPerceptionLevel
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
inline
const PHY_PerceptionLevel& DEC_Knowledge_AgentPerception::GetPreviousPerceptionLevel() const
{
    assert( pPreviousPerceptionLevel_ );
    return *pPreviousPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::IsPerceived
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_AgentPerception::IsPerceived() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ || bRecordModeEnabled_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetDetectionData
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
const DEC_Knowledge_AgentPerceptionDataDetection& DEC_Knowledge_AgentPerception::GetDetectionData() const
{
    return dataDetection_;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetRecognitionData
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
const DEC_Knowledge_AgentPerceptionDataRecognition& DEC_Knowledge_AgentPerception::GetRecognitionData() const
{
    return dataRecognition_;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetIdentificationData
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
const DEC_Knowledge_AgentPerceptionDataIdentification& DEC_Knowledge_AgentPerception::GetIdentificationData() const
{
    return dataIdentification_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::IsAvailable
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_AgentPerception::IsAvailable() const
{
    return !bRecordModeEnabled_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::MakeAvailable
// Created: NLD 2004-11-16
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_AgentPerception::MakeAvailable( uint nDelay )
{
    if( bRecordModeEnabled_ )
    {
        nRecordModeDisablingDelay_ = nDelay;
        bRecordModeEnabled_ = ( nRecordModeDisablingDelay_ != 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetCreationTimeStep
// Created: NLD 2004-11-16
// -----------------------------------------------------------------------------
inline
uint DEC_Knowledge_AgentPerception::GetCreationTimeStep() const
{
    return nCreationTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_AgentPerception::Clean()
{
    return !IsPerceived();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::NotifyAttacker
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_AgentPerception::NotifyAttacker()
{
    bAttacker_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::IsAttacker
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_AgentPerception::IsAttacker() const
{
    return bAttacker_;
}
