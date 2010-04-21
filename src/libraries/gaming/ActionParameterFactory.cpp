// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterFactory.h"
#include "actions/Limit.h"
#include "actions/LimaList.h"
#include "actions/EngineerConstruction.h"
#include "actions/EngineerConstructionList.h"
#include "actions/Objective.h"
#include "actions/ObjectiveList.h"
#include "actions/Direction.h"
#include "actions/Location.h"
#include "actions/Point.h"
#include "actions/Polygon.h"
#include "actions/PointList.h"
#include "actions/PolygonList.h"
#include "actions/LocationList.h"
#include "actions/PathList.h"
#include "actions/Path.h"
#include "actions/Agent.h"
#include "actions/Automat.h"
#include "actions/AgentList.h"
#include "actions/AutomatList.h"
#include "actions/AgentKnowledge.h"
#include "actions/PopulationKnowledge.h"
#include "actions/ObjectKnowledge.h"
#include "actions/AgentKnowledgeList.h"
#include "actions/ObjectKnowledgeList.h"
#include "actions/Enumeration.h"
#include "actions/Bool.h"
#include "actions/String.h"
#include "actions/Numeric.h"
#include "actions/DotationType.h"
#include "actions/AtlasNature.h"
#include "actions/MaintenancePriorities.h"
#include "actions/MedicalPriorities.h"
#include "actions/IntelligenceList.h"
#include "actions/DateTime.h"
#include "actions/UrbanBlock.h"
#include "actions/Army.h"
#include "actions/Formation.h"
#include "actions/Quantity.h"
#include "actions/Identifier.h"
#include "actions/ParameterList.h"
#include "actions/KnowledgeGroup.h"
#include "Model.h"
#include "StaticModel.h"
#include "AgentsModel.h"
#include "TeamsModel.h"
#include "ObjectsModel.h"
#include "KnowledgeGroupsModel.h"
#include "Tools.h"
#include "UrbanModel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/ObjectTypes.h"
#include "protocol/Protocol.h"
#include <urban/model.h>
#include <urban/blockmodel.h>
#include <xeumeuleu/xml.h>
#pragma warning( push, 1 )
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterFactory::ActionParameterFactory( const CoordinateConverter_ABC& converter, const Model& model, const StaticModel& staticModel
                                              , AgentKnowledgeConverter_ABC& agentKnowledgeConverter, ObjectKnowledgeConverter_ABC& objectKnowledgeConverter
                                              , kernel::Controller& controller )
    : converter_( converter )
    , model_( model )
    , staticModel_( staticModel )
    , agentKnowledgeConverter_( agentKnowledgeConverter )
    , objectKnowledgeConverter_( objectKnowledgeConverter )
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
actions::Parameter_ABC* ActionParameterFactory::CreateParameter( const OrderParameter& parameter, const Common::MsgMissionParameter& message, const Entity_ABC& entity ) const
{
    // $$$$ SBO 2007-10-11: we should create a parameter of the real type in order to be able (later) to edit parameters
    if( message.null_value() )
        return new actions::parameters::Parameter< QString >( parameter, tools::translate( "ActionParameterFactory", "not set" ) );
    if( message.value().has_abool() )
        return new actions::parameters::Bool( parameter, message.value().abool() != 0 );
    if( message.value().has_acharstr() )
        return new actions::parameters::String( parameter, message.value().acharstr().c_str() );
    if( message.value().has_unit() )
        return new actions::parameters::Agent( parameter, message.value().unit().oid(), model_.agents_, controller_ );
    if( message.value().has_areal() )
        return new actions::parameters::Numeric( parameter, message.value().areal() );
    if( message.value().has_automat() )
        return new actions::parameters::Automat( parameter, message.value().automat().oid(), model_.agents_, controller_ );
    if( message.value().has_heading() )
        return new actions::parameters::Parameter< int >( parameter, message.value().heading().heading() );
    if( message.value().has_enumeration() )
        return new actions::parameters::Enumeration( parameter, message.value().enumeration() );
    if( message.value().has_path() )
        return new actions::parameters::Path( parameter, converter_, message.value().path().location() );
    if( message.value().has_unitknowledge() )
        return new actions::parameters::AgentKnowledge( parameter, message.value().unitknowledge().oid(), agentKnowledgeConverter_, entity, controller_ );
    if( message.value().has_objectknowledge() )
        return new actions::parameters::ObjectKnowledge( parameter, message.value().objectknowledge().oid(), objectKnowledgeConverter_, entity, controller_ );
    if( message.value().has_populationknowledge() )
        return new actions::parameters::PopulationKnowledge( parameter, message.value().populationknowledge().oid(), agentKnowledgeConverter_, entity, controller_ );
    if( message.value().has_unitlist() )
        return new actions::parameters::AgentList( parameter, message.value().unitlist(), model_.agents_, controller_ );
    if( message.value().has_automatlist() )
        return new actions::parameters::AutomatList( parameter, message.value().automatlist(), model_.agents_, controller_ );
    if( message.value().has_pathlist() )
        return new actions::parameters::PathList( parameter, converter_, message.value().pathlist() );
    if( message.value().has_unitknowledgelist() )
        return new actions::parameters::AgentKnowledgeList( parameter, message.value().unitknowledgelist(), agentKnowledgeConverter_, entity, controller_ );
    if( message.value().has_objectknowledgelist() )
        return new actions::parameters::ObjectKnowledgeList( parameter, message.value().objectknowledgelist(), objectKnowledgeConverter_, entity, controller_ );
    if( message.value().has_locationlist() )
        return new actions::parameters::LocationList( parameter, converter_, message.value().locationlist() );
    if( message.value().has_plannedworklist() )
        return new actions::parameters::EngineerConstructionList( parameter, converter_, staticModel_.objectTypes_, model_.agents_, message.value().plannedworklist(), controller_ );
    if( message.value().has_pointlist() )
        return new actions::parameters::PointList( parameter, converter_, message.value().pointlist() );
    if( message.value().has_polygonlist() )
        return new actions::parameters::PolygonList( parameter, converter_, message.value().polygonlist() );
    if( message.value().has_location() )
        return new actions::parameters::Location( parameter, converter_, message.value().location() );
    if( message.value().has_plannedwork() )
        return new actions::parameters::EngineerConstruction( parameter, converter_, staticModel_.objectTypes_, model_.agents_, message.value().plannedwork(), controller_ );
    if( message.value().has_atlasnature() )
        return new actions::parameters::AtlasNature( parameter, message.value().atlasnature(), staticModel_.atlasNatures_ );
    if( message.value().has_missionobjective() )
        return new actions::parameters::Objective( parameter, converter_, message.value().missionobjective() );
    if( message.value().has_missionobjectivelist() )
        return new actions::parameters::ObjectiveList( parameter, converter_, message.value().missionobjectivelist() );
    if( message.value().has_point() )
        return new actions::parameters::Point( parameter, converter_, message.value().point() );
    if( message.value().has_polygon() )
        return new actions::parameters::Polygon( parameter, converter_, message.value().polygon().location() );
    if( message.value().has_dotationtype() )
        return new actions::parameters::DotationType( parameter, message.value().dotationtype().oid(), staticModel_.objectTypes_ );
    if( message.value().has_equipmenttype() )
        return 0;
    if( message.value().has_logmaintenancepriorities() )
        return new actions::parameters::MaintenancePriorities( parameter, staticModel_.objectTypes_, message.value().logmaintenancepriorities() );
    if( message.value().has_logmedicalpriorities() )
        return new actions::parameters::MedicalPriorities( parameter, message.value().logmedicalpriorities() );
    if( message.value().has_tirindirect() ) // $$$$ SBO 2007-05-21: reports only, not to be used!
        return 0;
    if( message.value().has_line() )
        return new actions::parameters::Limit( parameter, converter_, message.value().line() );
    if( message.value().has_limasorder() )
        return new actions::parameters::LimaList( parameter, converter_, message.value().limasorder() );
    if( message.value().has_intelligencelist() )
        return new actions::parameters::IntelligenceList( parameter, converter_, message.value().intelligencelist(), model_.teams_, staticModel_.levels_, controller_ );
    if( message.value().has_datetime() )
        return new actions::parameters::DateTime( parameter, message.value().datetime() );
    if( message.value().has_urbanblock() )
        return new actions::parameters::UrbanBlock( parameter, message.value().urbanblock() );
    if( message.value().has_army() )
        return new actions::parameters::Army( parameter, message.value().army(), model_.teams_, controller_ );
    if( message.value().has_formation() )
        return new actions::parameters::Formation( parameter, message.value().formation(), model_.teams_, controller_ );
    if( message.value().has_quantity() )
        return new actions::parameters::Quantity( parameter, message.value().quantity() );
    if( message.value().has_identifier() )
        return new actions::parameters::Identifier( parameter, message.value().identifier() );
    if( message.value().has_knowledgegroup() )
        return new actions::parameters::KnowledgeGroup( parameter, message.value().knowledgegroup(), model_.knowledgeGroups_, controller_ );
    if( message.value().list_size() )
        return new actions::parameters::ParameterList( parameter );
    return 0;
}

