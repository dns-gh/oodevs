// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Knowledge/DEC_Knowledge_AgentPerceptionDataDetection.inl $
// $Author: Nld $
// $Modtime: 10/03/05 18:56 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_AgentPerceptionDataDetection.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetTimeLastUpdate
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
inline
uint DEC_Knowledge_AgentPerceptionDataDetection::GetTimeLastUpdate() const
{
    return nTimeLastUpdate_;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetDirection
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& DEC_Knowledge_AgentPerceptionDataDetection::GetDirection() const
{
    return vDirection_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetSpeed
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_AgentPerceptionDataDetection::GetSpeed() const
{
    return rSpeed_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetPosition
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& DEC_Knowledge_AgentPerceptionDataDetection::GetPosition() const
{
    return vPosition_;
}
          
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetAltitude
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_AgentPerceptionDataDetection::GetAltitude() const
{
    return rAltitude_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetLastPosture
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
inline
const PHY_Posture& DEC_Knowledge_AgentPerceptionDataDetection::GetLastPosture() const
{
    assert( pLastPosture_ );
    return *pLastPosture_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetCurrentPosture
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
inline
const PHY_Posture& DEC_Knowledge_AgentPerceptionDataDetection::GetCurrentPosture() const
{
    assert( pCurrentPosture_ );
    return *pCurrentPosture_;
}
          
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetPostureCompletionPercentage
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_AgentPerceptionDataDetection::GetPostureCompletionPercentage() const
{
    return rPostureCompletionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetVisionVolumes
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
inline
const T_ComposanteVolumeSet& DEC_Knowledge_AgentPerceptionDataDetection::GetVisionVolumes() const
{
    return visionVolumes_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::IsSurrendered
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_AgentPerceptionDataDetection::IsSurrendered() const
{
    return bSurrendered_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::IsPrisoner
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_AgentPerceptionDataDetection::IsPrisoner() const
{
    return bPrisoner_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::IsRefugeeManaged
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_AgentPerceptionDataDetection::IsRefugeeManaged() const
{
    return bRefugeeManaged_;
}
