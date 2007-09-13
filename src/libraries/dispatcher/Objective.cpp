// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Objective.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Objective constructor
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
Objective::Objective( const ASN1T_MissionObjective& asn )
    : location_( asn.localisation )
    , schedule_( asn.horaire      )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Objective constructor
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
Objective::Objective()
    : location_()
    , schedule_( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: Objective destructor
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
Objective::~Objective()
{
    // NOTHING
}

 // -----------------------------------------------------------------------------
// Name: Objective::Send
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
void Objective::Send( ASN1T_MissionObjective& asn ) const
{
    asn.horaire = schedule_;
    location_.Send( asn.localisation );
}

// -----------------------------------------------------------------------------
// Name: Objective::AsnDelete
// Created: NLD 2007-04-23
// -----------------------------------------------------------------------------
// static
void Objective::AsnDelete( ASN1T_MissionObjective& asn )
{
    Localisation::AsnDelete( asn.localisation );
}
