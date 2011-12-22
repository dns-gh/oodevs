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
#include "clients_kernel/Entity_ABC.h"
#include "actions/Agent.h"
#include "actions/AgentKnowledge.h"
#include "actions/AtlasNature.h"
#include "actions/Automat.h"
#include "actions/Bool.h"
#include "actions/DateTime.h"
#include "actions/Direction.h"
#include "actions/DotationType.h"
#include "actions/Enumeration.h"
#include "actions/LimaList.h"
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
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Point.h"
#include <boost/assign/list_of.hpp>
#include "protocol/Protocol.h"

using namespace mission_tester;

namespace
{
    char RandomChar( char upLeft, char downRight )
    {
        if ( upLeft >= downRight )
            return upLeft;
		char res='I';
		while((res=='I') || (res=='O'))
			{
				res = char( rand() % ( downRight + 1 - upLeft ) + upLeft );
			}
        return res; 
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
       
        int prefixX =  atoi(RandomInt( upperLeft.prefixX, lowerRight.prefixX).c_str());
		char prefixY = RandomChar( upperLeft.prefixY, lowerRight.prefixY );
		char charmin = 'A';
		char charmax = 'Z';
		unsigned int max = 99999;
        unsigned int min = 0;
		if ( prefixY == upperLeft.prefixY )
		 charmin=lowerRight.alphaY;
		if ( prefixY == lowerRight.prefixY )
			 charmax=upperLeft.alphaY;
		char alphaY = RandomChar( charmin, charmax );
		if ( prefixX == upperLeft.prefixX )
			 charmin=upperLeft.alphaX;
		if ( prefixX == lowerRight.prefixX )
			 charmax=lowerRight.alphaX;
		char alphaX = RandomChar( charmin, charmax );
		if ((prefixX == upperLeft.prefixX) && (alphaX == upperLeft.alphaX) )
            min = upperLeft.x;
		if ( (prefixX == lowerRight.prefixX) && (alphaX == lowerRight.alphaX) )
            max = lowerRight.x;
        std::string x = RandomInt( min, max );
        if (( prefixY == upperLeft.prefixY ) && (alphaY == upperLeft.alphaY) )
            min = lowerRight.y;
        if (( prefixY == lowerRight.prefixY ) && (alphaY == lowerRight.alphaY) )
            max = upperLeft.y;
        std::string y = RandomInt( min, max );
        return boost::lexical_cast< std::string >(prefixX) + prefixY + alphaX + alphaY + x + y;
    }
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory constructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
ParameterFactory::ParameterFactory( const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller, const kernel::EntityResolver_ABC& entityResolver, xml::xistream& xis, const Model& model )
    : converter_     ( converter )
    , controller_    ( controller )
    , entityResolver_( entityResolver )
    , upperLeft_     ( xis.attribute< std::string >( "upper-left-point", "ppp" ) )
    , lowerRight_    ( xis.attribute< std::string >( "lower-right-point", "ppp" ) )
    , model_         ( model )
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
	//if( parameter.GetType() == "locationcomposite" )
	//	return CreateLocationCompositeParameter( parameter );
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
	if( parameter.GetType() == "maintenancepriorities" )
		return CreateMaintenancePrioritiesParameter( parameter );
	if( parameter.GetType() == "path" )
        return CreatePathParameter( parameter/*, target*/ );
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
	if( parameter.GetType() == "locationcomposite" )
		return CreateLocationCompositeParameter( parameter );
    return std::auto_ptr< actions::Parameter_ABC >();
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreatePhaselineParameter
// Created: PHC 2011-04-13
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreatePhaselineParameter( const kernel::OrderParameter& parameter ) const
{
    xml::xistringstream xis( "<parameter name='Limas' type='phaseline'>"
                             "  <parameter name='Limas (item 1)' type='lima' value='LDM'>"
                             "    <parameter name='Position' type='location'>"
                             "      <location type='line'>"
                             "        <point coordinates='" + ComputeCoord( upperLeft_, lowerRight_ ) + "'/>"
                             "        <point coordinates='" + ComputeCoord( upperLeft_, lowerRight_ ) + "'/>"
                             "      </location>"
                             "    </parameter>"
                             "    <parameter name='Horaire' type='datetime' value='20101023T120001'/>"
                             "  </parameter>"
                             "</parameter>" );
    xis >> xml::start( "parameter" );
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::LimaList( parameter, converter_, xis ) );
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
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreatePathParameter( const kernel::OrderParameter& parameter/*, const Entity_ABC& target*/ ) const
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
    kernel::Automat_ABC* exist = 0;
    unsigned int id = 0;
    while ( !exist )
    {
        id = rand() % 1000;
        exist = model_.FindAutomat( id );
    }
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Automat( parameter, id, entityResolver_, controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateAgentParameter
// Created: PHC 2011-04-19
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateAgentParameter( const kernel::OrderParameter& parameter ) const
{
    kernel::Agent_ABC* exist = 0;
    unsigned int id = 0;
    while ( !exist )
    {
        id = rand() % 1000;
        exist = model_.FindAgent( id );
    }
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Agent( parameter, id, entityResolver_, controller_ ) );
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
    std::string firstPoint = ComputeCoord( upperLeft_, lowerRight_ );
    polygonString +=        "    <point coordinates='" + firstPoint + "'/>";
    for( unsigned int it = 1; it < pointQuantity; ++it )
        polygonString +=        "    <point coordinates='" + ComputeCoord( upperLeft_, lowerRight_ ) + "'/>";
    polygonString +=            "    <point coordinates='" + firstPoint + "'/>"
                                "  </location>"
                                "</parameter>";
    xml::xistringstream xis( polygonString );
    xis >> xml::start( "parameter" );
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::Polygon( kernel::OrderParameter( parameter.GetName(), "path", true ), converter_, xis ) );
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
// Created: RCD 2011-05-08
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateDateTimeParameter( const kernel::OrderParameter& parameter ) const
{
    xml::xistringstream xis( "<parameter name='heure destruction' type='datetime' value='20101023T120001'/>" );
    xis >> xml::start( "parameter" );
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::DateTime( parameter, xis ) );
}

