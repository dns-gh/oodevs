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

using namespace dispatcher;

// =============================================================================
// STATICS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_ABC::Create
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
// static
MissionParameter_ABC* MissionParameter_ABC::Create( const ASN1T_MissionParameter& asn )
{
    switch( asn.value.t )
    {
        case T_MissionParameter_value_aBool                 : return new MissionParameter_Bool                  ( asn );
        case T_MissionParameter_value_aReal                 : return new MissionParameter_Numeric               ( asn );
        case T_MissionParameter_value_enumeration           : return new MissionParameter_Enumeration           ( asn );
        case T_MissionParameter_value_itineraire            : return new MissionParameter_Path                  ( asn );
        case T_MissionParameter_value_listItineraire        : return new MissionParameter_PathList              ( asn );
        case T_MissionParameter_value_point                 : return new MissionParameter_Point                 ( asn );
        case T_MissionParameter_value_listPoint             : return new MissionParameter_PointList             ( asn );
        case T_MissionParameter_value_polygon               : return new MissionParameter_Polygon               ( asn );
        case T_MissionParameter_value_listPolygon           : return new MissionParameter_PolygonList           ( asn );
        case T_MissionParameter_value_localisation          : return new MissionParameter_Location              ( asn );
        case T_MissionParameter_value_listLocalisation      : return new MissionParameter_LocationList          ( asn );
        case T_MissionParameter_value_direction             : return new MissionParameter_Direction             ( asn );
        case T_MissionParameter_value_natureAtlas           : return new MissionParameter_NatureAtlas           ( asn );
        case T_MissionParameter_value_agent                 : return new MissionParameter_Agent                 ( asn );
        case T_MissionParameter_value_listAgent             : return new MissionParameter_AgentList             ( asn );
        case T_MissionParameter_value_automate              : return new MissionParameter_Automat               ( asn );
        case T_MissionParameter_value_listAutomate          : return new MissionParameter_AutomatList           ( asn );
        case T_MissionParameter_value_knowledgeAgent        : return new MissionParameter_AgentKnowledge        ( asn );
        case T_MissionParameter_value_listKnowledgeAgent    : return new MissionParameter_AgentKnowledgeList    ( asn );
        case T_MissionParameter_value_knowledgeObject       : return new MissionParameter_ObjectKnowledge       ( asn );
        case T_MissionParameter_value_listKnowledgeObject   : return new MissionParameter_ObjectKnowledgeList   ( asn );
        case T_MissionParameter_value_knowledgePopulation   : return new MissionParameter_PopulationKnowledge   ( asn );
        case T_MissionParameter_value_missionGenObject      : return new MissionParameter_GenObject             ( asn );
        case T_MissionParameter_value_listMissionGenObject  : return new MissionParameter_GenObjectList         ( asn );
        case T_MissionParameter_value_typeDotation          : return new MissionParameter_DotationType          ( asn );
        case T_MissionParameter_value_typeEquipement        : return new MissionParameter_EquipmentType         ( asn );
        case T_MissionParameter_value_tirIndirect           : return new MissionParameter_IndirectFire          ( asn );
        case T_MissionParameter_value_gDH                   : return new MissionParameter_GDH                   ( asn );
        case T_MissionParameter_value_maintenancePriorites  : return new MissionParameter_MaintenancePriorities ( asn );
        case T_MissionParameter_value_santePriorites        : return new MissionParameter_MedicalPriorities     ( asn );
        case T_MissionParameter_value_aCharStr              : return new MissionParameter_String                ( asn );
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
