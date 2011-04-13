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
#include "actions/Lima.h"
#include "actions/Numeric.h"
#include "actions/Path.h"
#include "actions/Point.h"
#include "actions/ParameterList.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Point.h"
#include <boost/assign/list_of.hpp>

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: ParameterFactory constructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
ParameterFactory::ParameterFactory( const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , points_   ( boost::assign::list_of( "29TNF2373665534" )( "29TNF4227865556" )( "29TNF4218748160" )( "29TNF2999748106" ) )
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
        return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Direction( parameter, 360 ) );
    if( parameter.GetType() == "integer" || parameter.GetType() == "numeric"  )
        return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Numeric( parameter, 1.f ) );
    if( parameter.GetType() == "phaseline" )
        return CreatePhaselineParameter( parameter );
    if( parameter.GetType() == "limit" )
        return CreateLimitParameter( parameter );
    if( parameter.GetType() == "point" )
        return CreatePointParameter( parameter );
    if( parameter.GetType() == "path" )
        return CreatePathParameter( parameter );
    return std::auto_ptr< actions::Parameter_ABC >();
}

// -----------------------------------------------------------------------------
// Name: std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreatePhaselineParameter
// Created: PHC 2011-04-13
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreatePhaselineParameter( const kernel::OrderParameter& parameter ) const
{
    std::auto_ptr< actions::Parameter_ABC > list( new actions::parameters::ParameterList( kernel::OrderParameter( parameter.GetName(), "phaseline", true ) ) );
    xml::xistringstream xis( "<parameter name='Lima' type='lima' value='LDM'>"
                             "  <parameter name='Position' type='location'>"
                             "    <location type='line'>"
                             "      <point coordinates='31UDQ8022661674'/>"
                             "      <point coordinates='31UEQ0576863427'/>"
                             "      <point coordinates='31UEQ2054052685'/>"
                             "    </location>"
                             "  </parameter>"
                             "  <parameter name='Horaire' type='datetime' value='20121203T143802'/>"
                             "</parameter>" );
    xis >> xml::start( "parameter" );
    list->AddParameter( *new actions::parameters::Lima( converter_, xis ) );
    return list;
}

// -----------------------------------------------------------------------------
// Name: std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateLimitParameter
// Created: PHC 2011-04-13
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateLimitParameter( const kernel::OrderParameter& parameter ) const
{
    static bool b = true;
    b = !b;
    xml::xistringstream xis( std::string( "" ) + "<parameter name='Limit' type='Limit'>"
                             "  <location type='line'>"
                           + ( b ? "    <point coordinates='29TNF4287565859'/>"
                                   "    <point coordinates='29TNF4292847578'/>"
                                 : "    <point coordinates='29TNF2304365837'/>"
                                   "    <point coordinates='29TNF2912947516'/>" )
                           + "  </location>"
                             "</parameter>" );
    xis >> xml::start( "parameter" );
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Limit( parameter, converter_, xis ) );
}

// -----------------------------------------------------------------------------
// Name: std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreatePointParameter
// Created: PHC 2011-04-13
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreatePointParameter( const kernel::OrderParameter& parameter ) const
{
    static CIT_Points point = points_.begin();
    if( point == points_.end() )
        point = points_.begin();
    xml::xistringstream xis( "<parameter name='Point' type='point'>"
                             "  <location type='point'>"
                             "    <point coordinates='" + *( point++ ) + "'/>"
                             "  </location>"
                             "</parameter>" );
    xis >> xml::start( "parameter" );
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Point( kernel::OrderParameter( parameter.GetName(), "path", true ), converter_, xis ) );
}

// -----------------------------------------------------------------------------
// Name: std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreatePathParameter
// Created: PHC 2011-04-13
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreatePathParameter( const kernel::OrderParameter& parameter ) const
{
    static CIT_Points point = points_.begin();
    if( point == points_.end() )
        point = points_.begin();
    xml::xistringstream xis( "<parameter name='Assault route' type='Path'>"
                             "  <parameter name='Pass point1' type='pathpoint'>"
                             "    <location type='point'>"
                             "      <point coordinates='30TYS1037476379'/>"
                             "    </location>"
                             "  </parameter>"
                             "  <parameter name='Destination' type='pathpoint'>"
                             "    <location type='point'>"
                             "      <point coordinates='" + *( point++ ) + "'/>"
                             "    </location>"
                             "  </parameter>"
                             "</parameter>" );
    xis >> xml::start( "parameter" );
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Path( kernel::OrderParameter( parameter.GetName(), "path", true ), converter_, xis ) );
}
