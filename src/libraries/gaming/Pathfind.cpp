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
#include "PathfindHierarchies.h"
#include "actions/ActionsModel.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Equipments_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace gui;

namespace
{
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
                    const T_CanBeRenamedFunctor& canBeRenamedFunctor )
    : EntityImplementation< Pathfind_ABC >( controller, msg.id(), msg.request().has_name() ?
                        msg.request().name().c_str() : tools::translate( "Pathfind", "itinerary" ), &actionsModel, canBeRenamedFunctor )
    , pathfind_( msg )
    , entity_( &entity )
    , itinerary_( converter, msg, edition )
    , visible_( true )
{
    AddExtension( *this );
    Attach< kernel::Equipments_ABC >( *new Equipments( msg ) );
    Attach< kernel::TacticalHierarchies >( *new PathfindHierarchies( *this ) );
}

Pathfind::~Pathfind()
{
    Destroy();
}

void Pathfind::Pick( const geometry::Point2f& /*where*/, const Viewport_ABC& /*viewport*/, GLView_ABC& tools ) const
{
    itinerary_.Draw( tools, hover_, true );
}

void Pathfind::Draw( const geometry::Point2f& /*where*/, const Viewport_ABC& /*viewport*/, GLView_ABC& tools ) const
{
    itinerary_.Draw( tools, hover_, false );
}

void Pathfind::LoadPoints( const sword::PathResult& path )
{
    itinerary_.LoadPoints( path );
}

void Pathfind::Clear()
{
    itinerary_.Clear();
}

void Pathfind::StartOn( const geometry::Point2f& where )
{
    itinerary_.StartOn( where );
}

void Pathfind::StopOn( const geometry::Point2f& where )
{
    itinerary_.StopOn( where );
}

bool Pathfind::HasPath() const
{
    return itinerary_.HasPath();
}

bool Pathfind::HasWaypoints() const
{
    return itinerary_.HasWaypoints();
}

std::vector< geometry::Point2f > Pathfind::GetDots() const
{
    return itinerary_.GetDots();
}

boost::optional< Itinerary::Hover > Pathfind::PickWaypoint( const GLView_ABC& tools, const geometry::Point2f& where ) const
{
    return itinerary_.PickWaypoint( tools, where );
}

boost::optional< Itinerary::Hover > Pathfind::PickSegment( const GLView_ABC& tools, const geometry::Point2f& where ) const
{
    return itinerary_.PickSegment( tools, where );
}

void Pathfind::InsertWaypoint( size_t idx, const geometry::Point2f where )
{
    itinerary_.InsertWaypoint( idx, where );
}

void Pathfind::EraseWaypoint( size_t idx )
{
    itinerary_.EraseWaypoint( idx );
}

void Pathfind::SetHover( const boost::optional< Itinerary::Hover >& hover )
{
    hover_ = hover;
}

int Pathfind::ChangeSuperior( const kernel::Entity_ABC& target )
{
    entity_ = &target;
    sword::Pathfind pathfind( pathfind_ );
    auto request = pathfind.mutable_request();
    request->set_name( name_ );
    request->mutable_unit()->set_id( target.GetId() );
    return actionsModel_->PublishUpdatePathfind( pathfind );
}

const kernel::Entity_ABC& Pathfind::GetUnit() const
{
    return *entity_;
}

void Pathfind::SetVisible( bool visible )
{
    visible_ = visible;
}

sword::PathResult Pathfind::GetPathfind() const
{
    return itinerary_.GetPathfind();
}

geometry::Point2f Pathfind::GetPosition() const
{
    return itinerary_.GetPosition();
}

sword::Pathfind Pathfind::GetCreationMessage() const
{
    return pathfind_;
}

void Pathfind::UpdateMessage( const sword::Pathfind& msg, const Entity_ABC& owner )
{
    pathfind_ = msg;
    Rename( QString::fromStdString( pathfind_.request().name() ) );
    entity_ = &owner;
}

void Pathfind::NotifyDestruction() const
{
    actionsModel_->PublishDestroyPathfind( GetId() );
}

void Pathfind::PublishRename()
{
    auto pathfind = pathfind_;
    pathfind.mutable_request()->set_name( name_.toStdString() );
    actionsModel_->PublishUpdatePathfind( pathfind );
}
