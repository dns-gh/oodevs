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
#include "AlgorithmsFactories.h"
#include "InsideUrbanBlockPosition.h"
#include "LocationComputer_ABC.h"
#include "OutsideUrbanBlockPosition.h"
#include "PostureComputer_ABC.h"
#include "SpeedComputer_ABC.h"
#include "UrbanLocationComputer_ABC.h"
#include "UrbanLocationComputerFactory_ABC.h"
#include "Checkpoints/SerializationTools.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Actions/Flying/PHY_RoleAction_InterfaceFlying.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
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
    , hasCollision_( false )
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
    UrbanObjectWrapper* wrapper;
    file >> wrapper
         >> isInCity_
         >> isFlying_;
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
         << isFlying_;
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
    for( std::vector< TER_Object_ABC* >::const_iterator it = objects.begin(); it != objects.end(); ++it )
    {
        const UrbanObjectWrapper* urbanObject = dynamic_cast< const UrbanObjectWrapper* >( *it );
        if( urbanObject )
        {
            isInCity_ = true;
            if( !urbanObject->HasChild() )
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
    if( const UrbanObjectWrapper* urbanObject = dynamic_cast< UrbanObjectWrapper* >( &object ) )
    {
        if( !urbanObject->HasChild() )
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
    if( const UrbanObjectWrapper* urbanObject = dynamic_cast< UrbanObjectWrapper* >( &object ) )
    {
        if( !urbanObject->HasParent() )
            isInCity_ = false;
        if( !urbanObject->HasChild() )
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
const UrbanObjectWrapper* PHY_RolePion_UrbanLocation::GetCurrentUrbanBlock() const
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
    std::auto_ptr< UrbanLocationComputer_ABC > computer( const_cast< MIL_Agent_ABC& >( owner_ ).GetAlgorithms().urbanLocationComputerFactory_->Create() );
    const_cast< MIL_Agent_ABC& >( owner_ ).Execute( *computer );
    UrbanLocationComputer_ABC::Results& result = computer->Result();
    return delegate_->ComputeRatioPionInside( result, attritionSurface );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::ComputeRatioPionInside
// Created: SLG 2010-04-26
// -----------------------------------------------------------------------------
double PHY_RolePion_UrbanLocation::ComputeRatioPionInside( const TER_Polygon& polygon, double modificator ) const
{
    std::auto_ptr< UrbanLocationComputer_ABC > computer( const_cast< MIL_Agent_ABC& >( owner_ ).GetAlgorithms().urbanLocationComputerFactory_->Create() );
    const_cast< MIL_Agent_ABC& >( owner_ ).Execute( *computer );
    UrbanLocationComputer_ABC::Results& result = computer->Result();
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
MT_Vector2D PHY_RolePion_UrbanLocation::GetFirerPosition( MIL_Agent_ABC& target ) const
{
    std::auto_ptr< UrbanLocationComputer_ABC > firerComputer( const_cast< MIL_Agent_ABC& >( owner_ ).GetAlgorithms().urbanLocationComputerFactory_->Create() );
    const_cast< MIL_Agent_ABC& >( owner_ ).Execute( *firerComputer );
    UrbanLocationComputer_ABC::Results& firerResult = firerComputer->Result();
    return delegate_->GetFirerPosition( target, firerResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::GetTargetPosition
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
MT_Vector2D PHY_RolePion_UrbanLocation::GetTargetPosition( MIL_Agent_ABC& firer ) const
{
    std::auto_ptr< UrbanLocationComputer_ABC > targetComputer( const_cast< MIL_Agent_ABC& >( owner_ ).GetAlgorithms().urbanLocationComputerFactory_->Create() );
    const_cast< MIL_Agent_ABC& >( owner_ ).Execute( *targetComputer );
    UrbanLocationComputer_ABC::Results& targetResult = targetComputer->Result();
    return delegate_->GetTargetPosition( firer, targetResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::ComputeDistanceInsideSameUrbanBlock
// Created: SLG 2010-04-16
// -----------------------------------------------------------------------------
double PHY_RolePion_UrbanLocation::ComputeDistanceInsideSameUrbanBlock( MIL_Agent_ABC& target ) const
{
    std::auto_ptr< UrbanLocationComputer_ABC > firerComputer( const_cast< MIL_Agent_ABC& >( owner_ ).GetAlgorithms().urbanLocationComputerFactory_->Create() );
    owner_.Execute( *firerComputer );
    UrbanLocationComputer_ABC::Results& firerResult = firerComputer->Result();
    std::auto_ptr< UrbanLocationComputer_ABC > targetComputer( target.GetAlgorithms().urbanLocationComputerFactory_->Create() );
    target.Execute( *targetComputer );
    UrbanLocationComputer_ABC::Results& targetResult = targetComputer->Result();
    double distance = firerResult.position_.Distance( targetResult.position_ );
    return urbanObject_ ? distance * ( 1 - firerResult.urbanDeployment_ ) * ( 1 - targetResult.urbanDeployment_ ) : distance;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::Execute
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::Execute( posture::PostureComputer_ABC& /*algorithm*/ ) const
{
    // $$$$ LDC FIXME The value of ComputeXomplexity is just stupid (1 million...). It also doesn't make any sense for an isolated individual.
//    if( urbanObject_ )
//    {
//        double timeModifier = urbanObject_->GetObject().ComputeComplexity();
//        algorithm.AddUrbanCoefficientModifier( timeModifier );
//    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::Execute
// Created: SLG 2010-06-25
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::Execute( moving::SpeedComputer_ABC& algorithm ) const
{
    if( urbanObject_ && !isFlying_ && !owner_.Get< PHY_RoleAction_MovingUnderground >().PreparingToHide() )
        algorithm.AddModifier( 1. - urbanObject_->GetOccupation(), true );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::CanMount
// Created: LMT 2011-05-18
// -----------------------------------------------------------------------------
bool PHY_RolePion_UrbanLocation::CanMount() const
{
    if( urbanObject_ )
        return urbanObject_->GetTrafficability() >= owner_.GetRole< PHY_RoleInterface_Composantes >().GetMajorComponentWeight( true );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::Execute
// Created: LDC 2011-08-02
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::Execute( location::LocationComputer_ABC& algorithm ) const
{
    if( urbanObject_ )
        algorithm.IncreaseHeight( urbanObject_->GetHeight() );
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
// Name: PHY_RolePion_UrbanLocation::SetInhabitantCollision
// Created: LGY 2012-03-26
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::SetInhabitantCollision( bool value )
{
    hasCollision_ = value;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::HasInhabitantCollision
// Created: LGY 2012-03-26
// -----------------------------------------------------------------------------
bool PHY_RolePion_UrbanLocation::HasInhabitantCollision() const
{
    return hasCollision_;
}
