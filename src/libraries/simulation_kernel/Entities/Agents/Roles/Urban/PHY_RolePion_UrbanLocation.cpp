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
#include "AlgorithmsFactories.h"
#include "UrbanLocationComputer_ABC.h"
#include "UrbanLocationComputerFactory_ABC.h"
#include "UrbanType.h"
#include "UrbanModel.h"
#include <urban/Model.h>
#include <urban/Architecture.h>
#include <urban/MaterialCompositionType.h>
#include <urban/Soil.h>
#include <urban/StaticModel.h>
#include <urban/TerrainObject_ABC.h>

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

MT_Random PHY_RolePion_UrbanLocation::randomGenerator_;
using namespace urbanLocation;

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation constructor
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
PHY_RolePion_UrbanLocation::PHY_RolePion_UrbanLocation( MIL_Agent_ABC& pion )
    : pion_( pion )
    , urbanObject_( 0 )
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
    //TODO
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::save
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::MagicMove
// Created: SLG 2010-04-19
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::MagicMove( MT_Vector2D vPosition )
{
    std::vector< const urban::TerrainObject_ABC* > urbanBlocks;
    geometry::Point2f point( vPosition.rX_, vPosition.rY_ ); 
    UrbanModel::GetSingleton().GetModel().GetListWithinCircle( point, 500, urbanBlocks );
    for (std::vector< const urban::TerrainObject_ABC* >::const_iterator it = urbanBlocks.begin(); it != urbanBlocks.end(); ++it )
    {
        if( (**it).GetFootprint()->IsInside( point ) )
            urbanObject_ = *it;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::NotifyMoveInside
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::NotifyMovingInsideUrbanBlock( const urban::TerrainObject_ABC& urbanObject )
{
    if( !urbanObject_ )
        urbanObject_ = &urbanObject; 
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::NotifyMoveOutside
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::NotifyMovingOutsideUrbanBlock( const urban::TerrainObject_ABC& urbanObject )
{
    if( urbanObject_ != &urbanObject )
        throw std::exception( "urbanObject in PHY_RolePion_UrbanLocation not defined" );
    urbanObject_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::GetCurrentUrbanBlock
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
const urban::TerrainObject_ABC* PHY_RolePion_UrbanLocation::GetCurrentUrbanBlock() const
{
    return urbanObject_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::ComputeUrbanProtection
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_UrbanLocation::ComputeUrbanProtection( const PHY_DotationCategory& dotationCategory ) const
{
    if ( urbanObject_ )
    {
        const urban::Architecture* architecture = urbanObject_->RetrievePhysicalFeature< urban::Architecture >();
        const urban::Soil* soil = urbanObject_->RetrievePhysicalFeature< urban::Soil >();
        if( architecture  && soil )
        {
            unsigned materialID = UrbanType::GetUrbanType().GetStaticModel().FindType< urban::MaterialCompositionType >( architecture->GetMaterial() )->GetId();
            return ( 1 -dotationCategory.GetUrbanAttritionModifer( materialID ) )* ( soil->GetOccupation() ); // TODO SLG A changer quand les paramètres des BU changeront
        }
    }
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::GetFirerPosition
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
geometry::Point2f PHY_RolePion_UrbanLocation::GetFirerPosition( MIL_Agent_ABC& target ) const
{
    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > firerComputer( const_cast< MIL_Agent_ABC& >( pion_ ).GetAlgorithms().urbanLocationComputerFactory_->Create( *urbanObject_ ) );
    const_cast< MIL_Agent_ABC& >( pion_ ).Execute( *firerComputer );
    UrbanLocationComputer_ABC::Results& firerResult = firerComputer->Result();

    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > targetComputer( target.GetAlgorithms().urbanLocationComputerFactory_->Create( *urbanObject_ ) );
    target.Execute( *targetComputer );
    UrbanLocationComputer_ABC::Results& targetResult = targetComputer->Result();

    if( urbanObject_ )
    {
        std::vector< geometry::Point2f > points = urbanObject_->GetFootprint()->Intersect( geometry::Segment2f( firerResult.position_, targetResult.position_ ) );
        if( points.empty() )
            throw std::exception( " error in urbanBlock intersection" );
        return GetNearestUrbanBlockPoint( firerResult.position_, points ); 
    }
    return firerResult.position_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::GetTargetPosition
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
geometry::Point2f PHY_RolePion_UrbanLocation::GetTargetPosition( MIL_Agent_ABC& target ) const
{
    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > firerComputer( const_cast< MIL_Agent_ABC& >( pion_ ).GetAlgorithms().urbanLocationComputerFactory_->Create( *urbanObject_ ) );
    const_cast< MIL_Agent_ABC& >( pion_ ).Execute( *firerComputer );
    UrbanLocationComputer_ABC::Results& firerResult = firerComputer->Result();

    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > targetComputer( target.GetAlgorithms().urbanLocationComputerFactory_->Create( *urbanObject_ ) );
    target.Execute( *targetComputer );
    UrbanLocationComputer_ABC::Results& targetResult = targetComputer->Result();

    if( urbanObject_ )
    {
        geometry::Line2f lineTmp( firerResult.position_, targetResult.position_ );
        std::vector< geometry::Point2f > points = urbanObject_->GetFootprint()->Intersect( lineTmp );
        if( points.size() < 2 )
            throw std::exception( " error in urbanBlock intersection" );
        geometry::Point2f pfirst = GetNearestUrbanBlockPoint( firerResult.position_, points );
        geometry::Point2f pSecond = GetFurthestUrbanBlockPoint( firerResult.position_, points );
        geometry::Vector2f vector( pfirst, pSecond );
        vector = vector * ( 1 - firerResult.urbanDeployment_ );
        geometry::Point2f pM = pfirst + vector;
        vector = geometry::Vector2f( pM, pSecond ) * randomGenerator_.rand_ii();
        return pM + vector;
    }
    return targetResult.position_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::ComputeDistanceInsideSameUrbanBlock
// Created: SLG 2010-04-16
// -----------------------------------------------------------------------------
float PHY_RolePion_UrbanLocation::ComputeDistanceInsideSameUrbanBlock( MIL_Agent_ABC& target  ) const
{
    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > firerComputer( const_cast< MIL_Agent_ABC& >( pion_ ).GetAlgorithms().urbanLocationComputerFactory_->Create( *urbanObject_ ) );
    pion_.Execute( *firerComputer );
    UrbanLocationComputer_ABC::Results& firerResult = firerComputer->Result();

    std::auto_ptr< urbanLocation::UrbanLocationComputer_ABC > targetComputer( target.GetAlgorithms().urbanLocationComputerFactory_->Create( *urbanObject_ ) );
    target.Execute( *targetComputer );
    UrbanLocationComputer_ABC::Results& targetResult = targetComputer->Result();

    if( urbanObject_ )
    {
        geometry::Rectangle2f boundingBox = urbanObject_->GetFootprint()->BoundingBox();
        float distance = std::max( boundingBox.Height(), boundingBox.Width() );
        float firerDistance = ( distance/2 ) * firerResult.urbanDeployment_;
        float targetDistance = ( distance/2 ) * targetResult.urbanDeployment_;
        distance = ( distance - firerDistance - targetDistance ) * randomGenerator_.rand_ii();
        return distance;
    }
    return firerResult.position_.Distance( targetResult.position_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::GetNearestUrbanBlockPoint
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
geometry::Point2f PHY_RolePion_UrbanLocation::GetNearestUrbanBlockPoint( const geometry::Point2f pionPosition, const std::vector< geometry::Point2f > points ) const 
{
    geometry::Point2f nearestPosition;
    float distance = std::numeric_limits< MT_Float >::max();
    for( std::vector< geometry::Point2f >::const_iterator it = points.begin(); it != points.end(); ++it )
    {
        float distanceTemp = (*it).Distance( pionPosition );
        if( distanceTemp < distance )
        {
            distance = distanceTemp;
            nearestPosition = *it;
        }
    }
    return nearestPosition;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::GetFurthestUrbanBlockPoint
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
geometry::Point2f PHY_RolePion_UrbanLocation::GetFurthestUrbanBlockPoint( const geometry::Point2f pionPosition, const std::vector< geometry::Point2f > points ) const
{
    geometry::Point2f furthestPosition;
    float distance = 0;
    for( std::vector< geometry::Point2f >::const_iterator it = points.begin(); it != points.end(); ++it )
    {
        float distanceTemp = (*it).Distance( pionPosition );
        if( distanceTemp > distance )
        {
            distance = distanceTemp;
            furthestPosition = *it;
        }
    }
    return furthestPosition;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::Execute
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::Execute( posture::PostureComputer_ABC& algorithm ) const
{
    if ( urbanObject_ )
    {
        double timeModifier = urbanObject_->ComputeComplexity();
        algorithm.AddUrbanCoefficientModifier( timeModifier );
    }
}