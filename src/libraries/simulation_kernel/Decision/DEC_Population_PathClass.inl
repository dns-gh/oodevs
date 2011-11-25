// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathClass::GetCostOutsideOfChanneling
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
inline
double DEC_Population_PathClass::GetCostOutsideOfChanneling() const
{
    return rCostOutsideOfChanneling_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathClass::GetChannelingRange
// Created: SBO 2006-03-27
// -----------------------------------------------------------------------------
inline
double DEC_Population_PathClass::GetChannelingRange() const
{
    return rChannelingRange_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathClass::AvoidObjects
// Created: CMA 2011-11-24
// -----------------------------------------------------------------------------
inline
bool DEC_Population_PathClass::AvoidObjects() const
{
    return bAvoidObjects_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathClass::GetThreshold
// Created: CMA 2011-11-24
// -----------------------------------------------------------------------------
inline
double DEC_Population_PathClass::GetThreshold() const
{
    return rObstructionThreshold_;
}
