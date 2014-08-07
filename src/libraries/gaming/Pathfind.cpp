// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Pathfind.h"
#include "actions/ActionsModel.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Equipments_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/SimpleHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace gui;

namespace
{
    Pathfind::Point MakePoint( const geometry::Point2f& where, const boost::optional< uint32_t >& waypoint )
    {
        const Pathfind::Point p = { where, waypoint };
        return p;
    }

    Pathfind::T_Points GetPoints( const CoordinateConverter_ABC& converter, const sword::PathResult& pathfind )
    {
        Pathfind::T_Points reply;
        const auto& points = pathfind.points();
        for( auto it = points.begin(); it != points.end(); ++it )
        {
            boost::optional< uint32_t > waypoint;
            if( it->has_waypoint() && it->waypoint() >= 0 )
                waypoint = it->waypoint();
            reply.push_back( MakePoint( converter.ConvertToXY( it->coordinate() ), waypoint ) );
        }
        return reply;
    }

    sword::PathResult GetPoints( const CoordinateConverter_ABC& converter, const Pathfind::T_Points& points )
    {
        sword::PathResult reply;
        for( auto it = points.begin(); it != points.end(); ++it )
        {
            auto point = reply.add_points();
            if( it->waypoint )
                point->set_waypoint( *it->waypoint );
            const auto src = converter.ConvertToGeo( it->where );
            auto dst = point->mutable_coordinate();
            dst->set_longitude( src.X() );
            dst->set_latitude( src.Y() );
        }
        return reply;
    }

    Pathfind::T_Points GetWaypoints( const Pathfind::T_Points& points )
    {
        Pathfind::T_Points reply;
        for( auto it = points.begin(); it != points.end(); ++it )
            if( it->waypoint )
                reply.push_back( *it );
        return reply;
    }

    typedef std::vector< std::unique_ptr< kernel::EquipmentType > > T_Equipments;

    T_Equipments MakeEquipments( const sword::Pathfind& pathfind )
    {
        T_Equipments reply;
        const auto& eqs = pathfind.request().equipment_types();
        for( auto it = eqs.begin(); it != eqs.end(); ++it )
            reply.emplace_back( EquipmentType::MakeFakeEquipment( it->id() ) );
        return reply;
    }

    class Equipments : public kernel::Equipments_ABC
                     , public boost::noncopyable
    {
    public:
        Equipments( const sword::Pathfind& pathfind )
            : eqs_( MakeEquipments( pathfind ) )
        {
            // NOTHING
        }

        virtual bool HasEquipment( const kernel::EquipmentType& type ) const
        {
            return std::find_if( eqs_.begin(), eqs_.end(), [&]( const std::unique_ptr< kernel::EquipmentType >& ptr )
            {
                return ptr->GetId() == type.GetId();
            } ) != eqs_.end();
        };

        virtual void Visit( const std::function< bool( const kernel::EquipmentType& ) >& functor ) const
        {
            for( auto it = eqs_.begin(); it != eqs_.end(); ++it )
                if( !functor( **it ) )
                    return;
        };

        virtual void CreateMaintenanceConsign( const sword::LogMaintenanceHandlingCreation& /*message*/ ) {}
        virtual void DeleteMaintenanceConsign( int /*id*/ ) {}
        virtual void Update( const sword::LogMaintenanceHandlingUpdate& /*message*/ ) {}
        virtual float GetTotalWeight() const { return 0; };

    private:
        const T_Equipments eqs_;
    };
}

