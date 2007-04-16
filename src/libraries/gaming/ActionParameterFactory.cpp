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
#include "StaticModel.h"
#include "clients_kernel/ObjectTypes.h"

// -----------------------------------------------------------------------------
// Name: ActionParameterFactory constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterFactory::ActionParameterFactory( const kernel::CoordinateConverter_ABC& converter, const StaticModel& staticModel )
    : converter_( converter )
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
//    if( parameter.null_value )
//        return 0;
    switch( parameter.value.t )
    {
    case T_MissionParameter_value_aBool:
    case T_MissionParameter_value_aCharStr:
    case T_MissionParameter_value_agent:
    case T_MissionParameter_value_aReal:
    case T_MissionParameter_value_automate:
    case T_MissionParameter_value_direction:
    case T_MissionParameter_value_enumeration:
    case T_MissionParameter_value_gDH:
    case T_MissionParameter_value_itineraire:
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
    case T_MissionParameter_value_localisation:
    case T_MissionParameter_value_maintenancePriorites:
        break;
    case T_MissionParameter_value_missionGenObject:
        return new ActionParameterObstacle( name, converter_, staticModel_.objectTypes_, *parameter.value.u.missionGenObject );
    case T_MissionParameter_value_natureAtlas:
    case T_MissionParameter_value_point:
    case T_MissionParameter_value_polygon:
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
