// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_Agent.inl $
// $Author: Jvt $
// $Modtime: 24/03/05 11:57 $
// $Revision: 8 $
// $Workfile: DEC_Knowledge_Agent.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
uint DEC_Knowledge_Agent::GetID() const
{
    return nID_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetAgentPerceived
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
inline
MIL_Agent_ABC& DEC_Knowledge_Agent::GetAgentKnown() const
{
    assert( pAgentKnown_ );
    return *pAgentKnown_; 
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetPosition
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& DEC_Knowledge_Agent::GetPosition() const
{
    return dataDetection_.GetPosition();    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetDirection
// Created: JVT 2005-02-17
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& DEC_Knowledge_Agent::GetDirection() const
{
    return dataDetection_.GetDirection();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetAltitude
// Created: NLD 2004-06-07
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_Agent::GetAltitude() const
{
    return dataDetection_.GetAltitude();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetSpeed
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_Agent::GetSpeed() const
{
    return dataDetection_.GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetRelevance
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_Agent::GetRelevance() const
{
    return rRelevance_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetPostureCompletionPercentage
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_Agent::GetPostureCompletionPercentage() const
{
    return dataDetection_.GetPostureCompletionPercentage();   
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetOldPosture
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
inline
const PHY_Posture& DEC_Knowledge_Agent::GetLastPosture() const
{
    return dataDetection_.GetLastPosture();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetCurrentPosture
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
inline
const PHY_Posture& DEC_Knowledge_Agent::GetCurrentPosture() const
{
    return dataDetection_.GetCurrentPosture();
}
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetMaxPerceptionLevel
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
inline
const PHY_PerceptionLevel& DEC_Knowledge_Agent::GetMaxPerceptionLevel() const
{
    assert( pMaxPerceptionLevel_ );
    return *pMaxPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetCurrentPerceptionLevel
// Created: NLD 2004-04-28
// -----------------------------------------------------------------------------
inline
const PHY_PerceptionLevel& DEC_Knowledge_Agent::GetCurrentPerceptionLevel() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetOperationalState
// Created: NLD 2004-04-14
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_Agent::GetOperationalState() const
{
    return dataRecognition_.GetOperationalState();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetMajorOperationalState
// Created: NLD 2005-11-30
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_Agent::GetMajorOperationalState() const
{
    return dataRecognition_.GetMajorOperationalState();  
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsDead
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_Agent::IsDead() const
{
    return dataDetection_.IsDead();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsSurrendered
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_Agent::IsSurrendered() const
{
    return dataDetection_.IsSurrendered();
}
          
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsPrisoner
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_Agent::IsPrisoner() const
{
    return dataDetection_.IsPrisoner();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsRefugeeManaged
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_Agent::IsRefugeeManaged() const
{
    return dataDetection_.IsRefugeeManaged();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetArmy
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
inline
const MIL_Army* DEC_Knowledge_Agent::GetArmy() const
{
    return dataRecognition_.GetArmy();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetNatureWeapon
// Created: NLD 2004-03-31
// Modified: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const PHY_NatureWeapon& DEC_Knowledge_Agent::GetNatureWeapon() const
{
    return dataRecognition_.GetNatureWeapon();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetNatureAtlas
// Created: NLD 2004-03-31
// Modified: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const PHY_NatureAtlas& DEC_Knowledge_Agent::GetNatureAtlas() const
{
    return dataRecognition_.GetNatureAtlas();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::Clean
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_Agent::Clean() const
{
    return GetRelevance() <= 0.;
}