class TestKnowledgeParameter : public actions::Parameter_ABC
{
    public:
        TestKnowledgeParameter( const std::string& name, int id ) : actions::Parameter_ABC( name.c_str() ), id_( id ) {}

        virtual void CommitTo( sword::MissionParameter& message ) const
        {
            FillParameter( *message.mutable_value()->Add()->mutable_agentknowledge() );
        }
        virtual void CommitTo( sword::MissionParameter_Value& message ) const
        {
            FillParameter( *message.mutable_agentknowledge() );
        }
        void FillParameter( ::sword::UnitKnowledgeId& message ) const
        {
            message.set_id( id_ );
        }

    private:
        int id_;
};

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateAgentKnowledgeParameter
// Created: RCD 2011-05-08
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateAgentKnowledgeParameter( const kernel::OrderParameter& parameter ) const
{
    kernel::Agent_ABC* exist = 0;
    unsigned int id = 0;
    while ( !exist )
    {
        id = rand() % 1000;
        exist = model_.FindAgent( id );
    }
    return std::auto_ptr< actions::Parameter_ABC >( new TestKnowledgeParameter( parameter.GetName(), id ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateObjectKnowledgeParameter
// Created: RCD 2011-05-08
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateObjectKnowledgeParameter( const kernel::OrderParameter& parameter ) const
{
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::ObjectKnowledge( parameter, controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateCrowdKnowledgeParameter
// Created: RCD 2011-05-08
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateCrowdKnowledgeParameter( const kernel::OrderParameter& parameter ) const
{
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::PopulationKnowledge( parameter, controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateMedicalPrioritiesParameter
// Created: RCD 2011-05-08
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
    return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::MedicalPriorities( parameter, xis ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateMaintenancePrioritiesParameter
// Created: RCD 2011-05-09
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateMaintenancePrioritiesParameter( const kernel::OrderParameter& parameter ) const
{
	std::string priorities;
	int num = 4;
	unsigned int PrioritiesList[ 369 ];
	for (unsigned int it = 0; it < 369; ++it )
		PrioritiesList[ it ] = it + 1;
	unsigned int prioritiesQuantity = rand() % 369;
	for( unsigned int it = 1; it <= prioritiesQuantity; ++it )
	{
		unsigned int res = rand() % num;
		priorities += PrioritiesList[ res ];
		PrioritiesList[res] = PrioritiesList[ num - 1 ];
		--num;
		if( it != prioritiesQuantity )
			priorities += ';';
	}
	xml::xistringstream xis( "<parameter name='Reparations prioritaires' type='maintenancepriorities' value='" + priorities + "'/>" );
	return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::MaintenancePriorities( parameter ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreateLocationCompositeParameter
// Created: RCD 2011-05-12
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreateLocationCompositeParameter( const kernel::OrderParameter& parameter ) const
{
    std::string choice = parameter.GetChoice( 0 );
    if( choice == "agent" )
        return CreateAgentParameter( parameter );
    if( choice == "point" )
        return CreatePointParameter( parameter );
    if( choice == "polygon" )
        return CreatePolygonParameter( parameter );
    choice = parameter.GetChoice( 1 );
    if( choice == "agent" )
        return CreateAgentParameter( parameter );
    if( choice == "point" )
        return CreatePointParameter( parameter );
	if( choice == "polygon" )
		return CreatePolygonParameter( parameter );
	return std::auto_ptr< actions::Parameter_ABC >();
}

// -----------------------------------------------------------------------------
// Name: ParameterFactory::CreatePlannedWorkParameter
// Created: RCD 2011-05-12
// -----------------------------------------------------------------------------
std::auto_ptr< actions::Parameter_ABC > ParameterFactory::CreatePlannedWorkParameter( const kernel::OrderParameter& parameter ) const
{
	return std::auto_ptr< actions::Parameter_ABC >( new actions::parameters::ObstacleType( parameter, 1 ) );
}