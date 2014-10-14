// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Itinerary.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/Protocol.h"

using namespace gui;
using namespace kernel;

namespace
{
    Itinerary::Point MakePoint( const geometry::Point2f& where, const boost::optional< uint32_t >& waypoint )
    {
        const Itinerary::Point p = { where, waypoint };
        return p;
    }

    Itinerary::T_Points GetPoints( const CoordinateConverter_ABC& converter, const sword::PathResult& pathfind )
    {
        Itinerary::T_Points result;
        const auto& points = pathfind.points();
        for( auto it = points.begin(); it != points.end(); ++it )
        {
            boost::optional< uint32_t > waypoint;
            if( it->has_waypoint() && it->waypoint() >= 0 )
                waypoint = it->waypoint();
            result.push_back( MakePoint( converter.ConvertToXY( it->coordinate() ), waypoint ) );
        }
        return result;
    }

    sword::PathResult GetPoints( const CoordinateConverter_ABC& converter, const Itinerary::T_Points& points )
    {
        sword::PathResult result;
        for( auto it = points.begin(); it != points.end(); ++it )
        {
            auto point = result.add_points();
            if( it->waypoint )
                point->set_waypoint( *it->waypoint );
            const auto src = converter.ConvertToGeo( it->where );
            auto dst = point->mutable_coordinate();
            dst->set_longitude( src.X() );
            dst->set_latitude( src.Y() );
        }
        return result;
    }

    Itinerary::T_Points GetWaypoints( const Itinerary::T_Points& points )
    {
        Itinerary::T_Points result;
        for( auto it = points.begin(); it != points.end(); ++it )
            if( it->waypoint )
                result.push_back( *it );
        return result;
    }
}

Itinerary::Itinerary( const kernel::CoordinateConverter_ABC& converter, const sword::Pathfind& pathfind, bool edition )
    : converter_( converter )
    , path_( GetPoints( converter, pathfind.result() ) )
    , waypoints_( edition ? GetWaypoints( path_ ) : T_Points() )
{
    // NOTHING
}

namespace
{
    const int size = 20;

    void SetColor( QColor color, bool picking, const GLView_ABC& view )
    {
        if( !picking )
            glColor4d( color.redF(), color.greenF(), color.blueF(), view.GetCurrentAlpha() );
    }

    void DrawDisc( QPainter& p, QColor contour, QColor inside, float radius )
    {
        p.setPen( QPen( contour, 2 ) );
        p.setBrush( QBrush( inside ) );
        p.drawEllipse( QPointF( size / 2.f, size / 2.f ), radius, radius );
    }

    QImage MakeBitmap( QColor disc, QColor circle )
    {
        QPixmap pm( size, size );
        pm.fill( Qt::transparent );
        QPainter p( &pm );
        p.setRenderHint( QPainter::Antialiasing, true );
        DrawDisc( p, disc, disc, 5 );
        DrawDisc( p, circle, disc, 4 );
        return pm.convertToImage().mirror();
    }

    QColor GetCurrent()
    {
        GLfloat rgba[ 4 ];
        glGetFloatv( GL_CURRENT_COLOR, rgba );
        return QColor( static_cast< int >( floorf( rgba[0] * UINT8_MAX ) ),
                       static_cast< int >( floorf( rgba[1] * UINT8_MAX ) ),
                       static_cast< int >( floorf( rgba[2] * UINT8_MAX ) ),
                       static_cast< int >( floorf( rgba[3] * UINT8_MAX ) ) );
    }
}

void Itinerary::Draw( GLView_ABC& view, const boost::optional< Hover >& hover, bool picking ) const
{
    const QColor current = GetCurrent();
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT );
    glEnable( GL_ALPHA_TEST );
    glAlphaFunc( GL_GREATER, 0.1f );
    SetColor( current.darker(), picking, view );
    DrawLines( view, 5 );
    SetColor( current, picking, view );
    DrawLines( view, 3 );
    DrawPoints( view, current, hover, picking );
    glDisable( GL_ALPHA_TEST );
    glPopAttrib();
}

void Itinerary::DrawLines( GLView_ABC& view, float width ) const
{
    if( path_.empty() )
        return;
    auto last = path_.begin();
    view.DrawDisc( last->where, width / 2, GLView_ABC::pixels );
    for( auto it = last + 1; it != path_.end(); last = it++ )
    {
        view.DrawLine( last->where, it->where, width );
        view.DrawDisc( it->where, width / 2, GLView_ABC::pixels );
    }
}

