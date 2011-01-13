// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "ActionParameterFactory.h"
#include "Limit.h"
#include "Lima.h"
#include "LimaList.h"
#include "EngineerConstruction.h"
#include "Objective.h"
#include "Direction.h"
#include "Location.h"
#include "Point.h"
#include "Polygon.h"
#include "Path.h"
#include "Agent.h"
#include "Automat.h"
#include "AgentKnowledge.h"
#include "PopulationKnowledge.h"
#include "ObjectKnowledge.h"
#include "Enumeration.h"
#include "Bool.h"
#include "String.h"
#include "Numeric.h"
#include "DotationType.h"
#include "AtlasNature.h"
#include "MaintenancePriorities.h"
#include "MedicalPriorities.h"
#include "DateTime.h"
#include "UrbanBlock.h"
#include "Army.h"
#include "Formation.h"
#include "Quantity.h"
#include "Identifier.h"
#include "ParameterList.h"
#include "KnowledgeGroup.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"
#include <urban/model.h>
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 1 )
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterFactory::ActionParameterFactory( const kernel::CoordinateConverter_ABC& converter, const kernel::EntityResolver_ABC& entities, const kernel::StaticModel& staticModel
                                              , kernel::AgentKnowledgeConverter_ABC& agentKnowledgeConverter, kernel::ObjectKnowledgeConverter_ABC& objectKnowledgeConverter
                                              , kernel::UrbanKnowledgeConverter_ABC& urbanKnowledgeConverter, kernel::Controller& controller )
    : converter_( converter )
    , entities_( entities )
    , staticModel_( staticModel )
    , agentKnowledgeConverter_( agentKnowledgeConverter )
    , objectKnowledgeConverter_( objectKnowledgeConverter )
    , urbanKnowledgeConverter_( urbanKnowledgeConverter )
    , controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory destructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterFactory::~ActionParameterFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
