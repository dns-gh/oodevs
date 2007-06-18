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
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterDotationType constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterDotationType::ActionParameterDotationType( const OrderParameter& parameter, unsigned int id, const Resolver_ABC< DotationType >& resolver )
    : ActionParameter< QString >( parameter )
    , type_( resolver.Get( id ) )
{
    if( !type_.IsDType() )
        throw std::runtime_error( tools::translate( "ActionParameter", "Dotation '%1' is not 'D-Type'." ).arg( type_.GetCategory() ).ascii() );
    SetValue( type_.GetCategory() );
}

namespace
{
    unsigned int ReadId( xml::xistream& xis )
    {
        unsigned int id;
        xis >> attribute( "value", id );
        return id;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDotationType constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterDotationType::ActionParameterDotationType( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< DotationType >& resolver )
    : ActionParameter< QString >( parameter )
    , type_( resolver.Get( ReadId( xis ) ) )
{
    if( !type_.IsDType() )
        throw std::runtime_error( tools::translate( "ActionParameter", "Dotation '%1' is not 'D-Type'." ).arg( type_.GetCategory() ).ascii() );
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
    ActionParameter< QString >::Serialize( xos );
    xos << attribute( "value", type_.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterDotationType::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterDotationType::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = 0;
    asn.value.t = T_MissionParameter_value_dotationType;
    asn.value.u.dotationType = type_.GetId();
}
