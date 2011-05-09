// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"

#include "Surface.h"
#include "VisionMap.h"
#include "clients_kernel/SensorType.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_gui/VisionLine.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/Protocol.h"
#include "UrbanModel.h"
#include "UrbanBlockDetectionMap.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Surface constructor
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
Surface::Surface( const Agent_ABC& agent, const sword::VisionCone& message, const kernel::CoordinateConverter_ABC& converter, const DetectionMap& map, const tools::Resolver_ABC< SensorType, std::string >& resolver, float elongation, const UrbanBlockDetectionMap& urbanModelMap )
    : map_( map )
    , origin_( converter.ConvertToXY( message.origin() ) )
    , height_( message.height() + agent.Get< Positions >().GetHeight() )
    , sensorType_( resolver.Get( message.sensor() ) )
    , elongation_( elongation )
    , distanceModificator_( 1 )
    , urbanModelMap_( urbanModelMap )
{
    sectors_.reserve( message.directions().elem_size() );
    for( int i = 0; i < message.directions().elem_size(); ++i )
        sectors_.push_back( Sector( origin_, message.directions().elem( i ).heading(), sensorType_.GetAngle() ) );
    distanceModificator_ = elongation_ * sensorType_.GetDistanceModificator( agent );
    maxRadius_ = sensorType_.GetMaxDistance( distanceModificator_ );
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
void Surface::Draw( const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( ! viewport.IsVisible( Extent() ) )
        return;
    for( CIT_SectorVector itSector = sectors_.begin(); itSector != sectors_.end(); ++itSector )
        itSector->Draw( tools, maxRadius_ );
}

// -----------------------------------------------------------------------------
// Name: Surface::DrawFill
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void Surface::DrawFill( const kernel::Viewport_ABC& viewport ) const
{
    if( ! viewport.IsVisible( Extent() ) )
        return;
    for( CIT_SectorVector itSector = sectors_.begin(); itSector != sectors_.end(); ++itSector )
        itSector->DrawFill( maxRadius_ );
}

// -----------------------------------------------------------------------------
// Name: Surface::Initialize
// Created: AGE 2007-02-01
// -----------------------------------------------------------------------------
void Surface::Initialize( VisionMap& map )
{
    map.Incorporate( MappedExtent() );
}

// -----------------------------------------------------------------------------
// Name: Surface::Update
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void Surface::Update( VisionMap& map )
{
    Rectangle2< int > extent = MappedExtent();
    for( int i = extent.Left(); i <= extent.Right(); ++i )
        for( int j = extent.Bottom(); j < extent.Top(); ++j )
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
Rectangle2< int > Surface::MappedExtent() const
{
    Rectangle2f extent = Extent();
    std::pair< unsigned, unsigned > bl = map_.Unmap( extent.BottomLeft() );
    std::pair< unsigned, unsigned > tr = map_.Unmap( extent.TopRight() );
    return Rectangle2< int >( bl.first, bl.second, tr.first, tr.second );
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
    gui::VisionLine line( map_, origin_, point, height_ );
    float skyrock = std::numeric_limits< float >::infinity();
    while( ! line.IsDone() && skyrock > 0 )
    {
        line.Increment();
        if( skyrock == std::numeric_limits< float >::infinity() )
            skyrock = sensorType_.ComputeExtinction( distanceModificator_,
                line.IsInForest(), line.IsInTown(), line.IsInGround(), line.Length(), urbanModelMap_.GetEnvironment( line.CurrentPoint() ) );
        else
            skyrock = sensorType_.ComputeExtinction( distanceModificator_, skyrock,
                line.IsInForest(), line.IsInTown(), line.IsInGround(), line.Length(), urbanModelMap_.GetEnvironment( line.CurrentPoint() ) );
    }
    return sensorType_.InterpreteNRJ( skyrock );
}
