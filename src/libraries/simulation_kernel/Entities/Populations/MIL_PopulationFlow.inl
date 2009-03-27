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
// Name: MIL_PopulationFlow::GetSpeedWithReinforcement
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationFlow::GetSpeedWithReinforcement( const TerrainData& /*environment*/ ) const
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
    PHY_MovingEntity_ABC     ::Clean();
    MIL_PopulationElement_ABC::Clean();

    bPathUpdated_       = false;
    bFlowShapeUpdated_  = false;
    bDirectionUpdated_  = false;
    bSpeedUpdated_      = false;
    bHeadMoveFinished_  = false;
    pSplittingObject_   = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::HasChanged
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationFlow::HasChanged() const
{
    return    HasHumansChanged  () 
           || HasAttitudeChanged()
           || bFlowShapeUpdated_
           || bDirectionUpdated_
           || bSpeedUpdated_;
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
// Name: MIL_PopulationFlow::GetPosition
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MIL_PopulationFlow::GetPosition() const
{
    return GetHeadPosition();
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
// Name: MIL_PopulationFlow::GetSpeed
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
inline
MT_Float MIL_PopulationFlow::GetSpeed() const
{
    return rSpeed_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::IsValid
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationFlow::IsValid() const
{
    return GetNbrHumans() > 0. || pSourceConcentration_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::CanBePerceived
// Created: NLD 2005-12-07
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationFlow::CanBePerceived() const
{
    return IsValid();
}
