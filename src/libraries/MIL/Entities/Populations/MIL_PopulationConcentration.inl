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
// Name: MIL_PopulationConcentration::GetSplittingObject
// Created: NLD 2006-04-27
// -----------------------------------------------------------------------------
inline
const MIL_RealObject_ABC* MIL_PopulationConcentration::GetSplittingObject() const
{
    return pSplittingObject_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetPosition
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MIL_PopulationConcentration::GetPosition() const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::Clean
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
inline
void MIL_PopulationConcentration::Clean()
{
    MIL_PopulationElement_ABC::Clean();    
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::HasChanged
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationConcentration::HasChanged() const
{
    return HasAttitudeChanged() || HasHumansChanged();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::UnregisterPushingFlow
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
inline
void MIL_PopulationConcentration::UnregisterPushingFlow( MIL_PopulationFlow& flow )
{
    int nOut = pushingFlows_.erase( &flow );
    assert( nOut );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetLocation
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
inline
const TER_Localisation& MIL_PopulationConcentration::GetLocation() const
{
    return location_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetSecuringPoint
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
inline
MT_Vector2D MIL_PopulationConcentration::GetSecuringPoint( const MIL_Agent_ABC& /*securingAgent*/ ) const
{
    return position_; // $$$$ SBO 2005-12-16: should maybe return a random point into concentration
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::IsValid
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationConcentration::IsValid() const
{
    return GetNbrHumans() > 0. || !pushingFlows_.empty();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::CanBePerceived
// Created: NLD 2005-12-07
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationConcentration::CanBePerceived() const
{
    return IsValid();
}


