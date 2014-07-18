// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GridLayer.h"
#include "GlTools_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Options.h"
#include <boost/lexical_cast.hpp>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: GridLayer constructor
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
GridLayer::GridLayer( Controllers& controllers, const GlTools_ABC& tools, const CoordinateConverter_ABC& converter )
    : controllers_( controllers )
    , tools_( tools )
    , converter_( converter )
    , gridType_( eCoordinateSystem_Local )
    , gridSize_( 10000 ) // meters
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: GridLayer destructor
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
GridLayer::~GridLayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GridLayer::Paint
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void GridLayer::Paint( const geometry::Rectangle2f& v )
{
    if( !ShouldDrawPass() )
        return;
    if( gridSize_ <= 0 )
        return;
    const geometry::Rectangle2f viewport = v.Intersect( extent_ );
    if( viewport.IsEmpty() )
        return;
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
        glColor4f( 1.0f, 1.0f, 1.0f, 0.3f );
        glLineWidth( 1.0 );
        if( gridType_ != eCoordinateSystem_Mgrs )
        {
            const int size = GetSize();
            for( float x = Snap( viewport.Left() ); x < viewport.Right(); x += size )
                tools_.DrawLine( geometry::Point2f( x, viewport.Top() ),
                                 geometry::Point2f( x, viewport.Bottom() ) );
            for( float y = Snap( viewport.Bottom() ); y < viewport.Top(); y += size )
                tools_.DrawLine( geometry::Point2f( viewport.Left(), y ),
                                 geometry::Point2f( viewport.Right(), y ) );
        }
        else
            DrawSquares( viewport );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: GridLayer::Paint
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void GridLayer::Paint( Viewport_ABC& )
{
    // NOTHING
}

int GridLayer::GetSize() const
{
    return static_cast< int >( std::max( gridSize_, 2 * tools_.Pixels() ) );
}

// -----------------------------------------------------------------------------
// Name: GridLayer::Snap
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
float GridLayer::Snap( float value ) const
{
    return std::floor( value / gridSize_ ) * gridSize_;
}

// -----------------------------------------------------------------------------
// Name: GridLayer::OptionChanged
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void GridLayer::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "GridSize" )
        gridSize_ = value.To< float >() * 1000;
    else if( name == "GridType" )
        gridType_ = static_cast< E_CoordinateSystem >( value.To< int >() );
}

void GridLayer::DrawSquares( const geometry::Rectangle2f& viewport ) const
{
    for( auto it = squares_.begin(); it != squares_.end(); ++it )
    {
        DrawLine( viewport, it->bottomLeft_.coord_, it->topLeft_.coord_ );
        DrawLine( viewport, it->topLeft_.coord_, it->topRight_.coord_ );
        DrawVerticalSides( viewport, *it );
        DrawHorizontalSides( viewport, *it );
    }
}

namespace
{
    // Computes the intersection between a rectangle and a segment defined by
    // two points. The result can be any of 0, 1 or 2 points.
    std::vector< geometry::Point2f > Intersect(
        const geometry::Rectangle2f& r,
        const geometry::Point2f& p1, const geometry::Point2f& p2 )
    {
        std::vector< geometry::Point2f > result;
        const geometry::Segment2f s( p1, p2 );
        const geometry::Point2f topLeft( r.Left(), r.Top() );
        geometry::Point2f p;
        if( geometry::Segment2f( r.BottomLeft(), topLeft ).Intersect( s, p ) )
            result.push_back( p );
        if( geometry::Segment2f( topLeft, r.TopRight() ).Intersect( s, p ) )
            result.push_back( p );
        const geometry::Point2f bottomRight( r.Right(), r.Bottom() );
        if( geometry::Segment2f( r.TopRight(), bottomRight ).Intersect( s, p ) )
            result.push_back( p );
        if( geometry::Segment2f( bottomRight, r.BottomLeft() ).Intersect( s, p ) )
            result.push_back( p );
        return result;
    }
}

void GridLayer::DrawLine( const geometry::Rectangle2f& viewport,
    const geometry::Point2f& from, const geometry::Point2f& to ) const
{
    const bool fromOutside = viewport.IsOutside( from );
    const bool toOutside = viewport.IsOutside( to );
    if( !fromOutside && !toOutside )
        tools_.DrawLine( from, to );
    else if( !fromOutside && toOutside )
        tools_.DrawLine( from, Intersect( viewport, from, to )[ 0 ] );
    else if( fromOutside && !toOutside )
        tools_.DrawLine( Intersect( viewport, to, from )[ 0 ], to );
    else
    {
        const auto points = Intersect( viewport, to, from );
        if( points.size() > 1 )
            tools_.DrawLine( points[ 0 ], points[ 1 ] );
    }
}

