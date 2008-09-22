// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "DotationType.h"
#include "clients_kernel/DotationType.h"
#include "ParameterVisitor_ABC.h"
#include "xeumeuleu/xml.h"
#pragma warning (push)
#pragma warning (disable : 4127 4511 4512 )
#include <boost/lexical_cast.hpp>
#pragma warning (pop)

using namespace xml;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: DotationType constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
DotationType::DotationType( const kernel::OrderParameter& parameter, unsigned int id, const kernel::Resolver_ABC< kernel::DotationType >& resolver )
    : Parameter< std::string >( parameter )
    , type_( resolver.Get( id ) )
{
    SetValue( type_.GetCategory() );
}

// -----------------------------------------------------------------------------
// Name: DotationType constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
DotationType::DotationType( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::DotationType >& resolver )
    : Parameter< std::string >( parameter )
    , type_( resolver.Get( xml::attribute< unsigned int >( xis, "value" ) ) )
{
    SetValue( type_.GetCategory() );
}

// -----------------------------------------------------------------------------
// Name: DotationType destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
DotationType::~DotationType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationType::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void DotationType::Serialize( xml::xostream& xos ) const
{
    Parameter< std::string >::Serialize( xos );
    xos << attribute( "value", type_.GetId() );
}

// -----------------------------------------------------------------------------
// Name: DotationType::CommitTo
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
void DotationType::CommitTo( std::string& content ) const
{
    content += boost::lexical_cast< std::string >( type_.GetId() );
}

// -----------------------------------------------------------------------------
// Name: DotationType::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void DotationType::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_dotationType;
    asn.value.u.dotationType = type_.GetId();
}

// -----------------------------------------------------------------------------
// Name: DotationType::Accept
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
void DotationType::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
