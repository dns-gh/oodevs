// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "ObstacleType.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ObstacleType constructor
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
ObstacleType::ObstacleType( const OrderParameter& parameter, unsigned int value )
    : Parameter< QString >( parameter )
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
// Name: ObstacleType constructor
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
ObstacleType::ObstacleType( xml::xistream& xis )
    : Parameter< QString >( OrderParameter( xml::attribute< std::string >( xis, "name" ).c_str(), xml::attribute< std::string >( xis, "type" ), false ) )
    , value_( ReadValue( xis ) )
{
    SetValue( tools::ToString( value_ ) );
}

// -----------------------------------------------------------------------------
// Name: ObstacleType destructor
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
ObstacleType::~ObstacleType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleType::Serialize
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void ObstacleType::Serialize( xml::xostream& xos ) const
{
    Parameter< QString >::Serialize( xos );
    xos << attribute( "value", unsigned int( value_ ) );
}

// -----------------------------------------------------------------------------
// Name: ObstacleType::CommitTo
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void ObstacleType::CommitTo( ASN1T_EnumObstacleType& asn ) const
{
    asn = ASN1T_EnumObstacleType( value_ );
}