Pathfind::Pathfind( Controller& controller,
                    actions::ActionsModel& actionsModel,
                    const CoordinateConverter_ABC& converter,
                    kernel::Entity_ABC& entity,
                    const sword::Pathfind& msg,
                    bool edition,
                    bool canBeOrdered )
    : EntityImplementation< Pathfind_ABC >( controller, msg.id(), msg.request().has_name() ?
                        msg.request().name().c_str() : tools::translate( "Pathfind", "itinerary" ), !canBeOrdered )
    , controller_( controller )
    , actionsModel_( actionsModel )
    , converter_( converter )
    , pathfind_( msg )
    , entity_( entity )
    , path_( GetPoints( converter, msg.result() ) )
    , waypoints_( edition ? GetWaypoints( path_ ) : T_Points() )
    , visible_( true )
{
    AddExtension( *this );
    Attach< kernel::Equipments_ABC >( *new Equipments( msg ) );
    Attach< kernel::TacticalHierarchies >( *new SimpleHierarchies< kernel::TacticalHierarchies >( *this, &entity_ ) );
}

Pathfind::~Pathfind()
{
    Destroy();
}

namespace
{
    const int size = 20;

    void SetColor( QColor color, bool picking )
    {
        if( !picking )
            glColor3d( color.redF(), color.greenF(), color.blueF() );
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

void Pathfind::Pick( const geometry::Point2f& where, const Viewport_ABC& viewport, GlTools_ABC& tools ) const
{
    Draw( where, viewport, tools, GetCurrent(), true );
}

void Pathfind::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, GlTools_ABC& tools ) const
{
    Draw( where, viewport, tools, GetCurrent(), false );
}

void Pathfind::Draw( const geometry::Point2f& /*where*/, const Viewport_ABC& /*viewport*/, GlTools_ABC& tools, const QColor& current, bool picking ) const
{
    if( !visible_ )
        return;
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT );
    glEnable( GL_ALPHA_TEST );
    glAlphaFunc( GL_GREATER, 0.1f );
    SetColor( current.darker(), picking );
    DrawLines( tools, 5 );
    SetColor( current, picking );
    DrawLines( tools, 3 );
    DrawPoints( tools, current, picking );
    glDisable( GL_ALPHA_TEST );
    glPopAttrib();
}

void Pathfind::DrawLines( GlTools_ABC& tools, float width ) const
{
    if( path_.empty() )
        return;
    auto last = path_.begin();
    tools.DrawDisc( last->where, width / 2, GlTools_ABC::pixels );
    for( auto it = last + 1; it != path_.end(); last = it++ )
    {
        tools.DrawLine( last->where, it->where, width );
        tools.DrawDisc( it->where, width / 2, GlTools_ABC::pixels );
    }
}

void Pathfind::DrawPoints( GlTools_ABC& tools, const QColor& current, bool picking ) const
{
    const bool selected = tools.ShouldDisplay();
    const auto& waypoints = waypoints_.empty() ? path_ : waypoints_;
    std::size_t i = 0;
    for( auto it = waypoints.begin(); it != waypoints.end(); ++it )
    {
        if( !it->waypoint )
            continue;
        DrawPoint( tools, it->where, current, picking, selected ||
                 ( hover_ && hover_->onWaypoint_ && hover_->lastWaypoint_ == i ) );
        ++i;
    }
    if( hover_ && !hover_->onWaypoint_ && hover_->coordinate_ )
        DrawPoint( tools, *hover_->coordinate_, current, false, true );
}

void Pathfind::DrawPoint( GlTools_ABC& tools, geometry::Point2f p, const QColor& current, bool picking, bool highlight ) const
{
    static const QImage normal = MakeBitmap( Qt::white, Qt::black );
    static const QImage highlighted = MakeBitmap( Qt::black, Qt::white );
    const QImage image = picking ? MakeBitmap( current, current ) : highlight ? highlighted : normal;
    const float offset = size * tools.Pixels( p ) / 2;
    tools.DrawImage( image, geometry::Point2f( p.X() - offset, p.Y() - offset ) );
}

void Pathfind::LoadPoints( const sword::PathResult& path )
{
    path_ = GetPoints( converter_, path );
    waypoints_ = GetWaypoints( path_ );
}

void Pathfind::Clear()
{
    path_.clear();
    waypoints_.clear();
}

