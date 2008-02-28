// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterObstacleType.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleType constructor
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
ActionParameterObstacleType::ActionParameterObstacleType( const OrderParameter& parameter, unsigned int value )
    : ActionParameter< QString >( parameter )
    , value_( E_ObstacleType( value ) )
{
    SetValue( tools::ToString( value_ )  );
}

namespace
{
    E_ObstacleType ReadValue( xml::xistream& xis )
    {
        unsigned int value;
        xis >> attribute( "value", value );
        return E_ObstacleType( value );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleType constructor
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
ActionParameterObstacleType::ActionParameterObstacleType( xml::xistream& xis )
    : ActionParameter< QString >( OrderParameter( xml::attribute< std::string >( xis, "name" ).c_str(), xml::attribute< std::string >( xis, "type" ), false ) )
    , value_( ReadValue( xis ) )
{
    SetValue( tools::ToString( value_ ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleType destructor
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
ActionParameterObstacleType::~ActionParameterObstacleType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleType::Serialize
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void ActionParameterObstacleType::Serialize( xml::xostream& xos ) const
{
    ActionParameter< QString >::Serialize( xos );
    xos << attribute( "value", unsigned int( value_ ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObstacleType::CommitTo
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void ActionParameterObstacleType::CommitTo( ASN1T_EnumObstacleType& asn ) const
{
    asn = ASN1T_EnumObstacleType( value_ );
}
