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
#include "PostureComputer_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "AlgorithmsFactories.h"
#include "SpeedComputer_ABC.h"
#include "UrbanLocationComputer_ABC.h"
#include "UrbanBlockPosition_ABC.h"
#include "InsideUrbanBlockPosition.h"
#include "OutsideUrbanBlockPosition.h"
#include "UrbanLocationComputerFactory_ABC.h"
#include "UrbanType.h"
#include "UrbanModel.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "simulation_terrain/TER_World.h"
#include <urban/Architecture.h>
#include <urban/Model.h>
#include <urban/TerrainObjectVisitor_ABC.h>
#include <urban/TerrainObject_ABC.h>
#include <boost/bind.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_UrbanLocation )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_UrbanLocation* role, const unsigned int /*version*/ )
{
    MIL_Agent_ABC* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePion_UrbanLocation* role, const unsigned int /*version*/ )
{
    MIL_Agent_ABC* pion;
    archive >> pion;
    ::new( role )PHY_RolePion_UrbanLocation( *pion );
}

using namespace urbanLocation;

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation constructor
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
PHY_RolePion_UrbanLocation::PHY_RolePion_UrbanLocation( MIL_Agent_ABC& pion )
    : pion_( pion )
    , urbanObject_( 0 )
    , delegate_( new OutsideUrbanBlockPosition() )
    , isInCity_( false )
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
void PHY_RolePion_UrbanLocation::load( MIL_CheckPointInArchive& /*file*/, const unsigned int )
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::save
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::save( MIL_CheckPointOutArchive& /*file*/, const unsigned int ) const
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::MagicMove
// Created: SLG 2010-04-19
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::MagicMove( MT_Vector2D vPosition )
{
    urbanObject_ = 0;
    isInCity_ = false;
    std::vector< const TER_Object_ABC* > objects;
    TER_World::GetWorld().GetObjectManager().GetListWithinCircle2( vPosition, 500, objects );
    for (std::vector< const TER_Object_ABC* >::const_iterator it = objects.begin(); it != objects.end(); ++it )
    {
        const UrbanObjectWrapper* urbanObject = dynamic_cast< const UrbanObjectWrapper* >( *it );
        if( urbanObject && urbanObject->GetLocalisation().GetArea() && urbanObject->IsInside( vPosition ) )
        {
            isInCity_ = true;
            if( !urbanObject->GetObject().HasChild() )
            {
                urbanObject_ = urbanObject;
                delegate_.reset( new InsideUrbanBlockPosition( &urbanObject_->GetObject() ) );
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
    UrbanObjectWrapper* urbanObject = dynamic_cast< UrbanObjectWrapper* >( &object );
    if( urbanObject )
    {
        if( !urbanObject->GetObject().HasChild() )
        {
            urbanObject_ = urbanObject;
            delegate_.reset( new InsideUrbanBlockPosition( &urbanObject_->GetObject() ) );
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
    UrbanObjectWrapper* urbanObject = dynamic_cast< UrbanObjectWrapper* >( &object );
    if( urbanObject )
    {
        if( !urbanObject->GetObject().GetParent() )
            isInCity_ = false;
        if( !urbanObject->GetObject().HasChild() )
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
float PHY_RolePion_UrbanLocation::ComputeRatioPionInside( const MT_Ellipse& attritionSurface ) const
{
    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > computer( const_cast< MIL_Agent_ABC& >( pion_ ).GetAlgorithms().urbanLocationComputerFactory_->Create() );
    const_cast< MIL_Agent_ABC& >( pion_ ).Execute( *computer );
    UrbanLocationComputer_ABC::Results& result = computer->Result();

    return delegate_->ComputeRatioPionInside( result, attritionSurface );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::ComputeRatioPionInside
// Created: SLG 2010-04-26
// -----------------------------------------------------------------------------
float PHY_RolePion_UrbanLocation::ComputeRatioPionInside( const geometry::Polygon2f& polygon, float modificator ) const
{
    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > computer( const_cast< MIL_Agent_ABC& >( pion_ ).GetAlgorithms().urbanLocationComputerFactory_->Create() );
    const_cast< MIL_Agent_ABC& >( pion_ ).Execute( *computer );
    UrbanLocationComputer_ABC::Results& result = computer->Result();

    return delegate_->ComputeRatioPionInside( result, polygon, modificator );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::ComputeUrbanProtection
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_UrbanLocation::ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const
{
    return delegate_->ComputeUrbanProtection( dotationCategory );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::GetFirerPosition
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
geometry::Point2f PHY_RolePion_UrbanLocation::GetFirerPosition( MIL_Agent_ABC& target ) const
{
    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > firerComputer( const_cast< MIL_Agent_ABC& >( pion_ ).GetAlgorithms().urbanLocationComputerFactory_->Create() );
    const_cast< MIL_Agent_ABC& >( pion_ ).Execute( *firerComputer );
    UrbanLocationComputer_ABC::Results& firerResult = firerComputer->Result();

    return delegate_->GetFirerPosition( target, firerResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::GetTargetPosition
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
geometry::Point2f PHY_RolePion_UrbanLocation::GetTargetPosition( MIL_Agent_ABC& firer ) const
{
    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > targetComputer( const_cast< MIL_Agent_ABC& >( pion_ ).GetAlgorithms().urbanLocationComputerFactory_->Create() );
    const_cast< MIL_Agent_ABC& >( pion_ ).Execute( *targetComputer );
    UrbanLocationComputer_ABC::Results& targetResult = targetComputer->Result();

    return delegate_->GetTargetPosition( firer, targetResult );

}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::ComputeDistanceInsideSameUrbanBlock
// Created: SLG 2010-04-16
// -----------------------------------------------------------------------------
float PHY_RolePion_UrbanLocation::ComputeDistanceInsideSameUrbanBlock( MIL_Agent_ABC& target ) const
{
    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > firerComputer( const_cast< MIL_Agent_ABC& >( pion_ ).GetAlgorithms().urbanLocationComputerFactory_->Create() );
    pion_.Execute( *firerComputer );
    UrbanLocationComputer_ABC::Results& firerResult = firerComputer->Result();

    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > targetComputer( target.GetAlgorithms().urbanLocationComputerFactory_->Create() );
    target.Execute( *targetComputer );
    UrbanLocationComputer_ABC::Results& targetResult = targetComputer->Result();

    if( urbanObject_ )
    {
        float distance = firerResult.position_.Distance( targetResult.position_ );
        return distance * ( 1 - firerResult.urbanDeployment_ ) * ( 1 - targetResult.urbanDeployment_ );
    }
    return firerResult.position_.Distance( targetResult.position_ );
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
    if( urbanObject_ )
    {
        const urban::Architecture* architecture = urbanObject_->GetObject().Retrieve< urban::Architecture >();
        if( architecture )
            algorithm.AddModifier( 1. - architecture->GetOccupation() , true );
    }
}