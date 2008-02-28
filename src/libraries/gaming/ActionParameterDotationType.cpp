// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterDotationType.h"
#include "clients_kernel/DotationType.h"
#include "ActionParameterVisitor_ABC.h"
#include "xeumeuleu/xml.h"
#pragma warning (push)
#pragma warning (disable : 4127 4511 4512 )
#include <boost/lexical_cast.hpp>
#pragma warning (pop)

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterDotationType constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterDotationType::ActionParameterDotationType( const OrderParameter& parameter, unsigned int id, const Resolver_ABC< DotationType >& resolver )
    : ActionParameter< std::string >( parameter )
    , type_( resolver.Get( id ) )
{
    SetValue( type_.GetCategory() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDotationType constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterDotationType::ActionParameterDotationType( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< DotationType >& resolver )
    : ActionParameter< std::string >( parameter )
    , type_( resolver.Get( xml::attribute< unsigned int >( xis, "value" ) ) )
{
    SetValue( type_.GetCategory() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDotationType destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterDotationType::~ActionParameterDotationType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDotationType::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterDotationType::Serialize( xml::xostream& xos ) const
{
    ActionParameter< std::string >::Serialize( xos );
    xos << attribute( "value", type_.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDotationType::CommitTo
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
void ActionParameterDotationType::CommitTo( std::string& content ) const
{
    content += boost::lexical_cast< std::string >( type_.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDotationType::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterDotationType::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_dotationType;
    asn.value.u.dotationType = type_.GetId();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDotationType::Accept
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
void ActionParameterDotationType::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
