// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-28 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetID
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
uint MIL_PopulationFlow::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetPosition
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MIL_PopulationFlow::GetPosition() const
{
    assert( !flowShape_.empty() );
    return flowShape_.back();
}
    
// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetDirection
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MIL_PopulationFlow::GetDirection() const
{
    return direction_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetSpeedWithReinforcement
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationFlow::GetSpeedWithReinforcement( const TerrainData& /*environment*/ ) const
{
    return GetMaxSpeed();
}
    
// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetSpeedWithReinforcement
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationFlow::GetSpeedWithReinforcement( const TerrainData& /*environment*/, const MIL_Object_ABC& /*object*/ ) const
{
    return GetMaxSpeed();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::NotifyMovingOnSpecialPoint
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
void MIL_PopulationFlow::NotifyMovingOnSpecialPoint( const DEC_PathPoint& /*point*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::NotifyMovingInsideObject
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
void MIL_PopulationFlow::NotifyMovingInsideObject( MIL_Object_ABC& /*object*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::NotifyMovingOutsideObject
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
void MIL_PopulationFlow::NotifyMovingOutsideObject( MIL_Object_ABC& /*object*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::NotifyEnvironmentChanged
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
void MIL_PopulationFlow::NotifyEnvironmentChanged()
{

}
// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::NotifyCurrentPathChanged
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
void MIL_PopulationFlow::NotifyCurrentPathChanged()
{
    bPathUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::CanMove
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationFlow::CanMove() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::CanObjectInteractWith
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationFlow::CanObjectInteractWith( const MIL_Object_ABC& /*object*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::HasResources
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationFlow::HasResources()
{
    return true;
}
    
// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::Clean
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
void MIL_PopulationFlow::Clean()
{
    PHY_MovingEntity_ABC::Clean();
    bPathUpdated_      = false;
    bFlowShapeUpdated_ = false;
    bDirectionUpdated_ = false;
    bSpeedUpdated_     = false;
    bHumansUpdated_    = false;
    bAttitudeUpdated_  = false;
    bHeadMoveFinished_ = false;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::HasChanged
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationFlow::HasChanged() const
{
    return    bPathUpdated_
           || bFlowShapeUpdated_
           || bDirectionUpdated_
           || bSpeedUpdated_
           || bHumansUpdated_
           || bAttitudeUpdated_;  
}
    
// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::UnregisterSourceConcentration
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
inline
void MIL_PopulationFlow::UnregisterSourceConcentration( MIL_PopulationConcentration& concentration )
{
    assert( pSourceConcentration_ == &concentration );
    pSourceConcentration_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetHeadPosition
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MIL_PopulationFlow::GetHeadPosition() const
{
    assert( flowShape_.size() >= 2 );
    return flowShape_.back();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetTailPosition
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MIL_PopulationFlow::GetTailPosition() const
{
    assert( flowShape_.size() >= 2 );
    return flowShape_.front();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SetDirection
// Created: NLD 2005-10-20
// -----------------------------------------------------------------------------
inline
void MIL_PopulationFlow::SetDirection( const MT_Vector2D& direction )
{
    if( direction_ == direction )
        return;
    direction_         = direction;
    bDirectionUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SetSpeed
// Created: NLD 2005-10-20
// -----------------------------------------------------------------------------
inline
void MIL_PopulationFlow::SetSpeed( const MT_Float rSpeed )
{
    if( rSpeed_ == rSpeed )
        return;
    rSpeed_        = rSpeed;
    bSpeedUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SetHeadPosition
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
inline
void MIL_PopulationFlow::SetHeadPosition( const MT_Vector2D& position )
{
    assert( flowShape_.size() >= 2 );
    if( flowShape_.back() == position )
        return;
    bFlowShapeUpdated_ = true;
    flowShape_.back() = position;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SetTailPosition
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
inline
void MIL_PopulationFlow::SetTailPosition( const MT_Vector2D& position )
{
    assert( flowShape_.size() >= 2 );
    if( flowShape_.front() == position )
        return;
    bFlowShapeUpdated_ = true;
    flowShape_.front() = position;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetLocation
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
inline
const TER_Localisation& MIL_PopulationFlow::GetLocation() const
{
    return location_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetPopulation
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
inline
MIL_Population& MIL_PopulationFlow::GetPopulation() const
{
    assert( pPopulation_ );
    return *pPopulation_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetAttitude
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
inline
const MIL_PopulationAttitude& MIL_PopulationFlow::GetAttitude() const
{
    assert( pAttitude_ );
    return *pAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetNbrAliveHumans
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
inline
uint MIL_PopulationFlow::GetNbrAliveHumans() const
{
    return ( uint )rNbrAliveHumans_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetNbrDeadHumans
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
inline
uint MIL_PopulationFlow::GetNbrDeadHumans() const
{
    return ( uint )rNbrDeadHumans_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetSpeed
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationFlow::GetSpeed() const
{
    return rSpeed_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetDensity
// Created: NLD 2005-10-27
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationFlow::GetDensity() const
{
    return rDensity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::IsValid
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationFlow::IsValid() const
{
    return !pDestConcentration_ || pSourceConcentration_ || GetHeadPosition() != GetTailPosition();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::UpdateDensity
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
inline
void MIL_PopulationFlow::UpdateDensity()
{
    const MT_Float rArea = location_.GetArea();
    if( rArea == 0. )
        rDensity_ = 0; // $$$ +infini sauf si aucun humain
    else
        rDensity_ = rNbrAliveHumans_ / rArea;
}