namespace
{
    void ThrowUnexpected( const OrderParameter& expected, xml::xistream& xis )
    {
        const std::string found = xml::attribute< std::string >( xis, "name" );
        throw std::exception( tools::translate( "ActionParameterFactory", "Expecting '%1' found '%2'" ).arg( expected.GetName().c_str() ).arg( found.c_str() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
actions::Parameter_ABC* ActionParameterFactory::CreateParameter( const OrderParameter& parameter, xml::xistream& xis, const kernel::Entity_ABC& entity ) const
{
    std::string expected = boost::algorithm::to_lower_copy( parameter.GetType() );
    std::string type = boost::algorithm::to_lower_copy( xml::attribute< std::string >( xis, "type" ) );
    if( type != expected )
        ThrowUnexpected( parameter, xis );
    std::auto_ptr< actions::Parameter_ABC > param;
    
    bool found = DoCreateParameter( parameter, xis, entity, type, param );
    if( found == false )
        found = DoCreateParameter( parameter, xis, type, param );

    if( found == false )
        throw std::runtime_error( "Unknown parameter type '" + type + "'" );

    param->Set( true ); // $$$$ SBO 2007-10-11: ...
    return param.release();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
actions::Parameter_ABC* ActionParameterFactory::CreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis ) const
{
    std::string expected = boost::algorithm::to_lower_copy( parameter.GetType() );
    std::string type = boost::algorithm::to_lower_copy( xml::attribute< std::string >( xis, "type" ) );
    if( type != expected )
        ThrowUnexpected( parameter, xis );
    std::auto_ptr< actions::Parameter_ABC > param;
    
    if( DoCreateParameter( parameter, xis, type, param ) == false )
        throw std::runtime_error( "Unknown parameter type '" + type + "'" );

    param->Set( true ); // $$$$ SBO 2007-10-11: ...
    return param.release();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateListParameter
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void ActionParameterFactory::CreateListParameter( xml::xistream& xis, actions::parameters::ParameterList& list ) const
{
    const std::string name = xml::attribute< std::string >( xis, "name" );
    const std::string type = boost::algorithm::to_lower_copy( xml::attribute< std::string >( xis, "type" ) );
    list.AddParameter( *CreateParameter( OrderParameter( name, type, false), xis ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateListParameter
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void ActionParameterFactory::CreateListParameter( xml::xistream& xis, actions::parameters::ParameterList& list, const kernel::Entity_ABC& entity ) const
{
    const std::string name = xml::attribute< std::string >( xis, "name" );
    const std::string type = boost::algorithm::to_lower_copy( xml::attribute< std::string >( xis, "type" ) );
    list.AddParameter( *CreateParameter( OrderParameter( name, type, false), xis, entity ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::DoCreateParameter
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
bool ActionParameterFactory::DoCreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis, const std::string& type, std::auto_ptr< actions::Parameter_ABC >& param ) const
{
    if( type == "bool" )
        param.reset( new actions::parameters::Bool( parameter, xis ) );
    else if( type == "numeric" )
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
    else if( type == "pathlist" )
        param.reset( new actions::parameters::PathList( parameter, converter_, xis ) );
    else if( type == "pointlist" )
        param.reset( new actions::parameters::PointList( parameter, converter_, xis ) );
    else if( type == "polygonlist" )
        param.reset( new actions::parameters::PolygonList( parameter, converter_, xis ) );
    else if( type == "locationlist" )
        param.reset( new actions::parameters::LocationList( parameter, converter_, xis ) );
    else if( type == "direction" )
        param.reset( new actions::parameters::Direction( parameter, xis ) );
    else if( type == "phaselinelist" )
        param.reset( new actions::parameters::LimaList( parameter, converter_, xis ) );
    else if( type == "intelligencelist" )
        param.reset( new actions::parameters::IntelligenceList( parameter, converter_, xis, model_.teams_, staticModel_.levels_, controller_ ) );
    else if( type == "limit" )
        param.reset( new actions::parameters::Limit( parameter, converter_, xis ) );
    else if( type == "enumeration" )
        param.reset( new actions::parameters::Enumeration( parameter, xis ) );
    else if( type == "agent" )
        param.reset( new actions::parameters::Agent( parameter, xis, model_.agents_, controller_ ) );
    else if( type == "automate" )
        param.reset( new actions::parameters::Automat( parameter, xis, model_.agents_, controller_ ) );
    else if( type == "agentlist" )
        param.reset( new actions::parameters::AgentList( parameter, xis, model_.agents_, controller_ ) );
    else if( type == "automatelist" )
        param.reset( new actions::parameters::AutomatList( parameter, xis, model_.agents_, controller_ ) );
    else if( type == "army" )
        param.reset( new actions::parameters::Army( parameter, xis, model_.teams_, controller_ ) );
    else if( type == "formation" )
        param.reset( new actions::parameters::Formation( parameter, xis, model_.teams_, controller_ ) );
    else if( type == "dotationtype" )
        param.reset( new actions::parameters::DotationType( parameter, xis, staticModel_.objectTypes_ ) );
    else if( type == "genobject" )
        param.reset( new actions::parameters::EngineerConstruction( parameter, converter_, staticModel_.objectTypes_, model_.agents_, xis, controller_ ) );
    else if( type == "genobjectlist" )
        param.reset( new actions::parameters::EngineerConstructionList( parameter, converter_, staticModel_.objectTypes_, model_.agents_, xis, controller_ ) );
    else if( type == "natureatlas" )
        param.reset( new actions::parameters::AtlasNature( parameter, xis, staticModel_.atlasNatures_ ) );
    else if( type == "objective" )
        param.reset( new actions::parameters::Objective( parameter, xis, converter_ ) );
    else if( type == "objectivelist" )
        param.reset( new actions::parameters::ObjectiveList( parameter, xis, converter_ ) );
    else if( type == "medicalpriorities" )
        param.reset( new actions::parameters::MedicalPriorities( parameter, xis ) );
    else if( type == "maintenancepriorities" )
        param.reset( new actions::parameters::MaintenancePriorities( parameter, staticModel_.objectTypes_, xis ) );
    else if( type == "datetime" )
        param.reset( new actions::parameters::DateTime( parameter, xis ) );
    else if( type == "urbanblockbm" )
        param.reset( new actions::parameters::UrbanBlock( parameter, xis ) );
    else if( type == "quantity" )
        param.reset( new actions::parameters::Quantity( parameter, xis ) );
    else if( type == "identifier" )
        param.reset( new actions::parameters::Identifier( parameter, xis ) );
    else if( type == "knowledgegroup" )
        param.reset( new actions::parameters::KnowledgeGroup( parameter, xis, model_.knowledgeGroups_, controller_ ) );
    else if( type == "list" )
    {
        actions::parameters::ParameterList* parameterList = new actions::parameters::ParameterList( parameter );
        param.reset( parameterList );
        xis >> xml::list( "parameter", *this, &ActionParameterFactory::CreateListParameter, *parameterList );
    }
    else
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::DoCreateParameter
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
bool ActionParameterFactory::DoCreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Entity_ABC& entity, const std::string& type, std::auto_ptr< actions::Parameter_ABC >& param ) const
{
    if( type == "agentknowledge" )
        param.reset( new actions::parameters::AgentKnowledge( parameter, xis, model_.agents_, agentKnowledgeConverter_, entity, controller_ ) );
    else if( type == "populationknowledge" )
        param.reset( new actions::parameters::PopulationKnowledge( parameter, xis, model_.agents_, agentKnowledgeConverter_, entity, controller_ ) );
    else if( type == "objectknowledge" )
        param.reset( new actions::parameters::ObjectKnowledge( parameter, xis, model_.objects_, objectKnowledgeConverter_, entity, controller_ ) );
    else if( type == "agentknowledgelist" )
        param.reset( new actions::parameters::AgentKnowledgeList( parameter, xis, model_.agents_, agentKnowledgeConverter_, entity, controller_ ) );
    else if( type == "objectknowledgelist" )
        param.reset( new actions::parameters::ObjectKnowledgeList( parameter, xis, model_.objects_, objectKnowledgeConverter_, entity, controller_ ) );
    else if( type == "list" )
    {
        actions::parameters::ParameterList* parameterList = new actions::parameters::ParameterList( parameter );
        param.reset( parameterList );
        xis >> xml::list( "parameter", *this, &ActionParameterFactory::CreateListParameter, *parameterList, entity );
    }
    else
        return false;

    return true;
}