namespace
{
    int Next( int i, int size )
    {
        return ( i / size + 1 ) * size;
    }

    template< typename T >
    geometry::Point2f MapX( const T& p1, const T& p2, int x )
    {
        const geometry::Point2d P1( p1.x_, p1.y_ );
        const geometry::Point2d P2( p2.x_, p2.y_ );
        return p1.coord_ + geometry::Vector2f( p1.coord_, p2.coord_ ) * ( x - p1.x_ ) / ( p2.x_ - p1.x_ );
    }

    template< typename T >
    geometry::Point2f MapY( const T& p1, const T& p2, int y )
    {
        const geometry::Point2d P1( p1.x_, p1.y_ );
        const geometry::Point2d P2( p2.x_, p2.y_ );
        return p1.coord_ + geometry::Vector2f( p1.coord_, p2.coord_ ) * ( y - p1.y_ ) / ( p2.y_ - p1.y_ );
    }
}

void GridLayer::DrawVerticalSides( const geometry::Rectangle2f& viewport, const Square& s ) const
{
    const int size = GetSize();
    const int start = Next( std::min( s.bottomLeft_.x_, s.topLeft_.x_ ), size );
    const int mid1 = Next( std::max( s.bottomLeft_.x_, s.topLeft_.x_ ), size );
    const int mid2 = Next( std::min( s.bottomRight_.x_, s.topRight_.x_ ), size );
    const int end = std::max( s.bottomRight_.x_, s.topRight_.x_ );
    for( int i = start; i < mid1; i += size )
        DrawLine( viewport,
            s.bottomLeft_.x_ < s.topLeft_.x_ ?
                MapX( s.bottomLeft_, s.bottomRight_, i ) :
                MapX( s.topLeft_, s.topRight_, i ),
            MapX( s.bottomLeft_, s.topLeft_, i ) );
    for( int i = mid1; i < mid2; i += size )
        DrawLine( viewport,
            MapX( s.bottomLeft_, s.bottomRight_, i ),
            MapX( s.topLeft_, s.topRight_, i ) );
    for( int i = mid2; i < end; i += size )
        DrawLine( viewport,
            s.topRight_.x_ > s.bottomRight_.x_ ?
                MapX( s.topLeft_, s.topRight_, i ) :
                MapX( s.bottomLeft_, s.bottomRight_, i ),
            MapX( s.topRight_, s.bottomRight_, i ) );
}

void GridLayer::DrawHorizontalSides( const geometry::Rectangle2f& viewport, const Square& s ) const
{
    const int size = GetSize();
    const int start = Next( std::min( s.bottomLeft_.y_, s.bottomRight_.y_ ), size );
    const int mid1 = Next( std::max( s.bottomLeft_.y_, s.bottomRight_.y_ ), size );
    const int mid2 = Next( std::min( s.topLeft_.y_, s.topRight_.y_ ), size );
    const int end = std::max( s.topLeft_.y_, s.topRight_.y_ );
    for( int i = start; i < mid1; i += size )
        DrawLine( viewport,
            MapY( s.bottomLeft_, s.bottomRight_, i ),
            s.bottomLeft_.y_ < s.bottomRight_.y_ ?
                MapY( s.bottomLeft_, s.topLeft_, i ) :
                MapY( s.bottomRight_, s.topRight_, i ) );
    for( int i = mid1; i < mid2; i += size )
        DrawLine( viewport,
            MapY( s.bottomRight_, s.topRight_, i ),
            MapY( s.bottomLeft_, s.topLeft_, i ) );
    for( int i = mid2; i < end; i += size )
        DrawLine( viewport,
            MapY( s.topRight_, s.topLeft_, i ),
            s.topRight_.y_ > s.topLeft_.y_ ?
                MapY( s.topRight_, s.bottomRight_, i ) :
                MapY( s.topLeft_, s.bottomLeft_, i ) );
}

