// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "IsPartOfStruct.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: IsPartOfStruct constructor
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
IsPartOfStruct::IsPartOfStruct()
    : constituentPartNature_( 0 )
    , constituentPartPosition_( 0 )
    , stationNumber_( 0 )
    , constituentPartStationName( Other )
    , x ( 0.f )
    , y ( 0.f )
    , z ( 0.f )
    , range ( 0.f )
    , bearing ( 0.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IsPartOfStruct constructor
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
IsPartOfStruct::IsPartOfStruct( const rpr::EntityIdentifier& disId, const Omt13String& rtiId )
    : disId_( disId )
    , rtiId_( rtiId )
    , constituentPartNature_( 0 )
    , constituentPartPosition_( 0 )
    , stationNumber_( 0 )
    , constituentPartStationName( Other )
    , x ( 0.f )
    , y ( 0.f )
    , z ( 0.f )
    , range ( 0.f )
    , bearing ( 0.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IsPartOfStruct destructor
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
IsPartOfStruct::~IsPartOfStruct()
{
    // NOTHING
}
