// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "ParameterFactory.h"
#include "actions/Direction.h"
#include "actions/Limit.h"

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: ParameterFactory constructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
ParameterFactory::ParameterFactory( const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory destructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
ParameterFactory::~ParameterFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateParameter
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateParameter( const kernel::OrderParameter& parameter ) const
{
    if( parameter.GetType() == "heading" )
        return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Direction( parameter, 0 ) );
    if( parameter.GetType() == "phaseline" )
    {
        xml::xistringstream xis( "<limit name='Limite 7'><point>31UEQ0687827739</point><point>31UEQ0505917232</point></limit>" );
        return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Limit( parameter, converter_, xis ) );
    }
    if( parameter.GetType() == "limit" )
    {
        xml::xistringstream xis( "<limit name='Limite 7'><point>31UEQ0687827739</point><point>31UEQ0505917232</point></limit>" );
        return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Limit( parameter, converter_, xis ) );
    }
    return std::auto_ptr< actions::Parameter_ABC >();
}
