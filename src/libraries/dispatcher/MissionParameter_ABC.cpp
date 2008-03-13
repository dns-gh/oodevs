// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_ABC.h"
#include "Network_Def.h"

#include "MissionParameter_Bool.h"
#include "MissionParameter_Numeric.h"
#include "MissionParameter_Enumeration.h"
#include "MissionParameter_Path.h"
#include "MissionParameter_PathList.h"
#include "MissionParameter_Point.h"
#include "MissionParameter_PointList.h"
#include "MissionParameter_Polygon.h"
#include "MissionParameter_PolygonList.h"
#include "MissionParameter_Location.h"
#include "MissionParameter_LocationList.h"
#include "MissionParameter_Direction.h"
#include "MissionParameter_NatureAtlas.h"
#include "MissionParameter_Agent.h"
#include "MissionParameter_AgentList.h"
#include "MissionParameter_Automat.h"
#include "MissionParameter_AutomatList.h"
#include "MissionParameter_AgentKnowledge.h"
#include "MissionParameter_AgentKnowledgeList.h"
#include "MissionParameter_ObjectKnowledge.h"
#include "MissionParameter_ObjectKnowledgeList.h"
#include "MissionParameter_PopulationKnowledge.h"
#include "MissionParameter_GenObject.h"
#include "MissionParameter_GenObjectList.h"
#include "MissionParameter_DotationType.h"
#include "MissionParameter_EquipmentType.h"
#include "MissionParameter_IndirectFire.h"
#include "MissionParameter_GDH.h"
#include "MissionParameter_MaintenancePriorities.h"
#include "MissionParameter_MedicalPriorities.h"
#include "MissionParameter_String.h"
#include "MissionParameter_Objective.h"
#include "MissionParameter_ObjectiveList.h"
#include "MissionParameter_Line.h"
#include "MissionParameter_LimasOrder.h"
#include "MissionParameter_IntelligenceList.h"

using namespace dispatcher;

// =============================================================================
// STATICS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_ABC::Create
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
// static
MissionParameter_ABC* MissionParameter_ABC::Create( Model& model, const ASN1T_MissionParameter& asn )
{
    switch( asn.value.t )
    {
        case T_MissionParameter_value_aBool                     : return new MissionParameter_Bool                  ( asn );
        case T_MissionParameter_value_aReal                     : return new MissionParameter_Numeric               ( asn );
        case T_MissionParameter_value_enumeration               : return new MissionParameter_Enumeration           ( asn );
        case T_MissionParameter_value_path                      : return new MissionParameter_Path                  ( asn );
        case T_MissionParameter_value_pathList                  : return new MissionParameter_PathList              ( asn );
        case T_MissionParameter_value_point                     : return new MissionParameter_Point                 ( asn );
        case T_MissionParameter_value_pointList                 : return new MissionParameter_PointList             ( asn );
        case T_MissionParameter_value_polygon                   : return new MissionParameter_Polygon               ( asn );
        case T_MissionParameter_value_polygonList               : return new MissionParameter_PolygonList           ( asn );
        case T_MissionParameter_value_location                  : return new MissionParameter_Location              ( asn );
        case T_MissionParameter_value_locationList              : return new MissionParameter_LocationList          ( asn );
        case T_MissionParameter_value_heading                   : return new MissionParameter_Direction             ( asn );
        case T_MissionParameter_value_atlasNature               : return new MissionParameter_NatureAtlas           ( asn );
        case T_MissionParameter_value_unit                      : return new MissionParameter_Agent                 ( asn );
        case T_MissionParameter_value_unitList                  : return new MissionParameter_AgentList             ( asn );
        case T_MissionParameter_value_automat                   : return new MissionParameter_Automat               ( asn );
        case T_MissionParameter_value_automatList               : return new MissionParameter_AutomatList           ( asn );
        case T_MissionParameter_value_unitKnowledge             : return new MissionParameter_AgentKnowledge        ( asn );
        case T_MissionParameter_value_unitKnowledgeList         : return new MissionParameter_AgentKnowledgeList    ( asn );
        case T_MissionParameter_value_objectKnowledge           : return new MissionParameter_ObjectKnowledge       ( asn );
        case T_MissionParameter_value_objectKnowledgeList       : return new MissionParameter_ObjectKnowledgeList   ( asn );
        case T_MissionParameter_value_populationKnowledge       : return new MissionParameter_PopulationKnowledge   ( asn );
        case T_MissionParameter_value_plannedWork               : return new MissionParameter_GenObject             ( asn );
        case T_MissionParameter_value_plannedWorkList           : return new MissionParameter_GenObjectList         ( asn );
        case T_MissionParameter_value_dotationType              : return new MissionParameter_DotationType          ( asn );
        case T_MissionParameter_value_equipmentType             : return new MissionParameter_EquipmentType         ( asn );
        case T_MissionParameter_value_tirIndirect               : return new MissionParameter_IndirectFire          ( asn );
        case T_MissionParameter_value_dateTime                  : return new MissionParameter_GDH                   ( asn );
        case T_MissionParameter_value_logMaintenancePriorities  : return new MissionParameter_MaintenancePriorities ( asn );
        case T_MissionParameter_value_logMedicalPriorities      : return new MissionParameter_MedicalPriorities     ( asn );
        case T_MissionParameter_value_aCharStr                  : return new MissionParameter_String                ( asn );
        case T_MissionParameter_value_missionObjective          : return new MissionParameter_Objective             ( asn );
        case T_MissionParameter_value_missionObjectiveList      : return new MissionParameter_ObjectiveList         ( asn );
        case T_MissionParameter_value_line                      : return new MissionParameter_Line                  ( asn );
        case T_MissionParameter_value_limasOrder                : return new MissionParameter_LimasOrder            ( model, asn );
        case T_MissionParameter_value_intelligenceList          : return new MissionParameter_IntelligenceList      ( model, asn );
        default:
            throw std::runtime_error( "Invalid mission parameter type" );
    }
}

// =============================================================================
// INSTANCES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_ABC constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ABC::MissionParameter_ABC( const ASN1T_MissionParameter& asn )
    : bNullValue_( asn.null_value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ABC destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ABC::~MissionParameter_ABC()
{
    // NOTHING
}
