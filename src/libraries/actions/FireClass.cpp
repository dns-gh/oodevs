// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "FireClass.h"
#include "ParameterVisitor_ABC.h"
#include "clients_kernel/FireClass.h"

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: FireClass constructor
// Created: LGY 2013-11-26
// -----------------------------------------------------------------------------
FireClass::FireClass( const kernel::OrderParameter& parameter )
    : Parameter< std::string >( parameter )
    , type_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireClass constructor
// Created: LGY 2013-11-26
// -----------------------------------------------------------------------------
FireClass::FireClass( const kernel::OrderParameter& parameter, const std::string name,
                                          const tools::StringResolver< kernel::FireClass >& resolver )
    : Parameter< std::string >( parameter )
    , type_( &resolver.Get( name ) )
{
    SetValue( type_->GetName() );
}

// -----------------------------------------------------------------------------
// Name: FireClass destructor
// Created: LGY 2013-11-26
// -----------------------------------------------------------------------------
FireClass::~FireClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireClass::Serialize
// Created: LGY 2013-11-26
// -----------------------------------------------------------------------------
void FireClass::Serialize( xml::xostream& xos ) const
{
    Parameter< std::string >::Serialize( xos );
    if( IsSet() && type_ )
        xos << xml::attribute( "value", type_->GetName() );
}

// -----------------------------------------------------------------------------
// Name: FireClass::Accept
// Created: LGY 2013-11-26
// -----------------------------------------------------------------------------
void FireClass::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: FireClass::SerializeType
// Created: LGY 2013-11-26
// -----------------------------------------------------------------------------
std::string FireClass::SerializeType() const
{
    return "fireclass";
}
