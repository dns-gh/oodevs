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
#include "ActionParameterLimit.h"
#include "ActionParameterLimaList.h"
#include "ActionParameterObstacle.h"
#include "ActionParameterObstacleList.h"
#include "ActionParameterObjective.h"
#include "ActionParameterObjectiveList.h"
#include "ActionParameterDirection.h"
#include "ActionParameterLocation.h"
#include "ActionParameterPoint.h"
#include "ActionParameterPolygon.h"
#include "ActionParameterPointList.h"
#include "ActionParameterPolygonList.h"
#include "ActionParameterLocationList.h"
#include "ActionParameterPathList.h"
#include "ActionParameterPath.h"
#include "ActionParameterAgent.h"
#include "ActionParameterAutomat.h"
#include "ActionParameterAgentList.h"
#include "ActionParameterAutomatList.h"
#include "ActionParameterAgentKnowledge.h"
#include "ActionParameterPopulationKnowledge.h"
#include "ActionParameterObjectKnowledge.h"
#include "ActionParameterAgentKnowledgeList.h"
#include "ActionParameterObjectKnowledgeList.h"
#include "ActionParameterEnumeration.h"
#include "ActionParameterBool.h"
#include "ActionParameterNumeric.h"
#include "ActionParameterDotationType.h"
#include "ActionParameterAtlasNature.h"
#include "ActionParameterMaintenancePriorities.h"
#include "ActionParameterMedicalPriorities.h"
#include "ActionParameterIntelligenceList.h"
#include "Model.h"
#include "StaticModel.h"
#include "AgentsModel.h"
#include "TeamsModel.h"
#include "ObjectsModel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/ObjectTypes.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterFactory::ActionParameterFactory( const CoordinateConverter_ABC& converter, const Model& model, const StaticModel& staticModel
                                              , AgentKnowledgeConverter_ABC& agentKnowledgeConverter, ObjectKnowledgeConverter_ABC& objectKnowledgeConverter )
    : converter_( converter )
    , model_( model )
    , staticModel_( staticModel )
    , agentKnowledgeConverter_( agentKnowledgeConverter )
    , objectKnowledgeConverter_( objectKnowledgeConverter )
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
ActionParameter_ABC* ActionParameterFactory::CreateParameter( const OrderParameter& parameter, const ASN1T_MissionParameter& asn, const Entity_ABC& entity ) const
{
    // $$$$ SBO 2007-10-11: we should create a parameter of the real type in order to be able (later) to edit parameters
    if( asn.null_value )
        return new ActionParameter< QString >( parameter, tools::translate( "ActionParameterFactory", "not set" ) );
    switch( asn.value.t )
    {
    case T_MissionParameter_value_aBool:
        return new ActionParameterBool( parameter, asn.value.u.aBool != 0 );
    case T_MissionParameter_value_aCharStr:
        return new ActionParameter< QString >( parameter, asn.value.u.aCharStr );
    case T_MissionParameter_value_unit:
        return new ActionParameterAgent( parameter, asn.value.u.unit, model_.agents_ );
    case T_MissionParameter_value_aReal:
        return new ActionParameterNumeric( parameter, asn.value.u.aReal );
    case T_MissionParameter_value_automat:
        return new ActionParameterAutomat( parameter, asn.value.u.automat, model_.agents_ );
    case T_MissionParameter_value_heading:
        return new ActionParameter< float >( parameter, asn.value.u.heading );
    case T_MissionParameter_value_enumeration:
        return new ActionParameterEnumeration( parameter, asn.value.u.enumeration );
    case T_MissionParameter_value_path:
        return new ActionParameterPath( parameter, converter_, *asn.value.u.path );
    case T_MissionParameter_value_unitKnowledge:
        return new ActionParameterAgentKnowledge( parameter, asn.value.u.unitKnowledge, agentKnowledgeConverter_, entity );
    case T_MissionParameter_value_objectKnowledge:
        return new ActionParameterObjectKnowledge( parameter, asn.value.u.objectKnowledge, objectKnowledgeConverter_, entity );
    case T_MissionParameter_value_populationKnowledge:
        return new ActionParameterPopulationKnowledge( parameter, asn.value.u.populationKnowledge, agentKnowledgeConverter_, entity );
    case T_MissionParameter_value_unitList:
        return new ActionParameterAgentList( parameter, *asn.value.u.unitList, model_.agents_ );
    case T_MissionParameter_value_automatList:
        return new ActionParameterAutomatList( parameter, *asn.value.u.automatList, model_.agents_ );
    case T_MissionParameter_value_pathList:
        return new ActionParameterPathList( parameter, converter_, *asn.value.u.pathList );
    case T_MissionParameter_value_unitKnowledgeList:
        return new ActionParameterAgentKnowledgeList( parameter, *asn.value.u.unitKnowledgeList, agentKnowledgeConverter_, entity );
    case T_MissionParameter_value_objectKnowledgeList:
        return new ActionParameterObjectKnowledgeList( parameter, *asn.value.u.objectKnowledgeList, objectKnowledgeConverter_, entity );
    case T_MissionParameter_value_locationList:
        return new ActionParameterLocationList( parameter, converter_, *asn.value.u.locationList );
    case T_MissionParameter_value_plannedWorkList:
        return new ActionParameterObstacleList( parameter, converter_, staticModel_.objectTypes_, model_.agents_, *asn.value.u.plannedWorkList );
    case T_MissionParameter_value_pointList:
        return new ActionParameterPointList( parameter, converter_, *asn.value.u.pointList );
    case T_MissionParameter_value_polygonList:
        return new ActionParameterPolygonList( parameter, converter_, *asn.value.u.polygonList );
    case T_MissionParameter_value_location:
        return new ActionParameterLocation( parameter, converter_, *asn.value.u.location );
    case T_MissionParameter_value_plannedWork:
        return new ActionParameterObstacle( parameter, converter_, staticModel_.objectTypes_, model_.agents_, *asn.value.u.plannedWork );
    case T_MissionParameter_value_atlasNature:
        return new ActionParameterAtlasNature( parameter, *asn.value.u.atlasNature, staticModel_.atlasNatures_ );
    case T_MissionParameter_value_missionObjective:
        return new ActionParameterObjective( parameter, converter_, *asn.value.u.missionObjective );
    case T_MissionParameter_value_missionObjectiveList:
        return new ActionParameterObjectiveList( parameter, converter_, *asn.value.u.missionObjectiveList );
    case T_MissionParameter_value_point:
        return new ActionParameterPoint( parameter, converter_, *asn.value.u.point );
    case T_MissionParameter_value_polygon:
        return new ActionParameterPolygon( parameter, converter_, *asn.value.u.polygon );
    case T_MissionParameter_value_dotationType:
        return new ActionParameterDotationType( parameter, asn.value.u.dotationType, staticModel_.objectTypes_ );
    case T_MissionParameter_value_equipmentType:
        break;
    case T_MissionParameter_value_logMaintenancePriorities:
        return new ActionParameterMaintenancePriorities( parameter, staticModel_.objectTypes_, *asn.value.u.logMaintenancePriorities );
    case T_MissionParameter_value_logMedicalPriorities:
        return new ActionParameterMedicalPriorities( parameter, *asn.value.u.logMedicalPriorities );
    case T_MissionParameter_value_tirIndirect: // $$$$ SBO 2007-05-21: reports only, not to be used!
        break;
    case T_MissionParameter_value_line:
        return new ActionParameterLimit( parameter, converter_, *asn.value.u.line );
    case T_MissionParameter_value_limasOrder:
        return new ActionParameterLimaList( parameter, converter_, *asn.value.u.limasOrder );
    case T_MissionParameter_value_intelligenceList:
        return new ActionParameterIntelligenceList( parameter, converter_, *asn.value.u.intelligenceList, model_.teams_, staticModel_.levels_ );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameter_ABC* ActionParameterFactory::CreateParameter( const OrderParameter& parameter, xml::xistream& xis, const kernel::Entity_ABC& entity ) const
{
    std::string type = parameter.GetType();
    std::transform( type.begin(), type.end(), type.begin(), & tolower );

    std::auto_ptr< ActionParameter_ABC > param;
    if( type == "bool" )
        param.reset( new ActionParameterBool( parameter, xis ) );
    else if( type == "numeric" )
        param.reset( new ActionParameterNumeric( parameter, xis ) );
    else if( type == "path" )
        param.reset( new ActionParameterPath( parameter, converter_, xis ) );
    else if( type == "point" )
        param.reset( new ActionParameterPoint( parameter, converter_, xis ) );
    else if( type == "polygon" )
        param.reset( new ActionParameterPolygon( parameter, converter_, xis ) );
    else if( type == "location" )
        param.reset( new ActionParameterLocation( parameter, converter_, xis ) );
    else if( type == "pathlist" )
        param.reset( new ActionParameterPathList( parameter, converter_, xis ) );
    else if( type == "pointlist" )
        param.reset( new ActionParameterPointList( parameter, converter_, xis ) );
    else if( type == "polygonlist" )
        param.reset( new ActionParameterPolygonList( parameter, converter_, xis ) );
    else if( type == "locationlist" )
        param.reset( new ActionParameterLocationList( parameter, converter_, xis ) );
    else if( type == "direction" )
        param.reset( new ActionParameterDirection( parameter, xis ) );
    else if( type == "phaselinelist" )
        param.reset( new ActionParameterLimaList( parameter, converter_, xis ) );
    else if( type == "intelligencelist" )
        param.reset( new ActionParameterIntelligenceList( parameter, converter_, xis, model_.teams_, staticModel_.levels_ ) );
    else if( type == "limit" )
        param.reset( new ActionParameterLimit( parameter, converter_, xis ) );
    else if( type == "enumeration" )
        param.reset( new ActionParameterEnumeration( parameter, xis ) );
    else if( type == "agent" )
        param.reset( new ActionParameterAgent( parameter, xis, model_.agents_ ) );
    else if( type == "automate" )
        param.reset( new ActionParameterAutomat( parameter, xis, model_.agents_ ) );
    else if( type == "agentlist" )
        param.reset( new ActionParameterAgentList( parameter, xis, model_.agents_ ) );
    else if( type == "automatelist" )
        param.reset( new ActionParameterAutomatList( parameter, xis, model_.agents_ ) );
    else if( type == "dotationtype" )
        param.reset( new ActionParameterDotationType( parameter, xis, staticModel_.objectTypes_ ) );
    else if( type == "genobject" )
        param.reset( new ActionParameterObstacle( parameter, converter_, staticModel_.objectTypes_, model_.agents_, xis ) );
    else if( type == "genobjectlist" )
        param.reset( new ActionParameterObstacleList( parameter, converter_, staticModel_.objectTypes_, model_.agents_, xis ) );
    else if( type == "agentknowledge" )
        param.reset( new ActionParameterAgentKnowledge( parameter, xis, model_.agents_, agentKnowledgeConverter_, entity ) );
    else if( type == "populationknowledge" )
        param.reset( new ActionParameterPopulationKnowledge( parameter, xis, model_.agents_, agentKnowledgeConverter_, entity ) );
    else if( type == "objectknowledge" )
        param.reset( new ActionParameterObjectKnowledge( parameter, xis, model_.objects_, objectKnowledgeConverter_, entity ) );
    else if( type == "agentknowledgelist" )
        param.reset( new ActionParameterAgentKnowledgeList( parameter, xis, model_.agents_, agentKnowledgeConverter_, entity ) );
    else if( type == "objectknowledgelist" )
        param.reset( new ActionParameterObjectKnowledgeList( parameter, xis, model_.objects_, objectKnowledgeConverter_, entity ) );
    else if( type == "atlasnature" )
        param.reset( new ActionParameterAtlasNature( parameter, xis, staticModel_.atlasNatures_ ) );
    else if( type == "objective" )
        param.reset( new ActionParameterObjective( parameter, xis, converter_ ) );
    else if( type == "objectivelist" )
        param.reset( new ActionParameterObjectiveList( parameter, xis, converter_ ) );
    else if( type == "medicalpriorities" )
        param.reset( new ActionParameterMedicalPriorities( parameter, xis ) );
    else if( type == "maintenancepriorities" )
        param.reset( new ActionParameterMaintenancePriorities( parameter, staticModel_.objectTypes_, xis ) );
    else
        param.reset( new ActionParameter< QString >( parameter ) ); // $$$$ SBO 2007-05-16: default not yet implemented parameters...
    param->Set( true ); // $$$$ SBO 2007-10-11: ...
    return param.release();
}
