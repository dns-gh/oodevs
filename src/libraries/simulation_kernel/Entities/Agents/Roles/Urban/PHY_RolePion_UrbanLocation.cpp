// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_UrbanLocation.h"
#include "InsideUrbanBlockPosition.h"
#include "LocationComputer_ABC.h"
#include "OutsideUrbanBlockPosition.h"
#include "SpeedComputer_ABC.h"
#include "DefaultUrbanLocationComputer.h"
#include "CheckPoints/SerializationTools.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Actions/Flying/PHY_RoleAction_InterfaceFlying.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Urban/UrbanPhysicalCapacity.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "simulation_terrain/TER_World.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_UrbanLocation )

using namespace urbanLocation;

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation constructor
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
PHY_RolePion_UrbanLocation::PHY_RolePion_UrbanLocation( MIL_Agent_ABC& pion )
    : owner_       ( pion )
    , urbanObject_ ( 0 )
    , delegate_    ( new OutsideUrbanBlockPosition() )
    , isInCity_    ( false )
    , isFlying_    ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation destructor
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
PHY_RolePion_UrbanLocation::~PHY_RolePion_UrbanLocation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::load
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    MIL_UrbanObject_ABC* wrapper;
    file >> wrapper
         >> isInCity_
         >> isFlying_
         >> collisions_;

    urbanObject_ = wrapper;
    if( urbanObject_ )
        delegate_.reset( new InsideUrbanBlockPosition( *urbanObject_ ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::save
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << urbanObject_;
    file << isInCity_
         << isFlying_
         << collisions_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::MagicMove
// Created: SLG 2010-04-19
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::MagicMove( MT_Vector2D vPosition )
{
    urbanObject_ = 0;
    isInCity_ = false;
    std::vector< TER_Object_ABC* > objects;
    TER_World::GetWorld().GetObjectManager().GetListAt( vPosition, objects );
    for( auto it = objects.begin(); it != objects.end(); ++it )
    {
        const MIL_UrbanObject_ABC* urbanObject = dynamic_cast< const MIL_UrbanObject_ABC* >( *it );
        if( urbanObject )
        {
            isInCity_ = true;
            if( urbanObject->IsBlock() )
            {
                urbanObject_ = urbanObject;
                delegate_.reset( new InsideUrbanBlockPosition( *urbanObject_ ) );
                return;
            }
        }
    }
    delegate_.reset( new OutsideUrbanBlockPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::NotifyMovingInsideObject
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::NotifyMovingInsideObject( MIL_Object_ABC& object )
{
    if( const MIL_UrbanObject_ABC* urbanObject = dynamic_cast< MIL_UrbanObject_ABC* >( &object ) )
    {
        if( urbanObject->IsBlock() )
        {
            urbanObject_ = urbanObject;
            if( !isFlying_ )
                delegate_.reset( new InsideUrbanBlockPosition( *urbanObject_ ) );
        }
        isInCity_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::NotifyMovingOutsideObject
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::NotifyMovingOutsideObject( MIL_Object_ABC& object )
{
    if( const MIL_UrbanObject_ABC* urbanObject = dynamic_cast< MIL_UrbanObject_ABC* >( &object ) )
    {
        if( urbanObject->GetParent() == 0 )
            isInCity_ = false;
        if( urbanObject->IsBlock() )
        {
            urbanObject_ = 0;
            delegate_.reset( new OutsideUrbanBlockPosition() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::GetCurrentUrbanBlock
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
const MIL_UrbanObject_ABC* PHY_RolePion_UrbanLocation::GetCurrentUrbanBlock() const
{
    return urbanObject_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::IsInCity
// Created: SLG 2010-04-30
// -----------------------------------------------------------------------------
bool PHY_RolePion_UrbanLocation::IsInCity() const
{
    return isInCity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::ComputeRatioPionInside
// Created: SLG 2010-04-26
// -----------------------------------------------------------------------------
double PHY_RolePion_UrbanLocation::ComputeRatioPionInside( const MT_Ellipse& attritionSurface ) const
{
    DefaultUrbanLocationComputer computer;
    const_cast< MIL_Agent_ABC& >( owner_ ).Execute< UrbanLocationComputer_ABC >( computer );
    UrbanLocationComputer_ABC::Results& result = computer.Result();
    return delegate_->ComputeRatioPionInside( result, attritionSurface );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::ComputeRatioPionInside
// Created: SLG 2010-04-26
// -----------------------------------------------------------------------------
double PHY_RolePion_UrbanLocation::ComputeRatioPionInside( const TER_Polygon& polygon, double modificator ) const
{
    DefaultUrbanLocationComputer computer;
    const_cast< MIL_Agent_ABC& >( owner_ ).Execute< UrbanLocationComputer_ABC >( computer );
    UrbanLocationComputer_ABC::Results& result = computer.Result();
    return delegate_->ComputeRatioPionInside( result, polygon, modificator );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::ComputeUrbanProtection
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
double PHY_RolePion_UrbanLocation::ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const
{
    return delegate_->ComputeUrbanProtection( dotationCategory );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::GetFirerPosition
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
MT_Vector2D PHY_RolePion_UrbanLocation::GetFirerPosition( const MT_Vector2D& target ) const
{
    return delegate_->GetFirerPosition( target, owner_.GetRole< PHY_RoleInterface_Location >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::GetTargetPosition
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
MT_Vector2D PHY_RolePion_UrbanLocation::GetTargetPosition( MIL_Agent_ABC& firer ) const
{
    DefaultUrbanLocationComputer targetComputer;
    const_cast< MIL_Agent_ABC& >( owner_ ).Execute< UrbanLocationComputer_ABC >( targetComputer );
    UrbanLocationComputer_ABC::Results& targetResult = targetComputer.Result();
    return delegate_->GetTargetPosition( firer, targetResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::ComputeDistanceInsideSameUrbanBlock
// Created: SLG 2010-04-16
// -----------------------------------------------------------------------------
double PHY_RolePion_UrbanLocation::ComputeDistanceInsideSameUrbanBlock( MIL_Agent_ABC& target ) const
{
    DefaultUrbanLocationComputer firerComputer;
    owner_.Execute< UrbanLocationComputer_ABC >( firerComputer );
    UrbanLocationComputer_ABC::Results& firerResult = firerComputer.Result();
    DefaultUrbanLocationComputer targetComputer;
    target.Execute< UrbanLocationComputer_ABC >( targetComputer );
    UrbanLocationComputer_ABC::Results& targetResult = targetComputer.Result();
    double distance = firerResult.position_.Distance( targetResult.position_ );
    return urbanObject_ ? distance * ( 1 - firerResult.urbanDeployment_ ) * ( 1 - targetResult.urbanDeployment_ ) : distance;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::Execute
// Created: SLG 2010-06-25
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::Execute( moving::SpeedComputer_ABC& algorithm ) const
{
    if( urbanObject_ && !isFlying_ && !owner_.Get< PHY_RoleAction_MovingUnderground >().PreparingToHide() )
        if( const UrbanPhysicalCapacity* physical = urbanObject_->Retrieve< UrbanPhysicalCapacity >() )
            algorithm.AddModifier( 1. - physical->GetOccupation(), true );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::CanMount
// Created: LMT 2011-05-18
// -----------------------------------------------------------------------------
bool PHY_RolePion_UrbanLocation::CanMount() const
{
    if( urbanObject_ )
        if( const UrbanPhysicalCapacity* physical = urbanObject_->Retrieve< UrbanPhysicalCapacity >() )
            return physical->GetTrafficability() >= owner_.GetRole< PHY_RoleInterface_Composantes >().GetMaxWeight( true );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::Execute
// Created: LDC 2011-08-02
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::Execute( location::LocationComputer_ABC& algorithm ) const
{
    if( urbanObject_ )
        if( const UrbanPhysicalCapacity* physical = urbanObject_->Retrieve< UrbanPhysicalCapacity >() )
            algorithm.IncreaseHeight( physical->GetHeight() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::TakeOff
// Created: LDC 2011-08-02
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::TakeOff()
{
    isFlying_ = true;
    if( urbanObject_ )
        delegate_.reset( new OutsideUrbanBlockPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::Land
// Created: LDC 2011-08-02
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::Land()
{
    isFlying_ = false;
    if( urbanObject_ )
        delegate_.reset( new InsideUrbanBlockPosition( *urbanObject_ ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::AddInhabitantCollision
// Created: MMC 2012-07-19
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::AddInhabitantCollision( unsigned int id )
{
    collisions_.insert( id );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::RemoveInhabitantCollision
// Created: MMC 2012-07-19
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::RemoveInhabitantCollision( unsigned int id )
{
    collisions_.erase( id );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::HasInhabitantCollision
// Created: LGY 2012-03-26
// -----------------------------------------------------------------------------
bool PHY_RolePion_UrbanLocation::HasInhabitantCollision() const
{
    return !collisions_.empty();
}
