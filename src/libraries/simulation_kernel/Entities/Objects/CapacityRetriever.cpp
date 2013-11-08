// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "CapacityRetriever.h"
#include "MIL_ObjectType_ABC.h"
#include "ActivableCapacity.h"
#include "AltitudeModifierCapacity.h"
#include "AttitudeModifierCapacity.h"
#include "AttritionCapacity.h"
#include "AvoidanceCapacity.h"
#include "BorderCapacity.h"
#include "BridgingCapacity.h"
#include "BuildableCapacity.h"
#include "BypassableCapacity.h"
#include "ContaminationCapacity.h"
#include "DecontaminationCapacity.h"
#include "DelayCapacity.h"
#include "DetectionCapacity.h"
#include "FireForbiddenCapacity.h"
#include "FirePropagationModifierCapacity.h"
#include "FloodCapacity.h"
#include "TerrainHeuristicCapacity.h"
#include "ImprovableCapacity.h"
#include "InfrastructureCapacity.h"
#include "InteractWithSideCapacity.h"
#include "InteractIfHeightCapacity.h"
#include "InterferenceCapacity.h"
#include "IntoxicationCapacity.h"
#include "LodgingCapacity.h"
#include "LogisticCapacity.h"
#include "MedicalCapacity.h"
#include "MedicalCapacity.h"
#include "MobilityCapacity.h"
#include "PerceptionCapacity.h"
#include "PopulationFilterCapacity.h"
#include "ProtectionCapacity.h"
#include "ResourceNetworkCapacity.h"
#include "ScatteringCapacity.h"
#include "SpawnCapacity.h"
#include "StockCapacity.h"
#include "StructuralCapacity.h"
#include "SupplyCapacity.h"
#include "InteractIfEquippedCapacity.h"
#include "TimeLimitedCapacity.h"
#include "UndergroundCapacity.h"
#include "UniversalCapacity.h"
#include "WorkableCapacity.h"
#include "UrbanDestructionCapacity.h"
#include "TrafficabilityCapacity.h"
#include "BurnCapacity.h"
#include "PropagationCapacity_ABC.h"
#include "BurnSurfaceCapacity.h"
#include "CloudPropagationCapacity.h"

#define RETRIEVE( string, Capacity ) \
    { if( strCapacity == string ) return type.GetCapacity< Capacity >(); }

// -----------------------------------------------------------------------------
// Name: CapacityRetriever::RetrieveCapacity
// Created: JSR 2012-04-17
// -----------------------------------------------------------------------------
const ObjectCapacity_ABC* CapacityRetriever::RetrieveCapacity( const MIL_ObjectType_ABC& type,  const std::string& strCapacity )
{
    RETRIEVE( "activable", ActivableCapacity );
    RETRIEVE( "altitude-modifier", AltitudeModifierCapacity );
    RETRIEVE( "attitude-modifier", AttitudeModifierCapacity );
    RETRIEVE( "attrition", AttritionCapacity );
    RETRIEVE( "avoidable", AvoidanceCapacity );
    RETRIEVE( "border", BorderCapacity );
    RETRIEVE( "bridging", BridgingCapacity );
    RETRIEVE( "burn", BurnCapacity );
    RETRIEVE( "bypassable", BypassableCapacity );
    RETRIEVE( "contamination", ContaminationCapacity );
    RETRIEVE( "decontamination", DecontaminationCapacity );
    RETRIEVE( "delay", DelayCapacity );
    RETRIEVE( "detection", DetectionCapacity );
    RETRIEVE( "fire-forbidden", FireForbiddenCapacity );
    RETRIEVE( "fire-propagation-modifier", FirePropagationModifierCapacity );
    RETRIEVE( "flood", FloodCapacity );
    RETRIEVE( "heuristic", TerrainHeuristicCapacity );
    RETRIEVE( "infrastructure", InfrastructureCapacity );
    RETRIEVE( "interact-with-enemy", InteractWithSideCapacity );
    RETRIEVE( "interaction-height", InteractIfHeightCapacity );
    RETRIEVE( "interference", InterferenceCapacity );
    RETRIEVE( "intoxication", IntoxicationCapacity );
    RETRIEVE( "lodging", LodgingCapacity );
    RETRIEVE( "logistic", LogisticCapacity );
    RETRIEVE( "mobility", MobilityCapacity );
    RETRIEVE( "perception", PerceptionCapacity );
    RETRIEVE( "population-filter", PopulationFilterCapacity );
    RETRIEVE( "protection", ProtectionCapacity );
    RETRIEVE( "resources", ResourceNetworkCapacity );
    RETRIEVE( "scattering", ScatteringCapacity );
    RETRIEVE( "spawn", SpawnCapacity );
    RETRIEVE( "stock", StockCapacity );
    RETRIEVE( "structural", StructuralCapacity );
    RETRIEVE( "supply", SupplyCapacity );
    RETRIEVE( "supply-route", InteractIfEquippedCapacity );
    RETRIEVE( "time-limited", TimeLimitedCapacity );
    RETRIEVE( "underground-network", UndergroundCapacity );
    RETRIEVE( "universal", UniversalCapacity );
    RETRIEVE( "workable", WorkableCapacity );
    RETRIEVE( "urban-destruction", UrbanDestructionCapacity );
    RETRIEVE( "trafficability", TrafficabilityCapacity );

    if( strCapacity == "constructor" )
    {
        const ObjectCapacity_ABC* buildable = type.GetCapacity< BuildableCapacity >();
        return ( buildable ? buildable : type.GetCapacity< ImprovableCapacity >() );
    }
    if( strCapacity == "propagation" )
    {
        const ObjectCapacity_ABC* prop = type.GetCapacity< PropagationCapacity_ABC >();
        if( prop )
            return prop;
        const ObjectCapacity_ABC* burn = type.GetCapacity< BurnSurfaceCapacity >();
        return ( burn ? burn : type.GetCapacity< CloudPropagationCapacity >() );
    }
    return 0;
}
