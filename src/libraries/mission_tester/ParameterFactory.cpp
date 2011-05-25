// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "stdlib.h"
#include "mission_tester_pch.h"
#include "ParameterFactory.h"
#include "actions/Agent.h"
#include "actions/AgentKnowledge.h"
#include "actions/AtlasNature.h"
#include "actions/Automat.h"
#include "actions/Bool.h"
#include "actions/DateTime.h"
#include "actions/Direction.h"
#include "actions/DotationType.h"
#include "actions/Enumeration.h"
#include "actions/Lima.h"
#include "actions/Limit.h"
#include "actions/MaintenancePriorities.h"
#include "actions/MedicalPriorities.h"
#include "actions/Numeric.h"
#include "actions/ObjectKnowledge.h"
#include "actions/ObstacleType.h"
#include "actions/ParameterList.h"
#include "actions/Path.h"
#include "actions/Point.h"
#include "actions/Polygon.h"
#include "actions/PopulationKnowledge.h"
#include "actions/UrbanBlock.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Point.h"
#include <boost/assign/list_of.hpp>

using namespace mission_tester;

namespace
{
    char RandomChar( char upLeft, char downRight )
    {
        if ( upLeft >= downRight )
            return upLeft;
        return char( rand() % ( downRight + 1 - upLeft ) + upLeft );
    }

    std::string RandomInt( unsigned int min, unsigned int max )
    {
        std::string  res;
        if ( min >= max )
            res = boost::lexical_cast< std::string >( min );
        else
            res = boost::lexical_cast< std::string >( rand() % ( max - min ) + min );
        unsigned int it = 5;
        while( it > res.size() )
            res = '0' + res;
        return res;
    }

