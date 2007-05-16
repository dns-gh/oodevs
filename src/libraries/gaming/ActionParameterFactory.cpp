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
#include "ActionParameterLimits.h"
#include "ActionParameterLimaList.h"
#include "ActionParameterObstacle.h"
#include "ActionParameterObstacleList.h"
#include "ActionParameterDirection.h"
#include "ActionParameterLocation.h"
#include "ActionParameterLocationList.h"
#include "ActionParameterPath.h"
#include "ActionParameterEntityList.h"
#include "Model.h"
#include "StaticModel.h"
#include "AgentsModel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/ObjectTypes.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterFactory::ActionParameterFactory( const kernel::CoordinateConverter_ABC& converter, const Model& model, const StaticModel& staticModel )
    : converter_( converter )
    , model_( model )
    , staticModel_( staticModel )
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
ActionParameter_ABC* ActionParameterFactory::CreateParameter( const kernel::OrderParameter& parameter, const ASN1T_MissionParameter& asn, const kernel::Entity_ABC& entity ) const
{
    if( asn.null_value )
        return new ActionParameter< QString >( parameter, tools::translate( "ActionParameterFactory", "not set" ) );
    switch( asn.value.t )
    {
    case T_MissionParameter_value_aBool:
        return new ActionParameter< bool >( parameter, asn.value.u.aBool );
    case T_MissionParameter_value_aCharStr:
        return new ActionParameter< QString >( parameter, asn.value.u.aCharStr );
    case T_MissionParameter_value_agent:
        return new ActionParameter< const kernel::Agent_ABC* >( parameter, &model_.agents_.GetAgent( asn.value.u.agent ) );
    case T_MissionParameter_value_aReal:
        return new ActionParameter< float >( parameter, asn.value.u.aReal );
    case T_MissionParameter_value_automate:
        return new ActionParameter< const kernel::Automat_ABC* >( parameter, &model_.agents_.GetAutomat( asn.value.u.automate ) );
    case T_MissionParameter_value_direction:
        return new ActionParameter< float >( parameter, asn.value.u.direction );
    case T_MissionParameter_value_enumeration:
    case T_MissionParameter_value_gDH:
        break;
    case T_MissionParameter_value_itineraire:
        return new ActionParameterPath( parameter, converter_, *asn.value.u.itineraire, entity );
    case T_MissionParameter_value_knowledgeAgent:
    case T_MissionParameter_value_knowledgeObject:
    case T_MissionParameter_value_knowledgePopulation:
        break;
    case T_MissionParameter_value_listAgent:
        return new ActionParameterEntityList( parameter, *asn.value.u.listAgent, model_.agents_ );
    case T_MissionParameter_value_listAutomate:
        return new ActionParameterEntityList( parameter, *asn.value.u.listAutomate, model_.agents_ );
    case T_MissionParameter_value_listItineraire:
        return new ActionParameterLocationList( parameter, converter_, (ASN1T_ListLocalisation&)*asn.value.u.listItineraire, entity );
    case T_MissionParameter_value_listKnowledgeAgent:
    case T_MissionParameter_value_listKnowledgeObject:
        break;
    case T_MissionParameter_value_listLocalisation:
        return new ActionParameterLocationList( parameter, converter_, (ASN1T_ListLocalisation&)*asn.value.u.listLocalisation );
    case T_MissionParameter_value_listMissionGenObject:
        return new ActionParameterObstacleList( parameter, converter_, staticModel_.objectTypes_, *asn.value.u.listMissionGenObject );
    case T_MissionParameter_value_listPoint:
        return new ActionParameterLocationList( parameter, converter_, (ASN1T_ListLocalisation&)*asn.value.u.listPoint );
    case T_MissionParameter_value_listPolygon:
        return new ActionParameterLocationList( parameter, converter_, (ASN1T_ListLocalisation&)*asn.value.u.listPolygon );
    case T_MissionParameter_value_localisation:
        return new ActionParameterLocation( parameter, converter_, *asn.value.u.localisation );
    case T_MissionParameter_value_maintenancePriorites:
        break;
    case T_MissionParameter_value_missionGenObject:
        return new ActionParameterObstacle( parameter, converter_, staticModel_.objectTypes_, *asn.value.u.missionGenObject );
    case T_MissionParameter_value_natureAtlas:
        break;
    case T_MissionParameter_value_point:
        return new ActionParameterLocation( parameter, converter_, *asn.value.u.point );
    case T_MissionParameter_value_polygon:
        return new ActionParameterLocation( parameter, converter_, *asn.value.u.polygon );
    case T_MissionParameter_value_santePriorites:
    case T_MissionParameter_value_tirIndirect:
    case T_MissionParameter_value_typeDotation:
    case T_MissionParameter_value_typeEquipement:
        break;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameter_ABC* ActionParameterFactory::CreateParameter( const kernel::OrderParameter& parameter, const ASN1T_Line& line1, const ASN1T_Line& line2 ) const
{
    return new ActionParameterLimits( parameter, converter_, line1, line2 );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameter_ABC* ActionParameterFactory::CreateParameter( const kernel::OrderParameter& parameter, const ASN1T_LimasOrder& asn ) const
{
    return new ActionParameterLimaList( parameter, converter_, asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameter_ABC* ActionParameterFactory::CreateParameter( const kernel::OrderParameter& parameter, const ASN1T_Direction& asn ) const
{
    return new ActionParameterDirection( parameter, asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameter_ABC* ActionParameterFactory::CreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis ) const
{
    std::string type;
    xis >> attribute( "type", type );
    type = QString( type.c_str() ).lower().ascii();
    if( type != parameter.GetType().lower().ascii() )
        return new ActionParameter< QString >( parameter, QString( "Invalid: '%1' expecting '%2'" ).arg( type.c_str() ).arg( parameter.GetType() ) );

    if( type == "path" )
        return new ActionParameterPath( parameter, converter_, xis );
    else if( type == "point" || type == "pointlist" || type == "polygon" || type == "location" )
        return new ActionParameterLocation( parameter, converter_, xis );
    else if( type == "direction" || type == "dangerousdirection" )
        return new ActionParameterDirection( parameter, xis );
    else if( type == "limalist" )
        return new ActionParameterLimaList( parameter, converter_, xis );
    else if( type == "limits" )
        return new ActionParameterLimits( parameter, converter_, xis );

    return new ActionParameter< QString >( parameter ); // $$$$ SBO 2007-05-16: default not yet implemented parameters...
}

//"agent"
//"agentlist"
//"automate"
//"automatelist"
//"genobject"
//"genobjectlist"
//"dotationtype"
//"natureatlas"
//
//"bool"
//"direction"
//"gdh"
//"numeric"
//
//"agentknowledge"
//"agentknowledgelist"
//"objectknowledge"
//"objectknowledgelist"
//"populationknowledge"
//
//"path"
//"pathlist"
//"point"
//"pointlist"
//"polygon"
//"polygonlist"
//"location"
//"locationlist"
//
//"maintenancepriorities"
//"medicalpriorities"
//"enumeration"
//
//"limits"
//"limalist"
//"dangerousdirection"
