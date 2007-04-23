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
GenObject::GenObject( const ASN1T_MissionGenObject& asn )
    : type_              ( asn.type )
    , location_          ( asn.position )
    , preliminaire_      ( asn.preliminaire )
    , density_           ( asn.densite )
    , tc2_               ( asn.tc2 )
    , delaiActiviteMines_( asn.delai_activite_mines )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GenObject constructor
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
GenObject::GenObject()
    : type_              ( EnumObjectType::abattis )
    , location_          ()
    , preliminaire_      ( EnumMissionGenSousTypeObstacle::preliminaire )
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
void GenObject::Send( ASN1T_MissionGenObject& asn ) const
{
    asn.type                 = type_;
    asn.preliminaire         = preliminaire_;
    asn.densite              = density_;
    asn.tc2                  = tc2_;
    asn.delai_activite_mines = delaiActiviteMines_;
    location_.Send( asn.position );
}

// -----------------------------------------------------------------------------
// Name: GenObject::AsnDelete
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
// static
void GenObject::AsnDelete( ASN1T_MissionGenObject& asn )
{
    Localisation::AsnDelete( asn.position );
}
