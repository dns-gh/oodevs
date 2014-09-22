// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "WeaponRangeLayer.h"
#include "gaming/Weapons.h"
#include "clients_gui/Viewport2d.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Options.h"
#include "MT_Tools/MT_Logger.h"
#include <terrain/GeosUtil.h>
#include <terrain/GraphicData.h>
#include <terrain/ShapeTesselator.h>
#include <terrain/TesselatedShape.h>
#include <boost/lexical_cast.hpp>

using namespace terrain;

namespace
{

terrain::T_PointVector MakeCircle( geometry::Point2d center, uint32_t radius )
{
    terrain::T_PointVector points;
    const double step = std::acos( -1.f ) / 20.f + 1e-7f;
    const double twoPi = 2*std::acos( -1.f );
    for( double angle = 0; angle < twoPi; angle += step )
        points.push_back( geometry::Point2d(
            center.X() + radius*std::cos( angle ),
            center.Y() + radius*std::sin( angle ) ) );
    points.push_back( geometry::Point2d(
        center.X() + radius*std::cos( twoPi ),
        center.Y() + radius*std::sin( twoPi ) ) );
    return points;
}

// Returns a donut of maxRange outer radius and minRange inner radius. If
// minRange is zero, returns a disk.
GeosGeomPtr MakeDonut( const GeosContextPtr& h, geometry::Point2d center,
        uint32_t minRange, uint32_t maxRange )
{
    const auto outer = MakeCircle( center, maxRange );
    T_Rings inner;
    if( minRange > 0 )
        inner.push_back( MakeCircle( center, minRange ) );
    return CreateMultiPolygon( h, outer, inner );
}

// Returns the union of all donuts where the i-th donut is defined by
// centers[i] and ranges[i] minRange and maxRange.
GeosGeomPtr MergeDonuts( const GeosContextPtr& h,
        const std::vector< geometry::Point2f >& centers,
        const std::vector< std::pair< uint32_t, uint32_t > >& ranges )
{
    std::vector< GeosGeomPtr > donuts;
    donuts.reserve( centers.size() );
    for( size_t i = 0; i < centers.size(); ++i )
    {
        const auto range = ranges.at( i );
        const auto center = geometry::Point2d( centers[i].X(), centers[i].Y() );
        const auto donut = MakeDonut( h, center, range.first, range.second );
        donuts.push_back( donut );
    }
    return UnaryUnion( h, donuts );
}

typedef std::vector< TesselatedShapePtr > T_TesselatedShapes;

TesselatedShapePtr TesselateOne( const T_Polygon& polygon )
{
    ShapeTesselator tesselator;
    tesselator.BeginArea( *new GraphicData(), "" );
    tesselator.AddContour( polygon.first );
    for( auto it = polygon.second.begin(); it != polygon.second.end(); ++it )
        tesselator.AddContour( *it );
    return tesselator.EndTesselation().GetShape();
}

// Turns a collection of generic geometries into a collection of tessellated
// ones suitable for OpenGL rendering (no holes, possibly triangulated).
T_TesselatedShapes Tesselate( const GeosContextPtr& h, const GeosGeomPtr& geometries )
{
    T_TesselatedShapes tesselated;
    T_MultiPolygon collection;
    if( !ExtractMultiPolygon( h, geometries, collection ) )
        return tesselated;
    for( auto it = collection.begin(); it != collection.end(); ++it )
    {
        // We could bypass tesselation for polygons without holes but it is
        // just simpler to process all elements the same way.
        const auto result = TesselateOne( *it );
        if( result )
            tesselated.push_back( result );
    }
    return tesselated;
}

}  // namespace

WeaponRangeLayer::WeaponRangeLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                    gui::View_ABC& view, const kernel::Profile_ABC& profile )
    : gui::EntityLayerBase( controllers, tools, strategy, view, profile, eLayerTypes_WeaponRanges )
    , controllers_( controllers )
    , strategy_( strategy )
    , useColor_( false )
{
    controllers_.Register( *this );
}

WeaponRangeLayer::~WeaponRangeLayer()
{
    controllers_.Unregister( *this );
}

void WeaponRangeLayer::NotifyCreated( const kernel::Agent_ABC& entity )
{
    AddEntity( entity );
}

void WeaponRangeLayer::NotifyDeleted( const kernel::Agent_ABC& entity )
{
    RemoveEntity( entity );
}

void WeaponRangeLayer::Paint( const geometry::Rectangle2f& extent )
{
    if( !ShouldDrawPass() )
        return;
    auto ranges = ranges_;
    auto positions = positions_;
    ranges_.clear();
    positions_.clear();
    gui::Viewport2d viewport( extent );
    // Will call Draw with each entity and refill ranges_ and positions_.
    EntityLayerBase::Paint( viewport );
    if( ranges != ranges_ || positions_ != positions )
        Reset();
    for( auto it = tesselated_.begin(); it != tesselated_.end(); ++it )
    {
        const auto color = QColor( it->first );
        glPushAttrib( GL_CURRENT_BIT );
        glColor4d( color.redF(), color.greenF(), color.blueF(), 0.5 );
        for( auto is = it->second.begin(); is != it->second.end(); ++is )
            (*is)->Draw( extent );
        glPopAttrib();
    }
}

void WeaponRangeLayer::Reset()
{
    tesselated_.clear();
    const auto h = InitGeos( false );
    for( auto it = ranges_.begin(); it != ranges_.end(); ++it )
    {
        const auto positions = positions_.find( it->first );
        if( positions == positions_.end()
                || positions->second.size() != it->second.size() )
            continue;
        const auto donuts = MergeDonuts( h, positions->second, it->second );
        tesselated_[ it->first ] = Tesselate( h, donuts );
    }
}

void WeaponRangeLayer::Draw( const kernel::Entity_ABC& entity, gui::Viewport_ABC& /*viewport*/, bool /*pickingMode*/ )
{
    if( !ShouldDisplay( entity ) || !tools_.ShouldDisplay( type_ ) )
        return;
    // SelectColor actually controls the result of ShouldDisplay
    strategy_.SelectColor( static_cast< const kernel::Agent_ABC& >( entity ) );
    if( !tools_.ShouldDisplay( "WeaponRanges" ) )
        return;
    if( const Weapons* weapons = entity.Retrieve< Weapons >() )
    {
        const geometry::Point2f position = GetPosition( entity );
        weapons->DrawEfficientRange( position, tools_ );
        if( weapons->GetMaxRange() <= 0 )
            return;
        const auto color = useColor_ ? color_ : strategy_.FindColor( entity );
        ranges_[ color.name() ].push_back(
                std::make_pair( weapons->GetMinRange(), weapons->GetMaxRange() ) );
        positions_[ color.name() ].push_back( position );
    }
}

void WeaponRangeLayer::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "EfficientRangeUseCustomColor" )
        useColor_ = value.To< bool >();
    else if( name == "EfficientRangeCustomColor" )
    {
        color_.setNamedColor( value.To< QString >() );
        if( useColor_ )
        {
            ranges_.clear();
            positions_.clear();
        }
    }
}