    std::string ComputeCoord( Coordinates upperLeft, Coordinates lowerRight )
    {
        std::string prefix = upperLeft.prefix;
        char alphaX = RandomChar( upperLeft.alphaX, lowerRight.alphaX );
        char alphaY = RandomChar( upperLeft.alphaY, lowerRight.alphaY );
        unsigned int max = 99999;
        unsigned int min = 0;
        if ( alphaX == upperLeft.alphaX )
            min = upperLeft.x;
        if ( alphaX == lowerRight.alphaX )
            max = lowerRight.x;
        std::string x = RandomInt( min, max );
        if ( alphaY == upperLeft.alphaY )
            min = upperLeft.y;
        if ( alphaY == lowerRight.alphaY )
            max = lowerRight.y;
        std::string y = RandomInt( min, max );
        return prefix + alphaX + alphaY + x + y;
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
    , upperLeft_     ( xis.attribute< std::string >( "upper-left-point", "ppp" ) )
    , lowerRight_    ( xis.attribute< std::string >( "lower-right-point", "ppp" ) )
{
    srand( time( 0 ) );
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
        return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Direction( parameter, rand() % 360 + 1 ) );
    if( parameter.GetType() == "phaseline" )
        return CreatePhaselineParameter( parameter );
    if( parameter.GetType() == "limit" )
        return CreateLimitParameter( parameter );
    if( parameter.GetType() == "point" )
        return CreatePointParameter( parameter );
    if( parameter.GetType() == "integer" || parameter.GetType() == "numeric"  )
        return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Numeric( parameter, rand() % 2 ) );
    if( parameter.GetType() == "medicalpriorities" )
        return CreateMedicalPrioritiesParameter( parameter );
    if( parameter.GetType() == "path" )
        return CreatePathParameter( parameter );
    if( parameter.GetType() == "boolean" )
        return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Bool( parameter, ( ( rand() % 2 ) == 0 ) ) );
    if( parameter.GetType() == "polygon" )
        return CreatePolygonParameter( parameter );
    if( parameter.GetType() == "natureatlas" )
        return CreateNatureAtlasParameter( parameter );
    if( parameter.GetType() == "enumeration" )
        return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Enumeration( parameter, rand() % 2 ) );
    if( parameter.GetType() == "datetime" )
        return CreateDateTimeParameter( parameter );
    if( parameter.GetType() == "automat" )
        return CreateAutomatParameter( parameter );
    if( parameter.GetType() == "agent" )
        return CreateAgentParameter( parameter );
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
                             "      <point coordinates='" + ComputeCoord( upperLeft_, lowerRight_ ) + "'/>"
                             "      <point coordinates='" + ComputeCoord( upperLeft_, lowerRight_ ) + "'/>"
                             "    </location>"
                             "  </parameter>"
                             "  <parameter name='Horaire' type='datetime' value='20771023T120000'/>"
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
    xml::xistringstream xis( std::string( "" ) + "<parameter name='Limit' type='Limit'>"
                             "  <location type='line'>"
                             "    <point coordinates='" + ComputeCoord( upperLeft_, lowerRight_ ) + "'/>"
                             "    <point coordinates='" + ComputeCoord( upperLeft_, lowerRight_ ) + "'/>"
                             "  </location>"
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
    xml::xistringstream xis( "<parameter name='Point' type='point'>"
                             "  <location type='point'>"
                             "    <point coordinates='" + ComputeCoord( upperLeft_, lowerRight_ ) + "'/>"
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
    std::string pathString = "<parameter name='Assault route' type='Path'>";
    unsigned int pointQuantity = rand() % 3 + 1;
    for( unsigned int it = 0; it < pointQuantity; ++it )
    {
        pathString += "  <parameter name='Destination' type='pathpoint'>"
                      "    <location type='point'>"
                      "      <point coordinates='" + ComputeCoord( upperLeft_, lowerRight_ ) + "'/>"
                      "    </location>"
                      "  </parameter>";
    }
    pathString += "</parameter>";
    xml::xistringstream xis( pathString );
    xis >> xml::start( "parameter" );
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Path( kernel::OrderParameter( parameter.GetName(), "path", true ), converter_, xis ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateAutomatParameter
// Created: PHC 2011-04-19
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateAutomatParameter( const kernel::OrderParameter& parameter ) const
{
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Automat( parameter, 11, entityResolver_, controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateAgentParameter
// Created: PHC 2011-04-19
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateAgentParameter( const kernel::OrderParameter& parameter ) const
{
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Agent( parameter, 16, entityResolver_, controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreatePolygonParameter
// Created: PHC 2011-04-21
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreatePolygonParameter( const kernel::OrderParameter& parameter ) const
{
    std::string polygonString = "<parameter name='Area' type='Polygon'>"
                                "  <location type='polygon'>";
    unsigned int pointQuantity = rand() % 8 + 3;
    for( unsigned int it = 0; it < pointQuantity; ++it )
        polygonString +=        "    <point coordinates='" + ComputeCoord( upperLeft_, lowerRight_ ) + "'/>";
    polygonString +=            "  </location>"
                                "</parameter>";
    xml::xistringstream xis( polygonString );
    xis >> xml::start( "parameter" );
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Path( kernel::OrderParameter( parameter.GetName(), "path", true ), converter_, xis ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateNatureAtlasParameter
// Created: RCD 2011-04-27
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateNatureAtlasParameter( const kernel::OrderParameter& parameter ) const
{
    unsigned short value = rand() % 512 + ( rand() % 2 ) * 1024;
    const kernel::AtlasNature atlas( "atlasNature", value );
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::AtlasNature( parameter, atlas ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateDateTimeParameter
// Created: RCD 2011-04-28
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateDateTimeParameter( const kernel::OrderParameter& parameter ) const
{
    xml::xistringstream xis( "<parameter name='Heure destruction' type='datetime' value='20771023T120000'/>" );
    xis >> xml::start( "parameter" );
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::DateTime( parameter, xis ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateAgentKnowledgeParameter
// Created: RCD 2011-04-28
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateAgentKnowledgeParameter( const kernel::OrderParameter& parameter ) const
{
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::AgentKnowledge( parameter, controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateObjectKnowledgeParameter
// Created: RCD 2011-04-28
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateObjectKnowledgeParameter( const kernel::OrderParameter& parameter ) const
{
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::ObjectKnowledge( parameter, controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateCrowdKnowledgeParameter
// Created: RCD 2011-04-28
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateCrowdKnowledgeParameter( const kernel::OrderParameter& parameter ) const
{
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::PopulationKnowledge( parameter, controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateMedicalPrioritiesParameter
// Created: RCD 2011-04-28
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateMedicalPrioritiesParameter( const kernel::OrderParameter& parameter ) const
{
    std::string priorities;
    int num = 4;
    unsigned int PrioritiesList[ 4 ] = { 2, 3, 4, 5 };
    unsigned int prioritiesQuantity = rand() % 5;
    for( unsigned int it = 1; it <= prioritiesQuantity; ++it )
    {
        unsigned int res = rand() % num;
        priorities += PrioritiesList[ res ];
        PrioritiesList[res] = PrioritiesList[ num - 1 ];
        --num;
        if( it != prioritiesQuantity )
            priorities += ';';
    }
    xml::xistringstream xis( "<parameter name='Blessures traitees' type='medicalpriorities' value='" + priorities + "'/>" );
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::MedicalPriorities( parameter ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateMaintenancePrioritiesParameter
// Created: RCD 2011-04-28
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateMaintenancePrioritiesParameter( const kernel::OrderParameter& parameter ) const
{
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::MaintenancePriorities( parameter ) );
}
