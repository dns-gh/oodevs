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
#include "MIL_Object_ABC.h"

#include "BuildableCapacity.h"
#include "ImprovableCapacity.h"
#include "BypassableCapacity.h"
#include "ActivableCapacity.h"
#include "AttritionCapacity.h"
#include "MobilityCapacity.h"
#include "ExtinguishableCapacity.h"
#include "WorkableCapacity.h"
#include "DetectionCapacity.h"
#include "SpawnCapacity.h"
#include "StructuralCapacity.h"

#include "AnimatorAttribute.h"
#include "ChildObjectAttribute.h"
#include "ConstructionAttribute.h"
#include "BypassAttribute.h"
#include "MineAttribute.h"
#include "ObstacleAttribute.h"
#include "OccupantAttribute.h"

#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"

#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/DEC_Knowledge_Object.h"

#include "Tools/Iterator.h"
#include "Tools/MIL_Tools.h"

#include "simulation_terrain/TER_Localisation.h"

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
// Name: MIL_ObjectManipulator constructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
MIL_ObjectManipulator::MIL_ObjectManipulator( const MIL_Object_ABC& object )
    : object_ ( const_cast< MIL_Object_ABC& >( object ) )
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
void MIL_ObjectManipulator::Construct( MT_Float rDeltaPercentage )
{
    object_.GetAttribute< ConstructionAttribute >().Build( rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::Destroy
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::Destroy( MT_Float rDeltaPercentage )
{
    object_.GetAttribute< ConstructionAttribute >().Build( -rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::Destroy
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::Destroy()
{
    BuildableCapacity* buildableCapacity = object_.Retrieve< BuildableCapacity >();
    if( buildableCapacity )
        buildableCapacity->Destroy( object_ );

    ChildObjectAttribute* child = object_.RetrieveAttribute< ChildObjectAttribute >();
    if( child )
    {
        MIL_Object_ABC* childObject = child->GetChildObject();
        if( childObject )
            ( *childObject )().Destroy();
    }

    object_.MarkForDestruction();

    // All the knowledges associated to this object MUST be destroyed (for all the teams ..)
    const tools::Resolver< MIL_Army_ABC >& armies = MIL_AgentServer::GetWorkspace().GetEntityManager().GetArmies();
    for( tools::Iterator< const MIL_Army_ABC& > it = armies.CreateIterator(); it.HasMoreElements(); )
    {
        it.NextElement().GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( object_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::Mine
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::Mine( MT_Float rDeltaPercentage )
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
void MIL_ObjectManipulator::Demine( MT_Float rDeltaPercentage )
{
    object_.GetAttribute< MineAttribute >().Update( -rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::Bypass
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::Bypass( MT_Float rDeltaPercentage )
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
// Name: MIL_ObjectManipulator::Extinguish
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void MIL_ObjectManipulator::Extinguish( int agent, int nHose )
{
    object_.Get< ExtinguishableCapacity >().Extinguish( object_, agent, nHose );
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
    object_.Get< DetectionCapacity >().AddDetector( object_, agent );
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
    return !object_.IsMarkedForDestruction(); // && object_.GetCapacity< InvisibleCapacity >(); // TODO return object_.GetAttribute< >;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::CanBeActivated
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::CanBeActivated() const
{
    return !object_.IsMarkedForDestruction() &&
            object_.Retrieve< ActivableCapacity >() && object_.GetAttribute< ObstacleAttribute >().IsActivable() ;
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
// Name: MIL_ObjectManipulator::IsMined
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::IsMined() const
{
    return !object_.IsMarkedForDestruction() && object_.GetAttribute< MineAttribute >().GetState() > 0.;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::IsFullyMined
// Created: LDC 2009-03-18
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::IsFullyMined() const
{
    return !object_.IsMarkedForDestruction() && object_.GetAttribute< MineAttribute >().GetState() == 1.;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::IsBypassed
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::IsBypassed() const
{
    return !object_.IsMarkedForDestruction() && object_.GetAttribute< BypassAttribute >().GetState() >= 1.;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::IsBuilt
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::IsBuilt() const
{
    return !object_.IsMarkedForDestruction() && object_.GetAttribute< ConstructionAttribute >().GetState() >= 1.;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::HasMobilityInfluence
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
bool MIL_ObjectManipulator::HasMobilityInfluence() const
{
    return !object_.IsMarkedForDestruction() && object_.Retrieve< MobilityCapacity >() != 0;
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

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::ApplySpeedPolicy
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
MT_Float MIL_ObjectManipulator::ApplySpeedPolicy( MT_Float rAgentSpeedWithinObject, MT_Float rAgentSpeedWithinEnvironment, MT_Float rAgentMaxSpeed ) const
{
    const MobilityCapacity* capacity = object_.Retrieve< MobilityCapacity >();
    const StructuralCapacity* structuralcapacity = object_.Retrieve< StructuralCapacity >();
    if ( capacity )
        return capacity->ApplySpeedPolicy( rAgentSpeedWithinObject, rAgentSpeedWithinEnvironment, rAgentMaxSpeed, structuralcapacity ? 0.01 * structuralcapacity->GetStructuralState() : 1. );
    return std::numeric_limits< MT_Float >::max();
}


// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::GetSizeCoef
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
MT_Float MIL_ObjectManipulator::GetSizeCoef() const
{
    BuildableCapacity* buildable = object_.Retrieve< BuildableCapacity >();
    if( buildable && buildable->GetUnit() == ConstructionCapacity::eDensity )
        return MIL_Tools::ConvertSimToMeter( object_.GetLocalisation().GetArea() );
    ImprovableCapacity* improvable = object_.Retrieve< ImprovableCapacity >();
    if( improvable && improvable->GetUnit() == ConstructionCapacity::eDensity )
        return MIL_Tools::ConvertSimToMeter( object_.GetLocalisation().GetArea() );
    return 0.;
}
