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
#include "Object.h"

#include "BuildableCapacity.h"
#include "ImprovableCapacity.h"
#include "BypassableCapacity.h"
#include "ActivableCapacity.h"
#include "AttritionCapacity.h"
#include "MobilityCapacity.h"
#include "ExtinguishableCapacity.h"
#include "WorkableCapacity.h"

#include "AnimatorAttribute.h"
#include "ConstructionAttribute.h"
#include "BypassAttribute.h"
#include "MineAttribute.h"
#include "ObstacleAttribute.h"

#include "Tools/MIL_Tools.h"

#include "simulation_terrain/TER_Localisation.h"

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator constructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
MIL_ObjectManipulator::MIL_ObjectManipulator( Object& object )
    : object_ ( object )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator constructor
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
MIL_ObjectManipulator::MIL_ObjectManipulator( const Object& object )
    : object_ ( const_cast< Object& >( object ) )
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
    object_.Get< BuildableCapacity >().Destroy( object_ );    
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
    return !object_.IsMarkedForDestruction() && object_.Retrieve< BuildableCapacity >() != 0;
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
    if ( object_.Retrieve< WorkableCapacity >() != 0 )
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
    if ( capacity )
        return capacity->ApplySpeedPolicy( rAgentSpeedWithinObject, rAgentSpeedWithinEnvironment, rAgentMaxSpeed );
    return std::numeric_limits< MT_Float >::max();
}


// -----------------------------------------------------------------------------
// Name: MIL_ObjectManipulator::GetSizeCoef
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
MT_Float MIL_ObjectManipulator::GetSizeCoef() const
{    
    BuildableCapacity* buildable = object_.Retrieve< BuildableCapacity >();    
    if ( buildable && buildable->GetUnit() == ConstructionCapacity::eDensity )
        return MIL_Tools::ConvertSimToMeter( object_.GetLocalisation().GetArea() );
    ImprovableCapacity* improvable = object_.Retrieve< ImprovableCapacity >();    
    if ( improvable && improvable->GetUnit() == ConstructionCapacity::eDensity )
        return MIL_Tools::ConvertSimToMeter( object_.GetLocalisation().GetArea() );
    return 0.;
}
