// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "EntityIdentifier.h"
#include <sstream>

using namespace rpr;

// -----------------------------------------------------------------------------
// Name: EntityIdentifier constructor
// Created: AGE 2008-04-30
// -----------------------------------------------------------------------------
EntityIdentifier::EntityIdentifier()
    : federateIdentifier_( 0, 0 )
    , entityNumber_      ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityIdentifier constructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
EntityIdentifier::EntityIdentifier( unsigned short site, unsigned short application, unsigned short number )
    : federateIdentifier_( site, application )
    , entityNumber_      ( number )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityIdentifier destructor
// Created: AGE 2008-02-21
// -----------------------------------------------------------------------------
EntityIdentifier::~EntityIdentifier()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityIdentifier::str
// Created: AHC 2012-06-28
// -----------------------------------------------------------------------------
std::string EntityIdentifier::str() const
{
    std::stringstream ss;
    ss << federateIdentifier_.siteID_ << " "
       << federateIdentifier_.applicationID_ << " "
       << entityNumber_;
    return ss.str();
}
