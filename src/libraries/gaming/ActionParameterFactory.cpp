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
#include "actions/Numeric.h"
#include "actions/DotationType.h"
#include "actions/AtlasNature.h"
#include "actions/MaintenancePriorities.h"
#include "actions/MedicalPriorities.h"
#include "actions/IntelligenceList.h"
#include "actions/DateTime.h"
#include "actions/UrbanBlock.h"
#include "Model.h"
#include "StaticModel.h"
#include "AgentsModel.h"
#include "TeamsModel.h"
#include "ObjectsModel.h"
#include "UrbanModel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/ObjectTypes.h"
#include "Tools.h"
#include <urban/model.h>
#include <urban/blockmodel.h>
#include <xeumeuleu/xml.h>
#pragma warning( push, 1 )
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

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
actions::Parameter_ABC* ActionParameterFactory::CreateParameter( const OrderParameter& parameter, const ASN1T_MissionParameter& asn, const Entity_ABC& entity ) const
{
    // $$$$ SBO 2007-10-11: we should create a parameter of the real type in order to be able (later) to edit parameters
    if( asn.null_value )
        return new actions::parameters::Parameter< QString >( parameter, tools::translate( "ActionParameterFactory", "not set" ) );
    switch( asn.value.t )
    {
    case T_MissionParameter_value_aBool:
        return new actions::parameters::Bool( parameter, asn.value.u.aBool != 0 );
    case T_MissionParameter_value_aCharStr:
        return new actions::parameters::Parameter< QString >( parameter, asn.value.u.aCharStr );
    case T_MissionParameter_value_unit:
        return new actions::parameters::Agent( parameter, asn.value.u.unit, model_.agents_, controller_ );
    case T_MissionParameter_value_aReal:
        return new actions::parameters::Numeric( parameter, asn.value.u.aReal );
    case T_MissionParameter_value_automat:
        return new actions::parameters::Automat( parameter, asn.value.u.automat, model_.agents_, controller_ );
    case T_MissionParameter_value_heading:
        return new actions::parameters::Parameter< float >( parameter, asn.value.u.heading );
    case T_MissionParameter_value_enumeration:
        return new actions::parameters::Enumeration( parameter, asn.value.u.enumeration );
    case T_MissionParameter_value_path:
        return new actions::parameters::Path( parameter, converter_, *asn.value.u.path );
    case T_MissionParameter_value_unitKnowledge:
        return new actions::parameters::AgentKnowledge( parameter, asn.value.u.unitKnowledge, agentKnowledgeConverter_, entity, controller_ );
    case T_MissionParameter_value_objectKnowledge:
        return new actions::parameters::ObjectKnowledge( parameter, asn.value.u.objectKnowledge, objectKnowledgeConverter_, entity, controller_ );
    case T_MissionParameter_value_populationKnowledge:
        return new actions::parameters::PopulationKnowledge( parameter, asn.value.u.populationKnowledge, agentKnowledgeConverter_, entity, controller_ );
    case T_MissionParameter_value_unitList:
        return new actions::parameters::AgentList( parameter, *asn.value.u.unitList, model_.agents_, controller_ );
    case T_MissionParameter_value_automatList:
        return new actions::parameters::AutomatList( parameter, *asn.value.u.automatList, model_.agents_, controller_ );
    case T_MissionParameter_value_pathList:
        return new actions::parameters::PathList( parameter, converter_, *asn.value.u.pathList );
    case T_MissionParameter_value_unitKnowledgeList:
        return new actions::parameters::AgentKnowledgeList( parameter, *asn.value.u.unitKnowledgeList, agentKnowledgeConverter_, entity, controller_ );
    case T_MissionParameter_value_objectKnowledgeList:
        return new actions::parameters::ObjectKnowledgeList( parameter, *asn.value.u.objectKnowledgeList, objectKnowledgeConverter_, entity, controller_ );
    case T_MissionParameter_value_locationList:
        return new actions::parameters::LocationList( parameter, converter_, *asn.value.u.locationList );
    case T_MissionParameter_value_plannedWorkList:
        return new actions::parameters::EngineerConstructionList( parameter, converter_, staticModel_.objectTypes_, model_.agents_, *asn.value.u.plannedWorkList, controller_ );
    case T_MissionParameter_value_pointList:
        return new actions::parameters::PointList( parameter, converter_, *asn.value.u.pointList );
    case T_MissionParameter_value_polygonList:
        return new actions::parameters::PolygonList( parameter, converter_, *asn.value.u.polygonList );
    case T_MissionParameter_value_location:
        return new actions::parameters::Location( parameter, converter_, *asn.value.u.location );
    case T_MissionParameter_value_plannedWork:
        return new actions::parameters::EngineerConstruction( parameter, converter_, staticModel_.objectTypes_, model_.agents_, *asn.value.u.plannedWork, controller_ );
    case T_MissionParameter_value_atlasNature:
        return new actions::parameters::AtlasNature( parameter, *asn.value.u.atlasNature, staticModel_.atlasNatures_ );
    case T_MissionParameter_value_missionObjective:
        return new actions::parameters::Objective( parameter, converter_, *asn.value.u.missionObjective );
    case T_MissionParameter_value_missionObjectiveList:
        return new actions::parameters::ObjectiveList( parameter, converter_, *asn.value.u.missionObjectiveList );
    case T_MissionParameter_value_point:
        return new actions::parameters::Point( parameter, converter_, *asn.value.u.point );
    case T_MissionParameter_value_polygon:
        return new actions::parameters::Polygon( parameter, converter_, *asn.value.u.polygon );
    case T_MissionParameter_value_dotationType:
        return new actions::parameters::DotationType( parameter, asn.value.u.dotationType, staticModel_.objectTypes_ );
    case T_MissionParameter_value_equipmentType:
        break;
    case T_MissionParameter_value_logMaintenancePriorities:
        return new actions::parameters::MaintenancePriorities( parameter, staticModel_.objectTypes_, *asn.value.u.logMaintenancePriorities );
    case T_MissionParameter_value_logMedicalPriorities:
        return new actions::parameters::MedicalPriorities( parameter, *asn.value.u.logMedicalPriorities );
    case T_MissionParameter_value_tirIndirect: // $$$$ SBO 2007-05-21: reports only, not to be used!
        break;
    case T_MissionParameter_value_line:
        return new actions::parameters::Limit( parameter, converter_, *asn.value.u.line );
    case T_MissionParameter_value_limasOrder:
        return new actions::parameters::LimaList( parameter, converter_, *asn.value.u.limasOrder );
    case T_MissionParameter_value_intelligenceList:
        return new actions::parameters::IntelligenceList( parameter, converter_, *asn.value.u.intelligenceList, model_.teams_, staticModel_.levels_, controller_ );
    case T_MissionParameter_value_dateTime:
        return new actions::parameters::DateTime( parameter, *asn.value.u.dateTime );
    case T_MissionParameter_value_urbanBlock:
        return new actions::parameters::UrbanBlock( parameter, asn.value.u.urbanBlock, model_.urbanObjects_.model_.blocks_ );
    }
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
    if( type == "bool" )
        param.reset( new actions::parameters::Bool( parameter, xis ) );
    else if( type == "numeric" )
        param.reset( new actions::parameters::Numeric( parameter, xis ) );
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
    else if( type == "dotationtype" )
        param.reset( new actions::parameters::DotationType( parameter, xis, staticModel_.objectTypes_ ) );
    else if( type == "genobject" )
        param.reset( new actions::parameters::EngineerConstruction( parameter, converter_, staticModel_.objectTypes_, model_.agents_, xis, controller_ ) );
    else if( type == "genobjectlist" )
        param.reset( new actions::parameters::EngineerConstructionList( parameter, converter_, staticModel_.objectTypes_, model_.agents_, xis, controller_ ) );
    else if( type == "agentknowledge" )
        param.reset( new actions::parameters::AgentKnowledge( parameter, xis, model_.agents_, agentKnowledgeConverter_, entity, controller_ ) );
    else if( type == "populationknowledge" )
        param.reset( new actions::parameters::PopulationKnowledge( parameter, xis, model_.agents_, agentKnowledgeConverter_, entity, controller_ ) );
    else if( type == "objectknowledge" )
        param.reset( new actions::parameters::ObjectKnowledge( parameter, xis, model_.objects_, objectKnowledgeConverter_, entity, controller_ ) );
    else if( type == "agentknowledgelist" )
        param.reset( new actions::parameters::AgentKnowledgeList( parameter, xis, model_.agents_, agentKnowledgeConverter_, entity, controller_ ) );
    else if( type == "objectknowledgelist" )
        param.reset( new actions::parameters::ObjectKnowledgeList( parameter, xis, model_.objects_, objectKnowledgeConverter_, entity, controller_ ) );
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
        param.reset( new actions::parameters::UrbanBlock( parameter, xis, model_.urbanObjects_.model_.blocks_ ) );
    else
        throw std::runtime_error( "Unknown parameter type '" + type + "'" );
    param->Set( true ); // $$$$ SBO 2007-10-11: ...
    return param.release();
}
