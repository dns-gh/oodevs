// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

namespace dispatcher
{

// -----------------------------------------------------------------------------
// Name: Simulation::GetSimulationFromLink
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
inline
Simulation& Simulation::GetSimulationFromLink( const DIN::DIN_Link& link )
{
    DIN::DIN_UserData_ABC* pTmp = link.GetUserData();
    assert( pTmp );
    return *static_cast< Simulation* >( pTmp );    
}

}