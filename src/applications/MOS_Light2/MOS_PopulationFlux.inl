// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
// -----------------------------------------------------------------------------
// Name: std::string	MOS_PopulationFlux::GetName
// Created: HME 2005-10-04
// -----------------------------------------------------------------------------
inline
const std::string& MOS_PopulationFlux::GetName() const
{
	return strName_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlux::GetFlow
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
const T_PointVector& MOS_PopulationFlux::GetFlow() const
{
    return flow_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlux::GetItineraire
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
const T_PointVector& MOS_PopulationFlux::GetItineraire() const
{
    return itineraire_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlux::GetTailPosition
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_PopulationFlux::GetTailPosition() const
{
    assert( flow_.size() >= 2 );
    return flow_.front();
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlux::GetHeadPosition
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_PopulationFlux::GetHeadPosition() const
{
    assert( flow_.size() >= 2 );
    return flow_.back();
}


// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlux::GetPos
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_PopulationFlux::GetPos() const
{
    return GetTailPosition();
}