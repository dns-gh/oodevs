// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
 
#include "astec_gaming_pch.h"

#include "Surface.h"
#include "Net_Def.h"
#include "astec_kernel/SensorType.h"
#include "astec_kernel/DetectionMap.h"
#include "VisionLine.h"
#include "VisionMap.h"
#include "astec_kernel/Positions.h"
#include "astec_kernel/Agent_ABC.h"
#include "astec_kernel/GlTools_ABC.h"

using namespace geometry;

// -----------------------------------------------------------------------------
// Name: Surface constructor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
Surface::Surface( const Agent_ABC& agent, const VisionConesMessage& input, const DetectionMap& map, const Resolver_ABC< SensorType, std::string >& resolver )
    : agent_( agent )
    , map_( map )
    , elongation_( 1 )
    , distanceModificator_( 1 )
{
    double oX, oY, rHeight;
    input >> oX >> oY; origin_ = Point2f( float( oX ), float( oY ) );
    input >> rHeight; height_ = float( rHeight );

    std::string strTypeName;
    input >> strTypeName;

    pSensorType_ = & resolver.Get( strTypeName );
 
    unsigned long nNbrSectors;
    input >> nNbrSectors;
    sectors_.reserve( nNbrSectors );
    for( uint i = 0; i < nNbrSectors; ++i )
    {
        double x, y;
        input >> x >> y;
        sectors_.push_back( Sector( origin_, Vector2f( float(x), float(y) ), pSensorType_->GetAngle() ) );
    }
    distanceModificator_ = elongation_ * pSensorType_->GetDistanceModificator( agent_ );
    maxRadius_ = pSensorType_->GetMaxDistance( distanceModificator_ );
}

// -----------------------------------------------------------------------------
// Name: Surface::SetElongation
// Created: AGE 2006-04-27
// -----------------------------------------------------------------------------
void Surface::SetElongation( float elongation )
{
    elongation_ = elongation;
    distanceModificator_ = elongation_ * pSensorType_->GetDistanceModificator( agent_ );
    maxRadius_ = pSensorType_->GetMaxDistance( distanceModificator_ );
}

// -----------------------------------------------------------------------------
// Name: Surface destructor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
Surface::~Surface()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Surface::Draw
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void Surface::Draw( const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.Intersect( Extent() ).IsEmpty() )
        return;

    for( CIT_SectorVector itSector = sectors_.begin(); itSector != sectors_.end(); ++itSector )
        itSector->Draw( viewport, tools, maxRadius_ );
}

// -----------------------------------------------------------------------------
// Name: Surface::Update
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void Surface::Update( VisionMap& map )
{
    Rectangle2< unsigned > extent = MappedExtent();
    for( unsigned i = extent.Left(); i <= extent.Right(); ++i )
        for( unsigned j = extent.Bottom(); j < extent.Top(); ++j )
        {
            const Point2f point = map_.Map( i, j );
            const std::pair< unsigned, unsigned > mappedPoint( i, j );
            if( IsInSector( point ) && map.ShouldUpdate( mappedPoint ) )
            {
                E_PerceptionResult perception = ComputePerception( point );
                map.Update( mappedPoint, perception );
            }
        };
}

// -----------------------------------------------------------------------------
// Name: Surface::Extent
// Created: AGE 2006-04-14
// -----------------------------------------------------------------------------
Rectangle2f Surface::Extent() const
{
    return Rectangle2f( origin_.X() - maxRadius_, origin_.Y() - maxRadius_,     
                        origin_.X() + maxRadius_, origin_.Y() + maxRadius_ ).Intersect( map_.Extent() );
}
    
// -----------------------------------------------------------------------------
// Name: Rectangle2< unsigned > Surface::MappedExtent
// Created: AGE 2006-04-14
// -----------------------------------------------------------------------------
Rectangle2< unsigned > Surface::MappedExtent() const
{
    Rectangle2f extent = Extent();
    std::pair< unsigned, unsigned > bl = map_.Unmap( extent.BottomLeft() );
    std::pair< unsigned, unsigned > tr = map_.Unmap( extent.TopRight() );
    return Rectangle2< unsigned >( bl.first, bl.second, tr.first, tr.second );
}

// -----------------------------------------------------------------------------
// Name: Surface::IsInSector
// Created: AGE 2006-04-14
// -----------------------------------------------------------------------------
bool Surface::IsInSector( const geometry::Point2f& point ) const
{
    for( CIT_SectorVector it = sectors_.begin(); it != sectors_.end(); ++it )
        if( it->IsInCone( point, maxRadius_ ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: Surface::ComputePerception
// Created: AGE 2006-04-14
// -----------------------------------------------------------------------------
E_PerceptionResult Surface::ComputePerception( const geometry::Point2f& point ) const
{
    distanceModificator_ = elongation_ * pSensorType_->GetDistanceModificator( agent_ );
    maxRadius_ = pSensorType_->GetMaxDistance( distanceModificator_ );

    VisionLine line( map_, origin_, point, height_ + agent_.Get< Positions >().GetHeight() );
    float skyrock = std::numeric_limits< float >::infinity();
    while( ! line.IsDone() && skyrock > 0 )
    {
        line.Increment();
        if( skyrock == std::numeric_limits< float >::infinity() )
            skyrock = pSensorType_->ComputeExtinction( distanceModificator_,
                line.IsInForest(), line.IsInTown(), line.IsInGround(), line.Length() );
        else
            skyrock = pSensorType_->ComputeExtinction( distanceModificator_, skyrock,
                line.IsInForest(), line.IsInTown(), line.IsInGround(), line.Length() );
    }
    return pSensorType_->InterpreteNRJ( skyrock );
}
