// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
// -----------------------------------------------------------------------------
// Name: std::string	PopulationFlow::GetName
// Created: HME 2005-10-04
// -----------------------------------------------------------------------------
inline
const std::string& PopulationFlow::GetName() const
{
	return strName_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetFlow
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
const T_PointVector& PopulationFlow::GetFlow() const
{
    return flow_;
}
    
// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetItineraire
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
const T_PointVector& PopulationFlow::GetItineraire() const
{
    return itineraire_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetTailPosition
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& PopulationFlow::GetTailPosition() const
{
    assert( flow_.size() >= 2 );
    return flow_.front();
}

// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetHeadPosition
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& PopulationFlow::GetHeadPosition() const
{
    assert( flow_.size() >= 2 );
    return flow_.back();
}


// -----------------------------------------------------------------------------
// Name: PopulationFlow::GetPos
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& PopulationFlow::GetPos() const
{
    return GetHeadPosition();
}
