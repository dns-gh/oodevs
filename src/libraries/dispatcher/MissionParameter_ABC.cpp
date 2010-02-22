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
#include "ClientPublisher_ABC.h"

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
#include "MissionParameter_UrbanBlock.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_ABC::Create
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
// static
MissionParameter_ABC* MissionParameter_ABC::Create( const Common::MsgMissionParameter& asn )
{
    if( asn.null_value() )
        return 0;
    if( !asn.has_value() )
        throw std::runtime_error( __FUNCTION__ " Invalid mission parameter type" );
    if( asn.value().has_abool() )
        return new MissionParameter_Bool( asn );
    if( asn.value().has_areal() )
        return new MissionParameter_Numeric( asn );
    if( asn.value().has_enumeration() )
        return new MissionParameter_Enumeration( asn );
    if( asn.value().has_path() )
        return new MissionParameter_Path( asn );
    if( asn.value().has_pathlist() )
        return new MissionParameter_PathList( asn );
    if( asn.value().has_point() )
        return new MissionParameter_Point( asn );
    if( asn.value().has_pointlist() )
        return new MissionParameter_PointList( asn );
    if( asn.value().has_polygon() )
        return new MissionParameter_Polygon( asn );
    if( asn.value().has_polygonlist() )
        return new MissionParameter_PolygonList( asn );
    if( asn.value().has_location() )
        return new MissionParameter_Location( asn );
    if( asn.value().has_locationlist() )
        return new MissionParameter_LocationList( asn );
    if( asn.value().has_heading() )
        return new MissionParameter_Direction( asn );
    if( asn.value().has_atlasnature() )
        return new MissionParameter_NatureAtlas( asn );
    if( asn.value().has_unit() )
        return new MissionParameter_Agent( asn );
    if( asn.value().has_unitlist() )
        return new MissionParameter_AgentList( asn );
    if( asn.value().has_automat() )
        return new MissionParameter_Automat( asn );
    if( asn.value().has_automatlist() )
        return new MissionParameter_AutomatList( asn );
    if( asn.value().has_unitknowledge() )
        return new MissionParameter_AgentKnowledge( asn );
    if( asn.value().has_unitknowledgelist() )
        return new MissionParameter_AgentKnowledgeList( asn );
    if( asn.value().has_objectknowledge() )
        return new MissionParameter_ObjectKnowledge( asn );
    if( asn.value().has_objectknowledgelist() )
        return new MissionParameter_ObjectKnowledgeList( asn );
    if( asn.value().has_populationknowledge() )
        return new MissionParameter_PopulationKnowledge( asn );
    if( asn.value().has_plannedwork() )
        return new MissionParameter_GenObject( asn );
    if( asn.value().has_plannedworklist() )
        return new MissionParameter_GenObjectList( asn );
    if( asn.value().has_dotationtype() )
        return new MissionParameter_DotationType( asn );
    if( asn.value().has_equipmenttype() )
        return new MissionParameter_EquipmentType( asn );
    if( asn.value().has_tirindirect() )
        return new MissionParameter_IndirectFire( asn );
    if( asn.value().has_datetime() )
        return new MissionParameter_GDH ( asn );
    if( asn.value().has_logmaintenancepriorities() )
        return new MissionParameter_MaintenancePriorities ( asn );
    if( asn.value().has_logmedicalpriorities() )
        return new MissionParameter_MedicalPriorities( asn );
    if( asn.value().has_acharstr() )
        return new MissionParameter_String( asn );
    if( asn.value().has_missionobjective() )
        return new MissionParameter_Objective( asn );
    if( asn.value().has_missionobjectivelist() )
        return new MissionParameter_ObjectiveList( asn );
    if( asn.value().has_line() )
        return new MissionParameter_Line( asn );
    if( asn.value().has_limasorder() )
        return new MissionParameter_LimasOrder( asn );
    if( asn.value().has_intelligencelist() )
        return new MissionParameter_IntelligenceList( asn );
    if( asn.value().has_urbanblock() )
        return new MissionParameter_UrbanBlock( asn );
    throw std::runtime_error( __FUNCTION__ " Invalid mission parameter type" );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ABC constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ABC::MissionParameter_ABC( const Common::MsgMissionParameter& asn )
    : bNullValue_( asn.null_value() )
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
