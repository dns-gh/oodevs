// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: Agent_Path::GetPathClass
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const Agent_PathClass& Agent_Path::GetPathClass() const
{
    return pathClass_;
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::GetUnitSpeeds
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const Speeds& Agent_Path::GetUnitSpeeds() const
{
    return unitSpeeds_;
}

// -----------------------------------------------------------------------------
// Name: Agent_Path::GetNextWaypoints
// Created: CMA 2012-02-20
// -----------------------------------------------------------------------------
inline
const T_PointVector& Agent_Path::GetNextWaypoints() const
{
    return nextWaypoints_;
}
