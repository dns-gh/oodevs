// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Mission.h"
#include "Mapping.h"

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
Mission::Mission()
    : mapping_( 0 ) // $$$$ MCO : hmm !
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
Mission::Mission( const std::string& id, const std::vector< std::vector< Position > >& tacticals, Mapping& mapping )
    : id_       ( id )
    , tacticals_( tacticals )
    , mapping_  ( &mapping )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission destructor
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
Mission::~Mission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: operator<<
// Created: MCO 2010-07-13
// -----------------------------------------------------------------------------
xml::xostream& operator<<( xml::xostream& xos, const Mission& mission )
{
    mission.mapping_->Serialize( xos, mission.id_, mission.tacticals_ );
    return xos;
}
