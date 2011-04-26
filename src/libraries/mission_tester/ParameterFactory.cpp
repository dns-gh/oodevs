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
#include "actions/Agent.h"
#include "actions/Bool.h"
#include "actions/Limit.h"
#include "actions/Lima.h"
#include "actions/Numeric.h"
#include "actions/Path.h"
#include "actions/Point.h"
#include "actions/Polygon.h"
#include "actions/ParameterList.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Controller.h"
#include <boost/assign/list_of.hpp>

using namespace mission_tester;

namespace
{
    void ReadLimit( std::pair< std::string, std::string >& limit, xml::xistream& xis )
    {
        limit.first = xis.content< std::string >( "point1" );
        limit.second = xis.content< std::string >( "point2" );
    }
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory constructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
ParameterFactory::ParameterFactory( const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller, const kernel::EntityResolver_ABC& entityResolver, xml::xistream& xis )
    : converter_     ( converter )
    , controller_    ( controller )
    , entityResolver_( entityResolver )
{
    xis >> xml::start( "limits" )
          >> xml::start( "limit1" );
    ReadLimit( limit1_, xis );
    xis   >> xml::end
          >> xml::start( "limit2" );
    ReadLimit( limit2_, xis );
    xis   >> xml::end
          >> xml::start( "lima" );
    ReadLimit( lima_, xis );
    xis   >> xml::end
        >> xml::end
        >> xml::start( "pathfind" )
          >> xml::list( "point", *this, &ParameterFactory::ReadPoint )
        >> xml::end
        >> xml::start( "polygon" )
          >> xml::list( "point", *this, &ParameterFactory::ReadPolygonPoint );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory destructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
ParameterFactory::~ParameterFactory()
{
    // NOTHING
}

void ParameterFactory::ReadPoint( xml::xistream& xis )
{
    points_.push_back( xis.value< std::string >() );
}

void ParameterFactory::ReadPolygonPoint( xml::xistream& xis )
{
    polygon_.push_back( xis.value< std::string >() );
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
    if( parameter.GetType() == "point" || parameter.GetType() == "locationcomposite" )
        return CreatePointParameter( parameter );
    if( parameter.GetType() == "path" )
        return CreatePathParameter( parameter );
    if( parameter.GetType() == "agent" )
        return CreateAgentParameter( parameter );
    if( parameter.GetType() == "boolean" )
        return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Bool( parameter, true ) );
    if( parameter.GetType() == "polygon" )
        return CreatePolygonParameter( parameter );
    return std::auto_ptr< actions::Parameter_ABC >();
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreatePhaselineParameter
// Created: PHC 2011-04-13
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreatePhaselineParameter( const kernel::OrderParameter& parameter ) const
{
    std::auto_ptr< actions::Parameter_ABC > list( new actions::parameters::ParameterList( kernel::OrderParameter( parameter.GetName(), "phaseline", true ) ) );
    xml::xistringstream xis( "<parameter name='Lima' type='lima' value='LDM'>"
                             "  <parameter name='Position' type='location'>"
                             "    <location type='line'>"
                             "      <point coordinates='" + lima_.first + "'/>"
                             "      <point coordinates='" + lima_.second + "'/>"
                             "    </location>"
                             "  </parameter>"
                             "  <parameter name='Horaire' type='datetime' value='20121203T143802'/>"
                             "</parameter>" );
    xis >> xml::start( "parameter" );
    list->AddParameter( *new actions::parameters::Lima( converter_, xis ) );
    return list;
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateLimitParameter
// Created: PHC 2011-04-13
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateLimitParameter( const kernel::OrderParameter& parameter ) const
{
    static bool b = true;
    b = !b;
    xml::xistringstream xis( std::string( "" ) + "<parameter name='Limit' type='Limit'>"
                             "  <location type='line'>"
                           + ( b ? "    <point coordinates='" + limit1_.first + "'/>"
                                   "    <point coordinates='" + limit1_.second + "'/>"
                                 : "    <point coordinates='" + limit2_.first + "'/>"
                                   "    <point coordinates='" + limit2_.second + "'/>" )
                           + "  </location>"
                             "</parameter>" );
    xis >> xml::start( "parameter" );
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Limit( parameter, converter_, xis ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreatePointParameter
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
// Name: ParameterFactory::CreatePathParameter
// Created: PHC 2011-04-13
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreatePathParameter( const kernel::OrderParameter& parameter ) const
{
    static CIT_Points point = points_.begin();
    if( point == points_.end() )
        point = points_.begin();
    xml::xistringstream xis( "<parameter name='Assault route' type='Path'>"
                             "  <parameter name='Destination' type='pathpoint'>"
                             "    <location type='point'>"
                             "      <point coordinates='" + *( point++ ) + "'/>"
                             "    </location>"
                             "  </parameter>"
                             "</parameter>" );
    xis >> xml::start( "parameter" );
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Path( kernel::OrderParameter( parameter.GetName(), "path", true ), converter_, xis ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateAgentParameter
// Created: PHC 2011-04-19
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateAgentParameter( const kernel::OrderParameter& parameter ) const
{
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Agent( parameter, 19u, entityResolver_, controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreatePolygonParameter
// Created: PHC 2011-04-21
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreatePolygonParameter( const kernel::OrderParameter& parameter ) const
{
     xml::xistringstream xis( "<parameter name='Area' type='Polygon'>"
                              "  <location type='polygon'>"
                              "    <point coordinates='" + polygon_[0] + "'/>"
                              "    <point coordinates='" + polygon_[1] + "'/>"
                              "    <point coordinates='" + polygon_[2] + "'/>"
                              "    <point coordinates='" + polygon_[3] + "'/>"
                              "  </location>"
                              "</parameter>" );
    xis >> xml::start( "parameter" );
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Polygon( parameter, converter_, xis ) );
}