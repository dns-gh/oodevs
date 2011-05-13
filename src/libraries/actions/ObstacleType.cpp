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
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ObstacleType constructor
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
ObstacleType::ObstacleType( const kernel::OrderParameter& parameter, unsigned int value )
    : Parameter< QString >( parameter )
    , value_( sword::ObstacleType_DemolitionTargetType( value ) )
{
    SetValue( tools::ToString( E_DemolitionTargetType( value_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: ObstacleType constructor
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
ObstacleType::ObstacleType( xml::xistream& xis )
    : Parameter< QString >( kernel::OrderParameter( xis.attribute< std::string >( "name" ).c_str(), xis.attribute< std::string >( "type" ), false ) )
    , value_( sword::ObstacleType_DemolitionTargetType( xis.attribute< unsigned int >( "value" ) ) )
{
    SetValue( tools::ToString( E_DemolitionTargetType( value_ ) ) );
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
    xos << xml::attribute( "value", unsigned int( value_ ) );
}

// -----------------------------------------------------------------------------
// Name: ObstacleType::CommitTo
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void ObstacleType::CommitTo( T_Setter setter ) const
{
    setter( sword::ObstacleType_DemolitionTargetType( value_ ) );
}

// -----------------------------------------------------------------------------
// Name: ObstacleType::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string ObstacleType::SerializeType() const
{
    return "obstacletype";
}
