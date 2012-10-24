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

namespace
{
    bool Match(unsigned short lhs, unsigned short rhs)
    {
        return lhs == 0xFFFF ||
               rhs == 0xFFFF ||
               lhs == rhs;
    }
}

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
// Created: AHC 2012-10-23
// -----------------------------------------------------------------------------
EntityIdentifier::EntityIdentifier( const rpr::EntityIdentifier& rhs )
    : federateIdentifier_( rhs.federateIdentifier_ )
    , entityNumber_( rhs.entityNumber_ )
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

// -----------------------------------------------------------------------------
// Name: EntityIdentifier::operator==
// Created: AHC 2012-06-30
// -----------------------------------------------------------------------------
bool EntityIdentifier::operator== ( const EntityIdentifier& rhs ) const
{
    return entityNumber_ == rhs.entityNumber_ &&
            federateIdentifier_.siteID_ == rhs.federateIdentifier_.siteID_ &&
            federateIdentifier_.applicationID_ == rhs.federateIdentifier_.applicationID_;
}

// -----------------------------------------------------------------------------
// Name: EntityIdentifier::operator<
// Created: AHC 2012-06-30
// -----------------------------------------------------------------------------
bool EntityIdentifier::operator< ( const EntityIdentifier& rhs ) const
{
    return entityNumber_ < rhs.entityNumber_
            || ( entityNumber_ == rhs.entityNumber_ && federateIdentifier_.siteID_ < rhs.federateIdentifier_.siteID_ )
            || ( entityNumber_ == rhs.entityNumber_ && federateIdentifier_.siteID_ == rhs.federateIdentifier_.siteID_  && federateIdentifier_.applicationID_ < rhs.federateIdentifier_.applicationID_ );
}

// -----------------------------------------------------------------------------
// Name: EntityIdentifier::Match
// Created: AHC 2012-06-30
// -----------------------------------------------------------------------------
bool EntityIdentifier::Match( const rpr::EntityIdentifier& lhs, const rpr::EntityIdentifier& rhs)
{
    return ::Match( lhs.federateIdentifier_.siteID_, rhs.federateIdentifier_.siteID_ ) &&
           ::Match( lhs.federateIdentifier_.applicationID_, rhs.federateIdentifier_.applicationID_ ) &&
           ::Match( lhs.entityNumber_, rhs.entityNumber_ );
}

// -----------------------------------------------------------------------------
// Name: EntityIdentifier::GetNumber
// Created: AHC 2012-10-23
// -----------------------------------------------------------------------------
unsigned short EntityIdentifier::GetNumber() const
{
    return entityNumber_;
}
