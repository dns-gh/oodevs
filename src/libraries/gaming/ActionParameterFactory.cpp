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
#include "ActionParameterDirection.h"
#include "ActionParameterLocation.h"
#include "Model.h"
#include "StaticModel.h"
#include "AgentsModel.h"
#include "clients_kernel/ObjectTypes.h"
#include "Tools.h"

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
ActionParameter_ABC* ActionParameterFactory::CreateParameter( const QString& name, const ASN1T_MissionParameter& parameter ) const
{
    if( parameter.null_value )
        return new ActionParameter< QString >( name, tools::translate( "ActionParameterFactory", "not set" ) );
    switch( parameter.value.t )
    {
    case T_MissionParameter_value_aBool:
        return new ActionParameter< bool >( name, parameter.value.u.aBool );
    case T_MissionParameter_value_aCharStr:
        return new ActionParameter< QString >( name, parameter.value.u.aCharStr );
    case T_MissionParameter_value_agent:
        return new ActionParameter< const kernel::Agent_ABC* >( name, &model_.agents_.GetAgent( parameter.value.u.agent ) );
    case T_MissionParameter_value_aReal:
        return new ActionParameter< float >( name, parameter.value.u.aReal );
    case T_MissionParameter_value_automate:
        return new ActionParameter< const kernel::Automat_ABC* >( name, &model_.agents_.GetAutomat( parameter.value.u.automate ) );
    case T_MissionParameter_value_direction:
        return new ActionParameter< float >( name, parameter.value.u.direction );
    case T_MissionParameter_value_enumeration:
    case T_MissionParameter_value_gDH:
        break;
    case T_MissionParameter_value_itineraire:
        return new ActionParameterLocation( name, converter_, *parameter.value.u.itineraire );
    case T_MissionParameter_value_knowledgeAgent:
    case T_MissionParameter_value_knowledgeObject:
    case T_MissionParameter_value_knowledgePopulation:
    case T_MissionParameter_value_listAgent:
    case T_MissionParameter_value_listAutomate:
    case T_MissionParameter_value_listItineraire:
    case T_MissionParameter_value_listKnowledgeAgent:
    case T_MissionParameter_value_listKnowledgeObject:
    case T_MissionParameter_value_listLocalisation:
        break;
    case T_MissionParameter_value_listMissionGenObject:
        return new ActionParameterObstacleList( name, converter_, staticModel_.objectTypes_, *parameter.value.u.listMissionGenObject );
    case T_MissionParameter_value_listPoint:
    case T_MissionParameter_value_listPolygon:
        break;
    case T_MissionParameter_value_localisation:
        return new ActionParameterLocation( name, converter_, *parameter.value.u.localisation );
    case T_MissionParameter_value_maintenancePriorites:
        break;
    case T_MissionParameter_value_missionGenObject:
        return new ActionParameterObstacle( name, converter_, staticModel_.objectTypes_, *parameter.value.u.missionGenObject );
    case T_MissionParameter_value_natureAtlas:
        break;
    case T_MissionParameter_value_point:
        return new ActionParameterLocation( name, converter_, *parameter.value.u.point );
    case T_MissionParameter_value_polygon:
        return new ActionParameterLocation( name, converter_, *parameter.value.u.polygon );
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
ActionParameter_ABC* ActionParameterFactory::CreateParameter( const QString& name, const ASN1T_Line& parameter ) const
{
    return new ActionParameterLimit( name, converter_, parameter );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameter_ABC* ActionParameterFactory::CreateParameter( const QString& name, const ASN1T_LimasOrder& parameter ) const
{
    return new ActionParameterLimaList( name, converter_, parameter );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory::CreateParameter
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameter_ABC* ActionParameterFactory::CreateParameter( const QString& name, const ASN1T_Direction& parameter ) const
{
    return new ActionParameterDirection( name, parameter );
}