actions::Parameter_ABC* ActionParameterFactory::CreateParameter( const kernel::OrderParameter& parameter, const sword::MissionParameter& message, const kernel::Entity_ABC& entity ) const
{
    // $$$$ SBO 2007-10-11: we should create a parameter of the real type in order to be able (later) to edit parameters
    if( message.null_value() || message.value_size() == 0 )
        return new actions::parameters::String( parameter, tools::translate( "ActionParameterFactory", "not set" ).ascii() );
    else if( !parameter.IsList() && message.value_size() == 1 )
        return CreateParameter( parameter, message.value().Get( 0 ), entity );
    else
        return new actions::parameters::ParameterList( parameter, message.value(), *this, entity );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: MGD 2010-11-09
// -----------------------------------------------------------------------------
actions::Parameter_ABC* ActionParameterFactory::CreateParameter( const kernel::OrderParameter& parameter, const sword::MissionParameter_Value& message, const kernel::Entity_ABC& entity ) const
{
    if( message.has_booleanvalue() )
        return new actions::parameters::Bool( parameter, message.booleanvalue() != 0 );
    if( message.has_acharstr() )
        return new actions::parameters::String( parameter, message.acharstr().c_str() );
    if( message.has_agent() )
        return new actions::parameters::Agent( parameter, message.agent().id(), entities_, controller_ );
    if( message.has_areal() )
        return new actions::parameters::Numeric( parameter, message.areal() );
    if( message.has_automat() )
        return new actions::parameters::Automat( parameter, message.automat().id(), entities_, controller_ );
    if( message.has_heading() )
        return new actions::parameters::Direction( parameter, message.heading().heading() );
    if( message.has_enumeration() )
        return new actions::parameters::Enumeration( parameter, message.enumeration() );
    if( message.has_path() )
        return new actions::parameters::Path( parameter, converter_, message.path().location() );
    if( message.has_agentknowledge() )
        return new actions::parameters::AgentKnowledge( parameter, message.agentknowledge().id(), agentKnowledgeConverter_, entity, controller_ );
    if( message.has_objectknowledge() )
        return new actions::parameters::ObjectKnowledge( parameter, message.objectknowledge().id(), objectKnowledgeConverter_, entity, controller_ );
    if( message.has_crowdknowledge() )
        return new actions::parameters::PopulationKnowledge( parameter, message.crowdknowledge().id(), agentKnowledgeConverter_, entity, controller_ );
    if( message.has_location() )
        return new actions::parameters::Location( parameter, converter_, message.location() );
    if( message.has_plannedwork() )
        return new actions::parameters::EngineerConstruction( parameter, converter_, staticModel_.objectTypes_, entities_, message.plannedwork(), controller_ );
    if( message.has_atlasnature() )
        return new actions::parameters::AtlasNature( parameter, message.atlasnature(), staticModel_.atlasNatures_ );
    if( message.has_missionobjective() )
        return new actions::parameters::Objective( parameter, converter_, message.missionobjective() );
    if( message.has_point() )
        return new actions::parameters::Point( parameter, converter_, message.point() );
    if( message.has_area() )
        return new actions::parameters::Polygon( parameter, converter_, message.area().location() );
    if( message.has_resourcetype() )
        return new actions::parameters::DotationType( parameter, message.resourcetype().id(), staticModel_.objectTypes_ );
    if( message.has_equipmenttype() )
        return 0;
    if( message.has_logmaintenancepriorities() )
        return new actions::parameters::MaintenancePriorities( parameter, staticModel_.objectTypes_, message.logmaintenancepriorities() );
    if( message.has_logmedicalpriorities() )
        return new actions::parameters::MedicalPriorities( parameter, message.logmedicalpriorities() );
    if( message.has_tirindirect() ) // $$$$ SBO 2007-05-21: reports only, not to be used!
        return 0;
    if( message.has_limit() )
        return new actions::parameters::Limit( parameter, converter_, message.limit() );
    if( message.has_limasorder() )
        return new actions::parameters::LimaList( parameter, converter_, message.limasorder() );
    if( message.has_datetime() )
        return new actions::parameters::DateTime( parameter, message.datetime() );
    if( message.has_urbanknowledge() )
        return new actions::parameters::UrbanBlock( parameter, message.urbanknowledge().id(), urbanKnowledgeConverter_, entity, controller_  );
    if( message.has_party() )
        return new actions::parameters::Army( parameter, message.party().id(), entities_, controller_ );
    if( message.has_formation() )
        return new actions::parameters::Formation( parameter, message.formation().id(), entities_, controller_ );
    if( message.has_quantity() )
        return new actions::parameters::Quantity( parameter, message.quantity() );
    if( message.has_identifier() )
        return new actions::parameters::Identifier( parameter, message.identifier() );
    if( message.has_knowledgegroup() )
        return new actions::parameters::KnowledgeGroup( parameter, message.knowledgegroup().id(), entities_, controller_ );
    if( message.list_size() )
        return new actions::parameters::ParameterList( parameter );
    return 0;
}

namespace
{
    void ThrowUnexpected( const kernel::OrderParameter& expected, xml::xistream& xis )
    {
        const std::string found = xis.attribute< std::string >( "name" );
        throw std::exception( tools::translate( "ActionParameterFactory", "Expecting '%1' found '%2'" ).arg( expected.GetName().c_str() ).arg( found.c_str() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
actions::Parameter_ABC* ActionParameterFactory::CreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Entity_ABC& entity ) const
{
    std::string type = boost::algorithm::to_lower_copy( xis.attribute< std::string >( "type" ) );
    type = parameter.CompatibleType( type );
    if( type == "" )
        ThrowUnexpected( parameter, xis );
    std::auto_ptr< actions::Parameter_ABC > param;
    bool found = DoCreateParameter( parameter, xis, entity, type, param );
    if( found == false )
        found = DoCreateParameter( parameter, xis, type, param );
    if( found == false )
        throw std::runtime_error( "Unknown parameter type '" + type + "'" );
    return param.release();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
actions::Parameter_ABC* ActionParameterFactory::CreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis ) const
{
    std::string type = boost::algorithm::to_lower_copy( xis.attribute< std::string >( "type" ) );
    type = parameter.CompatibleType( type );
    if( type == "" )
        ThrowUnexpected( parameter, xis );
    std::auto_ptr< actions::Parameter_ABC > param;
    if( DoCreateParameter( parameter, xis, type, param ) == false )
        throw std::runtime_error( "Unknown parameter type '" + type + "'" );
    return param.release();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateListParameter
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void ActionParameterFactory::CreateListParameter( xml::xistream& xis, actions::parameters::ParameterList& list ) const
{
    const std::string name = xis.attribute< std::string >( "name" );
    const std::string type = boost::algorithm::to_lower_copy( xis.attribute< std::string >( "type" ) );
    list.AddParameter( *CreateParameter( kernel::OrderParameter( name, type, false ), xis ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateListParameter
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void ActionParameterFactory::CreateListParameter( xml::xistream& xis, actions::parameters::ParameterList& list, const kernel::Entity_ABC& entity ) const
{
    const std::string name = xis.attribute< std::string >( "name" );
    const std::string type = boost::algorithm::to_lower_copy( xis.attribute< std::string >( "type" ) );
    list.AddParameter( *CreateParameter( kernel::OrderParameter( name, type, false ), xis, entity ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::DoCreateParameter
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
bool ActionParameterFactory::DoCreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis, const std::string& type, std::auto_ptr< actions::Parameter_ABC >& param ) const
{
    if( parameter.IsList() || type == "list" || type == "locationcomposite" )
    {
        actions::parameters::ParameterList* parameterList = new actions::parameters::ParameterList( parameter );
        param.reset( parameterList );
        xis >> xml::list( "parameter", *this, &ActionParameterFactory::CreateListParameter, *parameterList );
    }
    else if( type == "bool" || type == "boolean" )
        param.reset( new actions::parameters::Bool( parameter, xis ) );
    else if( type == "numeric" )
        param.reset( new actions::parameters::Numeric( parameter, xis ) );
    else if( type == "integer" )
        param.reset( new actions::parameters::Numeric( parameter, xis ) );
    else if( type == "string" )
        param.reset( new actions::parameters::String( parameter, xis ) );
    else if( type == "path" )
        param.reset( new actions::parameters::Path( parameter, converter_, xis ) );
    else if( type == "point" )
        param.reset( new actions::parameters::Point( parameter, converter_, xis ) );
    else if( type == "polygon" )
        param.reset( new actions::parameters::Polygon( parameter, converter_, xis ) );
    else if( type == "location" )
        param.reset( new actions::parameters::Location( parameter, converter_, xis ) );
    else if( type == "heading" )
        param.reset( new actions::parameters::Direction( parameter, xis ) );
    else if( type == "phaseline" )
        param.reset( new actions::parameters::Lima( converter_, xis ) );
    else if( type == "limit" )
        param.reset( new actions::parameters::Limit( parameter, converter_, xis ) );
    else if( type == "enumeration" )
        param.reset( new actions::parameters::Enumeration( parameter, xis ) );
    else if( type == "agent" )
        param.reset( new actions::parameters::Agent( parameter, xis, entities_, controller_ ) );
    else if( type == "automate" )
        param.reset( new actions::parameters::Automat( parameter, xis, entities_, controller_ ) );
    else if( type == "automat" )
        param.reset( new actions::parameters::Automat( parameter, xis, entities_, controller_ ) );
    else if( type == "army" )
        param.reset( new actions::parameters::Army( parameter, xis, entities_, controller_ ) );
    else if( type == "formation" )
        param.reset( new actions::parameters::Formation( parameter, xis, entities_, controller_ ) );
    else if( type == "dotationtype" )
        param.reset( new actions::parameters::DotationType( parameter, xis, staticModel_.objectTypes_ ) );
    else if( type == "resourcetype" )
        param.reset( new actions::parameters::DotationType( parameter, xis, staticModel_.objectTypes_ ) );
    else if( type == "genobject" )
        param.reset( new actions::parameters::EngineerConstruction( parameter, converter_, staticModel_.objectTypes_, entities_, xis, controller_ ) );
    else if( type == "plannedwork" )
        param.reset( new actions::parameters::EngineerConstruction( parameter, converter_, staticModel_.objectTypes_, entities_, xis, controller_ ) );
    else if( type == "natureatlas" )
        param.reset( new actions::parameters::AtlasNature( parameter, xis, staticModel_.atlasNatures_ ) );
    else if( type == "objective" )
        param.reset( new actions::parameters::Objective( parameter, xis, converter_ ) );
    else if( type == "medicalpriorities" )
        param.reset( new actions::parameters::MedicalPriorities( parameter, xis ) );
    else if( type == "maintenancepriorities" )
        param.reset( new actions::parameters::MaintenancePriorities( parameter, staticModel_.objectTypes_, xis ) );
    else if( type == "datetime" )
        param.reset( new actions::parameters::DateTime( parameter, xis ) );
    else if( type == "quantity" )
        param.reset( new actions::parameters::Quantity( parameter, xis ) );
    else if( type == "identifier" )
        param.reset( new actions::parameters::Identifier( parameter, xis ) );
    else if( type == "knowledgegroup" )
        param.reset( new actions::parameters::KnowledgeGroup( parameter, xis, entities_, controller_ ) );
    else
        return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::DoCreateParameter
// Created: MGD 2010-11-10
// -----------------------------------------------------------------------------
bool ActionParameterFactory::DoCreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Entity_ABC& entity, const std::string& type, std::auto_ptr< actions::Parameter_ABC >& param ) const
{
    if( parameter.IsList() || type == "list" || type == "locationcomposite" )
    {
        actions::parameters::ParameterList* parameterList = new actions::parameters::ParameterList( parameter );
        param.reset( parameterList );
        xis >> xml::list( "parameter", *this, &ActionParameterFactory::CreateListParameter, *parameterList, entity );
    }
    else if( type == "agentknowledge" )
        param.reset( new actions::parameters::AgentKnowledge( parameter, xis, entities_, agentKnowledgeConverter_, entity, controller_ ) );
    else if( type == "populationknowledge" )
        param.reset( new actions::parameters::PopulationKnowledge( parameter, xis, entities_, agentKnowledgeConverter_, entity, controller_ ) );
    else if( type == "objectknowledge" )
        param.reset( new actions::parameters::ObjectKnowledge( parameter, xis, entities_, objectKnowledgeConverter_, entity, controller_ ) );
    else if( type == "urbanknowledge" )
        param.reset( new actions::parameters::UrbanBlock( parameter, xis, urbanKnowledgeConverter_, entity, controller_ ) );
    else
        return false;

    return true;
}