void Itinerary::DrawPoints( GLView_ABC& view, const QColor& current, const boost::optional< Hover >& hover, bool picking ) const
{
    const bool selected = view.ShouldDisplay();
    const auto& waypoints = waypoints_.empty() ? path_ : waypoints_;
    std::size_t i = 0;
    for( auto it = waypoints.begin(); it != waypoints.end(); ++it )
    {
        if( !it->waypoint )
            continue;
        DrawPoint( view, it->where, current, picking, selected ||
                 ( hover && hover->onWaypoint_ && hover->lastWaypoint_ == i ) );
        ++i;
    }
    if( hover && !hover->onWaypoint_ && hover->coordinate_ )
        DrawPoint( view, *hover->coordinate_, current, false, true );
}

void Itinerary::DrawPoint( GLView_ABC& view, geometry::Point2f p, const QColor& current, bool picking, bool highlight ) const
{
    static const QImage normal = MakeBitmap( Qt::white, Qt::black );
    static const QImage highlighted = MakeBitmap( Qt::black, Qt::white );
    const QImage image = picking ? MakeBitmap( current, current ) : highlight ? highlighted : normal;
    const float offset = size * view.Pixels( p ) / 2;
    view.DrawImage( image, geometry::Point2f( p.X() - offset, p.Y() - offset ) );
}

void Itinerary::LoadPoints( const sword::PathResult& path )
{
    path_ = GetPoints( converter_, path );
    waypoints_ = GetWaypoints( path_ );
}

void Itinerary::Clear()
{
    path_.clear();
    waypoints_.clear();
}

void Itinerary::StartOn( const geometry::Point2f& where )
{
    const auto point = MakePoint( where, 0u );
    if( waypoints_.size() > 1 )
        waypoints_.front() = point;
    else
        waypoints_.push_front( point );
}

void Itinerary::StopOn( const geometry::Point2f& where )
{
    const auto point = MakePoint( where, 0u );
    if( waypoints_.size() > 1 )
        waypoints_.back() = point;
    else
        waypoints_.push_back( point );
}

bool Itinerary::HasPath() const
{
    return !path_.empty();
}

bool Itinerary::HasWaypoints() const
{
    return waypoints_.size() > 1;
}

std::vector< geometry::Point2f > Itinerary::GetDots() const
{
    std::vector< geometry::Point2f > dots;
    dots.reserve( waypoints_.size() );
    for( auto it = waypoints_.begin(); it != waypoints_.end(); ++it )
        dots.push_back( it->where );
    return dots;
}

namespace
{
    bool IsNear( const GLView_ABC& view, float squareDistance, const geometry::Point2f& point )
    {
        const auto pixels = view.Pixels( point );
        static const auto threshold = 200; // pixels
        return squareDistance < threshold * pixels * pixels;
    }
}

boost::optional< Itinerary::Hover > Itinerary::PickWaypoint( const GLView_ABC& view, const geometry::Point2f& where ) const
{
    // path iterated backwards to select the waypoint on top
    // when several of them overlap
    for( auto i = waypoints_.size(); i > 0; --i )
    {
        const auto& position = waypoints_[ i - 1 ];
        if( !IsNear( view, where.SquareDistance( position.where ), where ) )
            continue;
        const Hover hover = {
            where,
            position.where,
            i - 1,
            true
        };
        return hover;
    }
    return boost::none;
}

boost::optional< Itinerary::Hover > Itinerary::PickSegment( const GLView_ABC& view, const geometry::Point2f& where ) const
{
    size_t waypoint = 0;
    float distance = std::numeric_limits< float >::infinity();
    auto last = path_.begin();
    boost::optional< Itinerary::Hover > best;
    for( auto it = last + 1; it != path_.end(); last = it++ )
    {
        if( last->waypoint )
            waypoint = *last->waypoint;
        const auto projection = geometry::Segment2f( it->where, last->where ).Project( where );
        const auto d = where.SquareDistance( projection );
        if( d >= distance || !IsNear( view, d, where ) )
            continue;
        distance = d;
        const Hover hover = {
            projection,
            boost::none,
            waypoint,
            false
        };
        best = hover;
    }
    return best;
}

void Itinerary::InsertWaypoint( size_t idx, const geometry::Point2f where )
{
    waypoints_.insert( waypoints_.begin() + idx, MakePoint( where, 0u ) );
}

void Itinerary::EraseWaypoint( size_t idx )
{
    waypoints_.erase( waypoints_.begin() + idx );
}

sword::PathResult Itinerary::GetPathfind() const
{
    return GetPoints( converter_, path_ );
}

geometry::Point2f Itinerary::GetPosition() const
{
    if( path_.empty() )
        return geometry::Point2f();
    return path_.front().where;
}
