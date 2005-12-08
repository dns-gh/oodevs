// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
// -----------------------------------------------------------------------------
// Name: std::string	MOS_PopulationFlow::GetName
// Created: HME 2005-10-04
// -----------------------------------------------------------------------------
inline
const std::string& MOS_PopulationFlow::GetName() const
{
	return strName_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlow::GetFlow
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
const T_PointVector& MOS_PopulationFlow::GetFlow() const
{
    return flow_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlow::GetItineraire
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
const T_PointVector& MOS_PopulationFlow::GetItineraire() const
{
    return itineraire_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlow::GetTailPosition
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_PopulationFlow::GetTailPosition() const
{
    assert( flow_.size() >= 2 );
    return flow_.front();
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlow::GetHeadPosition
// Created: NLD 2005-10-06
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_PopulationFlow::GetHeadPosition() const
{
    assert( flow_.size() >= 2 );
    return flow_.back();
}


// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlow::GetPos
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_PopulationFlow::GetPos() const
{
    return GetHeadPosition();
}