namespace
{
    // "Zone 1 covers longitude 180 to 174 W; zone numbering increases eastward to zone 60 that covers longitude 174 to 180 East."
    const int zoneWidth = 6;
    const int zoneOrigin = -180;
    // "Each latitude band is 8 degrees high, and is lettered starting from "C" at 80 S"
    const int bandHeight = 8;
    const int bandOrigin = -80;
}

boost::optional< std::string > GridLayer::Convert( double longitude, double latitude, const std::string& prefix, int x, int y ) const
{
    try
    {
        char coordinates[ 11 ];
        sprintf( coordinates, "%.5i%.5i", x, y );
        const std::string mgrs( prefix + coordinates );
        const auto latlong = converter_.ConvertToGeo( mgrs );
        if( longitude <= latlong.X() && latlong.X() < longitude + zoneWidth &&
            latitude <= latlong.Y() && latlong.Y() < latitude + bandHeight )
            return mgrs;
        return boost::none;
    }
    catch( ... )
    {
        return boost::none;
    }
}

void GridLayer::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    extent_ = geometry::Rectangle2f();
}

template< typename C >
GridLayer::Point GridLayer::Bisect( int good, int bad, const C& convert ) const
{
    boost::optional< std::string > result;
    while( std::abs( good - bad ) > 1 )
    {
        const int test = good + ( bad - good ) / 2;
        if( const auto p = convert( test ) )
        {
            good = test;
            result = p;
        }
        else
            bad = test;
    }
    return MakePoint( *result );
}

namespace
{
    // "The identification consists of a column letter (A-Z, omitting I and O) followed by a row letter (A-V, omitting I and O)"
    char NextColumn( char c )
    {
        ++c;
        if( c == 'Z' + 1 )
            c = 'A';
        else if( c == 'I' || c == 'O' )
            ++c;
        return c;
    }

    char NextRow( char c )
    {
        ++c;
        if( c == 'W' )
            c = 'A';
        else if( c == 'I' || c == 'O' )
            ++c;
        return c;
    }

    // the max value for a mgrs coordinate
    const int max = 99999;
    // used to move picking by a small margin into the zone to avoid "right on line" issues
    const double margin = 0.00001;
}

void GridLayer::NotifyUpdated( const kernel::ModelLoaded& )
{
    extent_ = converter_.GetExtent();
    squares_.clear();
    const geometry::Rectangle2d extent = ComputeBoundingExtent();
    for( auto longitude = std::floor( ( extent.Left() - zoneOrigin ) / zoneWidth ) * zoneWidth + zoneOrigin;
        longitude < extent.Right(); longitude += zoneWidth )
    {
        for( auto latitude = std::floor( ( extent.Bottom() - bandOrigin ) / bandHeight ) * bandHeight + bandOrigin;
            latitude < extent.Top(); latitude += bandHeight )
        {
            const auto topLeft = ExtractPrefix( extent, longitude + margin, latitude + bandHeight - margin );
            const auto bottomLeft = ExtractPrefix( extent, longitude + margin, latitude + margin );
            const auto bottomRight = ExtractPrefix( extent, longitude + zoneWidth - margin, latitude + margin );
            for( auto sq = bottomLeft; sq[4] != NextRow( topLeft[4] ); sq[4] = NextRow( sq[4] ) )
                for( auto s = sq; s[3] != NextColumn( bottomRight[3] ); s[3] = NextColumn( s[3] ) )
                {
                    const auto square = MakeSquare( s, longitude, latitude );
                    if( IsVisible( square ) )
                        squares_.push_back( square );
                }
        }
    }
}

geometry::Rectangle2d GridLayer::ComputeBoundingExtent() const
{
    const auto bottomLeft = converter_.ConvertToGeo( geometry::Point2f() );
    const auto topLeft = converter_.ConvertToGeo( geometry::Point2f( 0, extent_.Height() ) );
    const auto topRight = converter_.ConvertToGeo( geometry::Point2f( extent_.Width(), extent_.Height() ) );
    const auto bottomRight = converter_.ConvertToGeo( geometry::Point2f( extent_.Width(), 0 ) );
    geometry::Rectangle2d extent( bottomLeft, topRight );
    extent.Incorporate( topLeft );
    extent.Incorporate( bottomRight );
    return extent;
}

