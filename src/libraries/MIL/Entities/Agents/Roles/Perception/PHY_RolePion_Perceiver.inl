// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.inl $
// $Author: Nld $
// $Modtime: 2/05/05 18:21 $
// $Revision: 12 $
// $Workfile: PHY_RolePion_Perceiver.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Perceiver::NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level, bool bPerceptionRecorded )
{
    assert( pKsPerception_ );
    pKsPerception_->NotifyPerception( agent, level, bPerceptionRecorded );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Perceiver::NotifyPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level )
{
    assert( pKsPerception_ );
    pKsPerception_->NotifyPerception( agent, level, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Perceiver::NotifyPerception( MIL_RealObject_ABC& object, const PHY_PerceptionLevel& level )
{
    assert( pKsPerception_ ); 
    pKsPerception_->NotifyPerception( object, level, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Perceiver::NotifyPerception( MIL_PopulationConcentration& concentration, const PHY_PerceptionLevel& level )
{
    assert( pKsPerception_ );
    pKsPerception_->NotifyPerception( concentration, level, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Perceiver::NotifyPerception( MIL_PopulationFlow& flow, const PHY_PerceptionLevel& level, const T_PointVector& shape )
{
    assert( pKsPerception_ );
    pKsPerception_->NotifyPerception( flow, level, shape, bRecordModeEnabled_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::NotifytExternalPerception
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Perceiver::NotifyExternalPerception( MIL_Agent_ABC& agent , const PHY_PerceptionLevel& level )
{
    assert( pKsPerception_ );
    pKsPerception_->NotifyExternalPerception( agent, level );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::IsPeriphericalVisionEnabled
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Perceiver::IsPeriphericalVisionEnabled() const
{
    return bPeriphericalVisionEnabled_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetPion
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
inline
const MIL_AgentPion& PHY_RolePion_Perceiver::GetPion() const
{
    assert( pPion_ );
    return *pPion_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::SetVisionModeNormal
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Perceiver::SetVisionModeNormal()
{
    nSensorMode_ = eNormal;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::SetVisionModeDirection
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Perceiver::SetVisionModeDirection( const MT_Vector2D& vDirection )
{
    nSensorMode_ = eDirection;
    vSensorInfo_ = vDirection;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::SetVisionModePoint
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Perceiver::SetVisionModePoint( const MT_Vector2D& vPoint )
{
    nSensorMode_ = ePoint;
    vSensorInfo_ = vPoint;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::HasChanged
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Perceiver::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetMainPerceptionDirection
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Perceiver::GetMainPerceptionDirection( MT_Vector2D& vDirection ) const
{
    ComputeMainPerceptionDirection( vDirection );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::EnableRecordMode
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Perceiver::EnableRecordMode()
{
    bRecordModeEnabled_ = true;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::DisableRecordMode
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Perceiver::DisableRecordMode()
{
    bRecordModeEnabled_ = false;
    
    assert( pKsPerception_ );
    pKsPerception_->MakePerceptionsAvailable();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::HasRadarStateChanged
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Perceiver::HasRadarStateChanged() const
{
    return bRadarStateHasChanged_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Perceiver::Clean()
{
    bHasChanged_           = false;
    bRadarStateHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::HasDelayedPerceptions
// Created: JVT 2004-12-03
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Perceiver::HasDelayedPerceptions() const
{
    assert( pKsPerception_ );
    return pKsPerception_->HasDelayedPerceptions();
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetSurfacesAgent
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
inline
const PHY_RolePion_Perceiver::T_SurfaceAgentMap& PHY_RolePion_Perceiver::GetSurfacesAgent() const
{
    return surfacesAgent_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetSurfacesObject
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
inline
const PHY_RolePion_Perceiver::T_SurfaceObjectMap& PHY_RolePion_Perceiver::GetSurfacesObject() const
{
    return surfacesObject_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Perceiver::GetRadars
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
inline
const PHY_RolePion_Perceiver::T_RadarSet& PHY_RolePion_Perceiver::GetRadars( const PHY_RadarClass& radarClass )
{
    return radars_[ &radarClass ];
}
