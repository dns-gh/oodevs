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
#include "UrbanBlockPosition_ABC.h"
#include "InsideUrbanBlockPosition.h"
#include "OutsideUrbanBlockPosition.h"
#include "UrbanLocationComputerFactory_ABC.h"
#include "UrbanType.h"
#include "UrbanModel.h"
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

MT_Random PHY_RolePion_UrbanLocation::randomGenerator_;
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
    const geometry::Point2f point( static_cast< float >( vPosition.rX_ ), static_cast< float >( vPosition.rY_ ) ); 
    UrbanBlockMagicMove( point );
    CityMagicMove( point );    
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::UrbanBlockMagicMove
// Created: SLG 2010-04-28
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::UrbanBlockMagicMove( const geometry::Point2f& point )
{
    std::vector< const urban::TerrainObject_ABC* > urbanBlocks;
    UrbanModel::GetSingleton().GetModel().GetListWithinCircle( point, 500, urbanBlocks );
    for (std::vector< const urban::TerrainObject_ABC* >::const_iterator it = urbanBlocks.begin(); it != urbanBlocks.end(); ++it )
    {
        if( (**it).GetFootprint()->IsInside( point ) )
        {
            urbanObject_ = *it;
            delegate_.reset( new InsideUrbanBlockPosition( urbanObject_ ) ); 
            return;
        }
    }
    urbanObject_ = 0;
    delegate_.reset( new OutsideUrbanBlockPosition() ); 

}

namespace
{
    bool IsInside( const urban::TerrainObject_ABC* object, const geometry::Point2f& point )
    {
        return object->GetFootprint()->IsInside( point );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::CityMagicMove
// Created: SLG 2010-04-28
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::CityMagicMove( const geometry::Point2f& point )
{
    T_Cities cities = UrbanModel::GetSingleton().GetModel().GetCities();
    CIT_Cities it = std::find_if( cities.begin(), cities.end(), boost::bind( &IsInside, _1, boost::cref( point ) ) );
    isInCity_ = it != cities.end();
}

namespace
{
    class UrbanMovingInsideVisitor : public urban::TerrainObjectVisitor_ABC
    {
    public:
        UrbanMovingInsideVisitor( bool &isInCity, const urban::TerrainObject_ABC*& urbanObject, std::auto_ptr< UrbanBlockPosition_ABC > delegator ) : isInCity_( isInCity ), currentUrbanObject_( urbanObject ), delegate_( delegator )
        {}
        ~UrbanMovingInsideVisitor()
        {}
        virtual void VisitBlock( const urban::TerrainObject_ABC& urbanObject )
        {
            currentUrbanObject_ = &urbanObject;
            delegate_.reset( new InsideUrbanBlockPosition( &urbanObject ) ); 
        }
        virtual void VisitCity( const urban::TerrainObject_ABC& /*urbanObject*/ )
        {
            isInCity_ = true;
        }
        virtual void VisitDistrict( const urban::TerrainObject_ABC& /*urbanObject*/ )
        {
            //NOTHING
        }
        std::auto_ptr< UrbanBlockPosition_ABC > GetDelegator(){ return delegate_; }
    private:
        bool& isInCity_;
        const urban::TerrainObject_ABC*& currentUrbanObject_;
        std::auto_ptr< UrbanBlockPosition_ABC > delegate_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::NotifyMoveInside
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::NotifyMovingInsideUrbanBlock( const urban::TerrainObject_ABC& urbanObject )
{
    UrbanMovingInsideVisitor visitor( isInCity_, urbanObject_, delegate_ );
    urbanObject.Accept( visitor );
    delegate_ = visitor.GetDelegator();
}

namespace
{
    class UrbanMovingOutsideVisitor : public urban::TerrainObjectVisitor_ABC
    {
    public:
        UrbanMovingOutsideVisitor( bool& isInCity, const urban::TerrainObject_ABC*& urbanObject, std::auto_ptr< UrbanBlockPosition_ABC > delegator ) : isInCity_( isInCity ), currentUrbanObject_( urbanObject ), delegate_( delegator )
        {}
        ~UrbanMovingOutsideVisitor()
        {}
        void VisitBlock( const urban::TerrainObject_ABC& urbanObject )
        {
            if( currentUrbanObject_ != &urbanObject )
                throw std::exception( "urbanObject in PHY_RolePion_UrbanLocation not defined" );
            currentUrbanObject_ = 0;
            delegate_.reset( new OutsideUrbanBlockPosition() );
        }
        void VisitCity( const urban::TerrainObject_ABC& /*urbanObject*/ )
        {
            isInCity_ = false;
        }
        void VisitDistrict( const urban::TerrainObject_ABC& /*urbanObject*/ )
        {
            //NOTHING
        }
        std::auto_ptr< UrbanBlockPosition_ABC > GetDelegator(){ return delegate_; }
    private:
        bool isInCity_;
        const urban::TerrainObject_ABC*& currentUrbanObject_;
        std::auto_ptr< UrbanBlockPosition_ABC > delegate_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_UrbanLocation::NotifyMoveOutside
// Created: SLG 2010-04-08
// -----------------------------------------------------------------------------
void PHY_RolePion_UrbanLocation::NotifyMovingOutsideUrbanBlock( const urban::TerrainObject_ABC& urbanObject )
{
    UrbanMovingOutsideVisitor visitor( isInCity_, urbanObject_, delegate_ );
    urbanObject.Accept( visitor );
    delegate_ = visitor.GetDelegator();
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
void PHY_RolePion_UrbanLocation::Execute( posture::PostureComputer_ABC& algorithm ) const
{
    if ( urbanObject_ )
    {
        double timeModifier = urbanObject_->ComputeComplexity();
        algorithm.AddUrbanCoefficientModifier( timeModifier );
    }
}