std::string GridLayer::ExtractPrefix( const geometry::Rectangle2d& extent, double longitude, double latitude ) const
{
    const std::string mgrs = converter_.ConvertToMgrs(
        converter_.ConvertFromGeo(
            geometry::Point2d(
                std::min( std::max( longitude, extent.Left() ), extent.Right() ),
                std::min( std::max( latitude, extent.Bottom() ), extent.Top() ) ) ) );
    return mgrs.substr( 0, 5 );
}

bool GridLayer::IsVisible( const Square& s ) const
{
    geometry::Polygon2f p;
    p.Add( s.topLeft_.coord_ );
    p.Add( s.topRight_.coord_ );
    p.Add( s.bottomRight_.coord_ );
    p.Add( s.bottomLeft_.coord_ );
    return !p.BoundingBox().Intersect( extent_ ).IsEmpty();
}

GridLayer::Square GridLayer::MakeSquare( const std::string& prefix, double longitude, double latitude ) const
{
    Square square;
    const auto bl = Convert( longitude, latitude, prefix, 0, 0 );
    const auto tl = Convert( longitude, latitude, prefix, 0, max );
    const auto tr = Convert( longitude, latitude, prefix, max, max );
    const auto br = Convert( longitude, latitude, prefix, max, 0 );
    // $$$$ MCO 2014-06-19: the following code has a few bugs:
    // $$$$ MCO 2014-06-19: . V shape squares (e.g. degenerated to triangles) are not handled properly
    // $$$$ MCO 2014-06-19:   (for instance 30UYE or 18FYF)
    // $$$$ MCO 2014-06-19: . pentagonal squares are not handled properly either
    // $$$$ MCO 2014-06-19:   (for instance 30UXE or 18FXF)
    if( bl )
    {
        square.bottomLeft_ = MakePoint( *bl );
        if( !br )
            square.bottomRight_ = Bisect( 0, max,
                [&]( int i ) { return Convert( longitude, latitude, prefix, i, 0 ); } );
        if( !tl && !tr )
            square.topLeft_ = Bisect( 0, max,
                [&]( int i ) { return Convert( longitude, latitude, prefix, 0, i ); } );
    }
    if( tl )
    {
        square.topLeft_ = MakePoint( *tl );
        if( !tr )
            square.topRight_ = Bisect( 0, max,
                [&]( int i ) { return Convert( longitude, latitude, prefix, i, max ); } );
        if( !bl && !br )
            square.bottomLeft_ = Bisect( max, 0,
                [&]( int i ) { return Convert( longitude, latitude, prefix, 0, i ); } );
    }
    if( tr )
    {
        square.topRight_ = MakePoint( *tr );
        if( !tl )
            square.topLeft_ = Bisect( max, 0,
                [&]( int i ) { return Convert( longitude, latitude, prefix, i, max ); } );
        if( !br && !bl )
            square.bottomRight_ = Bisect( max, 0,
                [&]( int i ) { return Convert( longitude, latitude, prefix, max, i ); } );
    }
    if( br )
    {
        square.bottomRight_ = MakePoint( *br );
        if( !bl )
            square.bottomLeft_ = Bisect( max, 0,
                [&]( int i ) { return Convert( longitude, latitude, prefix, i, 0 ); } );
        if( !tr && !tl )
            square.topRight_ = Bisect( 0, max,
                [&]( int i ) { return Convert( longitude, latitude, prefix, max, i ); } );
    }
    if( !br && !tl && !tr )
        square.topRight_ = MakePoint( longitude + zoneWidth - margin, latitude + bandHeight - margin );
    if( !tr && !bl && !br )
        square.bottomRight_ = MakePoint( longitude + zoneWidth - margin, latitude + margin );
    if( !tl && !br && !bl )
        square.bottomLeft_ = MakePoint( longitude + margin, latitude + margin );
    if( !bl && !tr && !tl )
        square.topLeft_ = MakePoint( longitude + margin, latitude + bandHeight - margin );
    return square;
}

GridLayer::Point GridLayer::MakePoint( double longitude, double latitude ) const
{
    return MakePoint( converter_.ConvertToMgrs( geometry::Point2d( longitude, latitude ) ) );
}

GridLayer::Point GridLayer::MakePoint( const std::string& mgrs ) const
{
    return Point( converter_.ConvertToXY( mgrs ),
        boost::lexical_cast< int >( mgrs.substr( 5, 5 ) ),
        boost::lexical_cast< int >( mgrs.substr( 10, 5 ) ) );
}
