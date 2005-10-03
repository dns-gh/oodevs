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
    return headPosition_;
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
// Name: MIL_PopulationFlow::NotifySpecialPoint
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
void MIL_PopulationFlow::NotifySpecialPoint( const DEC_PathPoint& /*point*/ )
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
}
