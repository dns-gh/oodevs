// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Knowledge/DEC_Knowledge_AgentDataDetection.inl $
// $Author: Nld $
// $Modtime: 10/03/05 19:01 $
// $Revision: 5 $
// $Workfile: DEC_Knowledge_AgentDataDetection.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::IsSurrendered
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_AgentDataDetection::IsSurrendered() const
{
    return bSurrendered_;
}
          
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::IsPrisoner
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_AgentDataDetection::IsPrisoner() const
{
    return bPrisoner_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::IsRefugeeManaged
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_AgentDataDetection::IsRefugeeManaged() const
{
    return bRefugeeManaged_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetSpeed
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_AgentDataDetection::GetSpeed() const
{
    return rSpeed_ == std::numeric_limits< MT_Float >::max() ? 0. : rSpeed_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetAltitude
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_AgentDataDetection::GetAltitude() const
{
    return rAltitude_ == std::numeric_limits< MT_Float >::max() ? 0. : rAltitude_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetDirection
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& DEC_Knowledge_AgentDataDetection::GetDirection() const
{
    return vDirection_;
}
   
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetPosition
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& DEC_Knowledge_AgentDataDetection::GetPosition() const
{
    return vPosition_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetLastPosture
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
inline
const PHY_Posture& DEC_Knowledge_AgentDataDetection::GetLastPosture() const
{
    assert( pLastPosture_ );
    return *pLastPosture_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetCurrentPosture
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
inline
const PHY_Posture& DEC_Knowledge_AgentDataDetection::GetCurrentPosture() const
{
    assert( pCurrentPosture_ );
    return *pCurrentPosture_;
}
          
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetPostureCompletionPercentage
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_AgentDataDetection::GetPostureCompletionPercentage() const
{
    return rPostureCompletionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetVisionVolumes
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
inline
const T_ComposanteVolumeSet& DEC_Knowledge_AgentDataDetection::GetVisionVolumes() const
{
    return visionVolumes_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetTimeLastUpdate
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
uint DEC_Knowledge_AgentDataDetection::GetTimeLastUpdate() const
{
    return nTimeLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::HasChanged
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_AgentDataDetection::HasChanged() const
{
    return bDirectionUpdated_ || bSpeedUpdated_ || bPositionUpdated_ || bPrisonerUpdated_ || bSurrenderedUpdated_ || bRefugeeManagedUpdated_;
}
