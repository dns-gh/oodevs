// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ObjectManipulator.h"
#include "MIL_AgentServer.h"
#include "MIL_Object_ABC.h"
#include "ActivableCapacity.h"
#include "AnimatorAttribute.h"
#include "AttritionCapacity.h"
#include "BuildableCapacity.h"
#include "BypassAttribute.h"
#include "BypassableCapacity.h"
#include "ConstructionAttribute.h"
#include "CrowdCapacity.h"
#include "DetectionCapacity.h"
#include "ImprovableCapacity.h"
#include "InteractWithSideCapacity.h"
#include "MineAttribute.h"
#include "MobilityCapacity.h"
#include "ObstacleAttribute.h"
#include "OccupantAttribute.h"
#include "ResourceNetworkCapacity.h"
#include "SpawnCapacity.h"
#include "StructuralCapacity.h"
#include "TrafficabilityAttribute.h"
#include "WorkableCapacity.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Actions/Flying/PHY_RoleAction_InterfaceFlying.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Objects/BridgingCapacity.h"
#include "Entities/Objects/ConstructionAttribute.h"
#include "Tools/MIL_Tools.h"
#include "simulation_terrain/TER_Localisation.h"
#include <tools/Iterator.h>

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator constructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
MIL_ObjectManipulator::MIL_ObjectManipulator( MIL_Object_ABC& object )
    : object_ ( object )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator destructor
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
MIL_ObjectManipulator::~MIL_ObjectManipulator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::Construct
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::Construct()
{
    BuildableCapacity* pCapacity = object_.Retrieve< BuildableCapacity >();
    if( pCapacity )
        pCapacity->Construct( object_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::Construct
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::Construct( double rDeltaPercentage )
{
    double state = 1.;
    if( StructuralCapacity* pCapacity = object_.Retrieve< StructuralCapacity >() )
    {
        pCapacity->Build( rDeltaPercentage );
        state = pCapacity->GetStructuralState();
    }
    if( ConstructionAttribute* pAttribute = object_.RetrieveAttribute< ConstructionAttribute >() )
    {
        pAttribute->Build( rDeltaPercentage );
        state = std::min( state, pAttribute->GetState() );
    }
    if( state >= 0.99 )
        if( ResourceNetworkCapacity* resourceNetwork = object_.Retrieve< ResourceNetworkCapacity >() )
            resourceNetwork->ActivateAll();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::Destroy
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::Destroy( double rDeltaPercentage )
{
    if( StructuralCapacity* pCapacity = object_.Retrieve< StructuralCapacity >() )
        pCapacity->Build( -rDeltaPercentage );
    if( ConstructionAttribute* pAttribute = object_.RetrieveAttribute< ConstructionAttribute >() )
        pAttribute->Build( -rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::Destroy
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::Destroy()
{
    object_.MarkForDestructionNextUpdate();
    }

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::Mine
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::Mine( double rDeltaPercentage )
{
    object_.GetAttribute< MineAttribute >().Update( rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::Mine
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::Mine()
{
    object_.Get< ImprovableCapacity >().Mine( object_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::Demine
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::Demine( double rDeltaPercentage )
{
    object_.GetAttribute< MineAttribute >().Update( -rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::Bypass
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::Bypass( double rDeltaPercentage )
{
    object_.Get< BypassableCapacity >().Bypass( object_, rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::Activate
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::Activate()
{
    object_.Get< ActivableCapacity >().Activate( object_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::AddAnimator
// Created: JCR 2008-08-21
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::AddAnimator( const MIL_Agent_ABC& agent )
{
    object_.Get< WorkableCapacity >().AddAnimator( object_, agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::AddCreator
// Created: SLG 2010-02-17
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::AddCreator( const MIL_Agent_ABC& agent )
{
    object_.Get< SpawnCapacity >().AddCreator( object_, agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::AddDetector
// Created: SLG 2010-02-17
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::AddDetector( const MIL_Agent_ABC& agent )
{
    DetectionCapacity* capacity = object_.Retrieve< DetectionCapacity >();
    if( capacity )
        capacity->AddDetector( object_, agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::ReleaseAnimator
// Created: JCR 2008-08-21
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::ReleaseAnimator( const MIL_Agent_ABC& agent )
{
    object_.Get< WorkableCapacity >().ReleaseAnimator( object_, agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::CanBeConstructed
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::CanBeConstructed() const
{
    return !object_.IsMarkedForDestruction();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::CanBeMined
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::CanBeMined() const
{
    return !object_.IsMarkedForDestruction() && object_.Retrieve< ImprovableCapacity >() != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::CanBeDemined
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::CanBeDemined() const
{
    return CanBeMined();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::CanBeDestroyed
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::CanBeDestroyed() const
{
    return CanBeConstructed();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::CanBeBypassed
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::CanBeBypassed() const
{
    return  !object_.IsMarkedForDestruction() && object_.Retrieve< BypassableCapacity >() != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::CanBePerceived
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::CanBePerceived() const
{
    const ObstacleAttribute* obstacle = object_.RetrieveAttribute< ObstacleAttribute >();
    if( obstacle && !obstacle->IsActivated() )
        return false;
    return !object_.IsMarkedForDestruction(); // && object_.GetCapacity< InvisibleCapacity >(); // TODO return object_.GetAttribute< >;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::CanBeActivated
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::CanBeActivated() const
{
    return !object_.IsMarkedForDestruction() &&
            object_.Retrieve< ActivableCapacity >() &&
           !object_.GetAttribute< ObstacleAttribute >().IsActivated() ;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::CanBeOccupiedBy
// Created: MGD 2010-02-18
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::CanBeOccupiedBy( const MIL_Agent_ABC& agent ) const
{
    if( !object_.IsMarkedForDestruction() )
    {
        if( OccupantAttribute* occupiedAttribut = object_.RetrieveAttribute< OccupantAttribute >() )
        {
            const MIL_Agent_ABC* occupant = occupiedAttribut->GetOccupant();
            return !occupant || occupant == &agent;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::IsTrafficable
// Created: CMA 2011-09-12
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::IsTrafficable( const MIL_Agent_ABC& agent ) const
{
    if( !agent.GetRole< PHY_RoleAction_InterfaceFlying >().IsFlying() && object_.CanInteractWith( agent ) )
    {
        BridgingCapacity* bridgingCapacity = object_.Retrieve< BridgingCapacity >();
        if( bridgingCapacity && bridgingCapacity->IsBridge() )
        {
            const ConstructionAttribute* pConstruction = object_.RetrieveAttribute< ConstructionAttribute >();
            if( pConstruction && !pConstruction->IsConstructed() )
                return false;
        }
        if( const TrafficabilityAttribute* pTrafficability = object_.RetrieveAttribute< TrafficabilityAttribute >() )
        {
                double weight = agent.GetRole< PHY_RoleInterface_Composantes >().GetMaxWeight();
                return ( pTrafficability->GetMaxValue() > weight );
            }
        }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::IsMined
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::IsMined() const
{
    if( object_.IsMarkedForDestruction() )
        return false;
    const MineAttribute* attribute = object_.RetrieveAttribute< MineAttribute >();
    return attribute && attribute->GetState() > 0.;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::IsFullyMined
// Created: LDC 2009-03-18
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::IsFullyMined() const
{
    if( object_.IsMarkedForDestruction() )
        return false;
    const MineAttribute* attribute = object_.RetrieveAttribute< MineAttribute >();
    return attribute && attribute->GetState() ==1.;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::IsBypassed
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::IsBypassed() const
{
    if( object_.IsMarkedForDestruction() )
        return false;
    const BypassAttribute* attribute = object_.RetrieveAttribute< BypassAttribute >();
    return attribute && attribute->IsBypassed();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::IsBuilt
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::IsBuilt() const
{
    if( object_.IsMarkedForDestruction() )
        return false;
    if( const ConstructionAttribute* pAttribute = object_.RetrieveAttribute< ConstructionAttribute >() )
        return pAttribute->IsConstructed();
    if( const StructuralCapacity* pCapacity = object_.Retrieve< StructuralCapacity >() )
        return pCapacity->GetStructuralState() >= 1.f;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::HasMobilityInfluence
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::HasMobilityInfluence() const
{
    return !object_.IsMarkedForDestruction() &&
           ( object_.Retrieve< MobilityCapacity >() != 0 ||
             object_.Retrieve< CrowdCapacity >() != 0 ||
             object_.RetrieveAttribute< TrafficabilityAttribute >() != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::CanBeAnimated
// Created: JCR 2008-08-26
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::CanBeAnimatedBy( const MIL_Agent_ABC& agent ) const
{
    if( object_.Retrieve< WorkableCapacity >() != 0 )
        return object_.GetAttribute< AnimatorAttribute >().CanBeAnimatedBy( agent );
    return false;
}

namespace
{
    double GetSpeed( double rAgentSpeedWithinObject, double rAgentSpeedWithinEnvironment, double rAgentMaxSpeed,
                     bool canInteractWith, const MIL_Entity_ABC& entity, MIL_Object_ABC& object )
    {
        double speed = std::numeric_limits< double >::max();
        const PHY_RoleAction_InterfaceFlying* flying = entity.RetrieveRole< PHY_RoleAction_InterfaceFlying >();
        if( ( !flying || !flying->IsFlying() ) && canInteractWith )
        {
            const MobilityCapacity* capacity = object.Retrieve< MobilityCapacity >();
            if( capacity )
            {
                const StructuralCapacity* structuralcapacity = object.Retrieve< StructuralCapacity >();
                speed = capacity->ApplySpeedPolicy( rAgentSpeedWithinObject, rAgentSpeedWithinEnvironment, rAgentMaxSpeed, structuralcapacity ? structuralcapacity->GetStructuralState() : 1. );
            }
            const CrowdCapacity* crowdcapacity = object.Retrieve< CrowdCapacity >();
            if( crowdcapacity )
                speed = std::min( speed, crowdcapacity->ApplySpeedPolicy( entity ) );
        }
        return std::min( speed, rAgentSpeedWithinObject );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::ApplySpeedPolicy
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
double MIL_ObjectManipulator::ApplySpeedPolicy( double rAgentSpeedWithinObject, double rAgentSpeedWithinEnvironment, double rAgentMaxSpeed, const MIL_Agent_ABC& agent ) const
{
    return GetSpeed( rAgentSpeedWithinObject, rAgentSpeedWithinEnvironment, rAgentMaxSpeed, object_.CanInteractWith( agent ), agent, object_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::ApplySpeedPolicy
// Created: LGY 2013-01-04
// -----------------------------------------------------------------------------
double MIL_ObjectManipulator::ApplySpeedPolicy( double rAgentSpeedWithinObject, double rAgentSpeedWithinEnvironment, double rAgentMaxSpeed, const MIL_Population& population ) const
{
    return GetSpeed( rAgentSpeedWithinObject, rAgentSpeedWithinEnvironment, rAgentMaxSpeed, object_.CanInteractWith( population ), population, object_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::GetMaxSpeed
// Created: LDC 2011-05-19
// -----------------------------------------------------------------------------
double MIL_ObjectManipulator::GetMaxSpeed() const
{
    const BypassableCapacity* bypass = object_.Retrieve< BypassableCapacity >();
    if( bypass && bypass->IsBypassed( object_ ) )
        return bypass->GetBypassSpeed();
    if( const MobilityCapacity* mobility = object_.Retrieve< MobilityCapacity >() )
        return mobility->GetDefaultSpeed();
    return std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::GetSizeCoef
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
double MIL_ObjectManipulator::GetSizeCoef() const
{
    BuildableCapacity* buildable = object_.Retrieve< BuildableCapacity >();
    if( buildable && buildable->GetUnit() == ConstructionCapacity::eDensity )
        return MIL_Tools::ConvertSimToMeter( object_.GetLocalisation().GetArea() );
    ImprovableCapacity* improvable = object_.Retrieve< ImprovableCapacity >();
    if( improvable && improvable->GetUnit() == ConstructionCapacity::eDensity )
        return MIL_Tools::ConvertSimToMeter( object_.GetLocalisation().GetArea() );
    return 0.;
}