void Pathfind::StartOn( const geometry::Point2f& where )
{
    const auto point = MakePoint( where, 0u );
    if( waypoints_.size() > 1 )
        waypoints_.front() = point;
    else
        waypoints_.push_front( point );
}

void Pathfind::StopOn( const geometry::Point2f& where )
{
    const auto point = MakePoint( where, 0u );
    if( waypoints_.size() > 1 )
        waypoints_.back() = point;
    else
        waypoints_.push_back( point );
}

bool Pathfind::HasPath() const
{
    return !path_.empty();
}

bool Pathfind::HasWaypoints() const
{
    return waypoints_.size() > 1;
}

std::vector< geometry::Point2f > Pathfind::GetDots() const
{
    std::vector< geometry::Point2f > reply;
    reply.reserve( waypoints_.size() );
    for( auto it = waypoints_.begin(); it != waypoints_.end(); ++it )
        reply.push_back( it->where );
    return reply;
}

namespace
{
    const auto threshold = 200; // pixels
}

bool Pathfind::IsNear( const GlTools_ABC& tools, float squareDistance, const geometry::Point2f& point ) const
{
    const auto pixels = tools.Pixels( point );
    return squareDistance < threshold * pixels * pixels;
}

boost::optional< Pathfind::Hover > Pathfind::PickWaypoint( const GlTools_ABC& tools, const geometry::Point2f& where ) const
{
    // path iterated backwards to select the waypoint on top
    // when several of them overlap
    for( auto i = waypoints_.size(); i > 0; --i )
    {
        const auto& position = waypoints_[ i - 1 ];
        if( !IsNear( tools, where.SquareDistance( position.where ), where ) )
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

boost::optional< Pathfind::Hover > Pathfind::PickSegment( const GlTools_ABC& tools, const geometry::Point2f& where ) const
{
    size_t waypoint = 0;
    float distance = std::numeric_limits< float >::infinity();
    auto last = path_.begin();
    boost::optional< Pathfind::Hover > best;
    for( auto it = last + 1; it != path_.end(); last = it++ )
    {
        if( last->waypoint )
            waypoint = *last->waypoint;
        const auto projection = geometry::Segment2f( it->where, last->where ).Project( where );
        const auto d = where.SquareDistance( projection );
        if( d >= distance || !IsNear( tools, d, where ) )
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

void Pathfind::InsertWaypoint( size_t idx, const geometry::Point2f where )
{
    waypoints_.insert( waypoints_.begin() + idx, MakePoint( where, 0u ) );
}

void Pathfind::EraseWaypoint( size_t idx )
{
    waypoints_.erase( waypoints_.begin() + idx );
}

void Pathfind::SetHover( const boost::optional< Pathfind::Hover >& hover )
{
    hover_ = hover;
}

const kernel::Entity_ABC& Pathfind::GetUnit() const
{
    return entity_;
}

void Pathfind::SetVisible( bool visible )
{
    visible_ = visible;
}

sword::PathResult Pathfind::GetPathfind() const
{
    return GetPoints( converter_, path_ );
}

geometry::Point2f Pathfind::GetPosition() const
{
    if( path_.empty() )
        return geometry::Point2f();
    return path_.front().where;
}

sword::Pathfind Pathfind::GetCreationMessage() const
{
    return pathfind_;
}

void Pathfind::DoUpdate( const sword::Pathfind& msg )
{
    if( msg.request().has_name() )
    {
        pathfind_ = msg;
        name_ = pathfind_.request().name().c_str();
        controller_.Update( gui::DictionaryUpdated( *this, tools::translate( "EntityImplementation", "Info" ) ) );
        Touch();
    }
}

void Pathfind::Rename( const QString& name )
{
    EntityImplementation< Pathfind_ABC >::Rename( name );
    auto pathfind = pathfind_;
    pathfind.mutable_request()->set_name( name.toStdString() );
    actionsModel_.PublishUpdatePathfind( pathfind );
}

void Pathfind::NotifyDestruction() const
{
    actionsModel_.PublishDestroyPathfind( GetId() );
}
