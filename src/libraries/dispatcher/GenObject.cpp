// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "GenObject.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: GenObject constructor
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
GenObject::GenObject( const ASN1T_PlannedWork& asn )
    : type_              ( asn.type )
    , location_          ( asn.position )
    , typeObstacle_      ( asn.type_obstacle )
    , density_           ( asn.densite )
    , tc2_               ( asn.tc2 )
    , delaiActiviteMines_( asn.activity_time )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GenObject constructor
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
GenObject::GenObject()
    : type_              ( "" )
    , location_          ()
    , typeObstacle_      ( EnumDemolitionTargetType::preliminary )
    , density_           ( 0. )
    , tc2_               ( 0 )
    , delaiActiviteMines_( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: GenObject destructor
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
GenObject::~GenObject()
{
    // NOTHING
}

 // -----------------------------------------------------------------------------
// Name: GenObject::Send
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
void GenObject::Send( ASN1T_PlannedWork& asn ) const
{
    asn.type           = type_.c_str();
    asn.type_obstacle  = typeObstacle_;
    asn.densite        = density_;
    asn.tc2            = tc2_;
    asn.activity_time = delaiActiviteMines_;
    location_.Send( asn.position );
}
