// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EntityLayer.h"

#include "DrawVisitor.h"
#include "InformationToolTip.h"
#include "View_ABC.h"
#include "Viewport_ABC.h"
#include "Viewport2d.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "ENT/ENT_Tr.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::EntityLayerBase
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
EntityLayerBase::EntityLayerBase( Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view,
                                 const Profile_ABC& profile, E_LayerTypes type )
    : controllers_( controllers )
    , profile_    ( profile )
    , tools_      ( tools )
    , strategy_   ( strategy )
    , view_       ( view )
    , infoTooltip_( new InformationToolTip() )
    , selected_   ( controllers )
    , name_       ( ENT_Tr::ConvertFromLayerTypes( type ).c_str() )
    , type_       ( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::~EntityLayerBase
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
EntityLayerBase::~EntityLayerBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::Initialize
// Created: JSR 2012-06-25
// -----------------------------------------------------------------------------
void EntityLayerBase::Initialize( const geometry::Rectangle2f& extent )
{
    world_ = extent;
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::Paint
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::Paint( Viewport_ABC& viewport )
{
    strategy_.SetAlpha( GetAlpha() );
    bool pickingMode = tools_.IsPickingMode();

    // Draw the entities
    for( auto it = entities_.begin(); it != entities_.end(); ++it )
        if( *it != &*selected_ && !(*it)->IsHighlighted() )
            Draw( **it, viewport, pickingMode );
    // Draw the highlighted entities
    for( auto it = entities_.begin(); it != entities_.end(); ++it )
        if( *it != &*selected_ && (*it)->IsHighlighted() )
            Draw( **it, viewport, pickingMode );
    // Draw the selected entity
    if( selected_ )
        Draw( *selected_, viewport, pickingMode );

    if( !pickingMode )
        infoTooltip_->Draw();
}

namespace
{
    geometry::Point2f GetPosition( const Entity_ABC& entity )
    {
        if( const kernel::UrbanPositions_ABC* urbanPositions = entity.Retrieve< kernel::UrbanPositions_ABC >() )
            return urbanPositions->Barycenter();
        const kernel::Positions& positions = entity.Get< kernel::Positions >();
        return positions.GetPosition();
    }
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::Draw( const Entity_ABC& entity, Viewport_ABC& viewport, bool pickingMode )
{
    if( ShouldDisplay( entity ) && tools_.ShouldDisplay( type_ ) )
    {
        SelectColor( entity );
        const geometry::Point2f position = GetPosition( entity );
        viewport.SetHotpoint( position );
        DrawVisitor drawer;
        entity.Apply( drawer );

        if( pickingMode )
        {
            tools_.RenderPicking( std::make_pair( entity.GetId(), type_ ) );
            drawer.Pick( position, viewport, tools_ );
            tools_.Picking();
        }
        else
            drawer.Draw( position, viewport, tools_ );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::IsPickable
// Created: LGY 2013-02-25
// -----------------------------------------------------------------------------
bool EntityLayerBase::IsPickable() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::ShouldDisplay
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
bool EntityLayerBase::ShouldDisplay( const Entity_ABC& entity )
{
    return IsEnabled() && profile_.IsVisible( entity );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::HandleMouseDoubleClick
// Created: JSR 2012-06-01
// -----------------------------------------------------------------------------
bool EntityLayerBase::HandleMouseDoubleClick( QMouseEvent* event, const geometry::Point2f& point )
{
    return HandleMousePress( event, point );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::Select
// Created: AGE 2006-08-03
// -----------------------------------------------------------------------------
void EntityLayerBase::Select( const kernel::GraphicalEntity_ABC& selectable, bool control, bool /*shift*/ )
{
    selected_ = &static_cast< const kernel::Entity_ABC& >( selectable );
    controllers_.actions_.SetSelected( selectable, control );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::ContextMenu
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void EntityLayerBase::ContextMenu( const kernel::GraphicalEntity_ABC& selectable, const geometry::Point2f&, const QPoint& where )
{
    selectable.ContextMenu( controllers_.actions_, where );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::ContextMenu
// Created: LGY 2013-04-03
// -----------------------------------------------------------------------------
bool EntityLayerBase::ContextMenu( const std::vector< const kernel::GraphicalEntity_ABC* >& elements, const geometry::Point2f& geoPoint, const QPoint& where )
{
    if( !selected_ )
        return false;
    for( auto it = elements.begin(); it != elements.end(); ++it )
    {
        const kernel::Entity_ABC& entity = static_cast< const kernel::Entity_ABC& >( **it );
        if( IsSelected( entity ) )
        {
            ContextMenu( *selected_, geoPoint, where );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::IsInSelection
// Created: LGY 2013-03-01
// -----------------------------------------------------------------------------
bool EntityLayerBase::IsInSelection( const kernel::Entity_ABC& entity ) const
{
    return selection_.find( entity.GetId() ) != selection_.end();
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::IsSelected
// Created: LGY 2013-04-04
// -----------------------------------------------------------------------------
bool EntityLayerBase::IsSelected( const kernel::Entity_ABC& entity ) const
{
    return selected_ && selected_->GetId() == entity.GetId();
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::IsInside
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
bool EntityLayerBase::IsInside( const kernel::Entity_ABC& entity, const geometry::Rectangle2f& rectangle ) const
{
    return entity.Get< Positions >().IsIn( rectangle );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::AddEntity
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::AddEntity( const Entity_ABC& entity )
{
    if( std::find( entities_.begin(), entities_.end(), &entity ) == entities_.end() )
        entities_.push_back( &entity );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::RemoveEntity
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool EntityLayerBase::RemoveEntity( const Entity_ABC& entity )
{
    auto it = std::find( entities_.begin(), entities_.end(), &entity );
    if( it != entities_.end() )
    {
        entities_.erase( it );
        if( IsSelected( entity ) )
            selected_ = 0;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::ActivateEntity
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::ActivateEntity( const Entity_ABC& entity )
{
    view_.CenterOn( entity.Get< Positions >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::SelectEntity
// Created: AGE 2006-06-19
// -----------------------------------------------------------------------------
void EntityLayerBase::SelectEntity( const Entity_ABC& entity )
{
    if( std::find( entities_.begin(), entities_.end(), &entity ) != entities_.end() )
        selected_ = &entity;
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::SelectColor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::SelectColor( const Entity_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::SelectInRectangle
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
void EntityLayerBase::SelectInRectangle( const geometry::Point2f& topLeft, const geometry::Point2f& bottomRight )
{
    if( entities_.empty() )
        return;
    if( controllers_.actions_.IsSingleSelection( entities_.front() ) )
        return;
    geometry::Rectangle2f rectangle( topLeft, bottomRight );
    selected_ = 0;
    kernel::GraphicalEntity_ABC::T_GraphicalEntities selectables;
    for( auto it = entities_.begin(); it != entities_.end(); ++it )
    {
        if( ShouldDisplay( **it ) && IsInside( **it, rectangle ) )
        {
            selected_ = *it;
            selectables.push_back( *it );
        }
    }
    if( !selectables.empty() )
        controllers_.actions_.AddToSelection( selectables );
}

namespace
{
    bool IsValid( const kernel::Entity_ABC& entity, const GlTools_ABC::T_ObjectPicking& selection, E_LayerTypes type )
    {
        return entity.GetId() == selection.first && type == selection.second;
    }

    bool IsInLayerSelection(  const kernel::Entity_ABC& entity, const GlTools_ABC::T_ObjectsPicking& selection, E_LayerTypes type )
    {
        for( auto it = selection.begin(); it != selection.end(); ++it )
            if( IsValid( entity, *it, type ) )
                return true;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::ExtractElements
// Created: ABR 2013-01-25
// -----------------------------------------------------------------------------
void EntityLayerBase::ExtractElements( T_LayerElements& extractedElement, const T_ObjectsPicking& selection )
{
    selection_.clear();
    infoTooltip_->Hide();
    for( auto it = entities_.begin(); it != entities_.end(); ++it )
    {
        const kernel::Entity_ABC& entity = **it;
        if( ShouldDisplay( **it ) && IsInLayerSelection( entity, selection, type_ ) )
        {
            extractedElement[ this ].push_back( &entity );
            selection_.insert( entity.GetId() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::GetName
// Created: ABR 2013-01-25
// -----------------------------------------------------------------------------
QString EntityLayerBase::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::ShowTooltip
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
bool EntityLayerBase::ShowTooltip( const T_ObjectPicking& selection )
{
    for( auto it = entities_.begin(); it != entities_.end(); ++it )
    {
        const kernel::Entity_ABC& entity = **it;
        if( ShouldDisplay( entity ) && IsValid( entity, selection, type_ ) )
        {
            entity.GetInterfaces().Apply( &Displayable_ABC::DisplayInTooltip, *infoTooltip_ );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::HideTooltip
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
void EntityLayerBase::HideTooltip()
{
    infoTooltip_->Hide();
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::Pick
// Created: LGY 2013-03-11
// -----------------------------------------------------------------------------
void EntityLayerBase::Pick( const geometry::Point2f& point )
{
    selection_.clear();
    GlTools_ABC::T_ObjectsPicking selection;
    tools_.FillSelection( point, selection, type_ );
    if( !selection.empty() )
        for( auto it = entities_.begin(); it != entities_.end(); ++it )
        {
            const kernel::Entity_ABC& entity = **it;
            if( ShouldDisplay( **it ) && IsInLayerSelection( entity, selection, type_ ) )
                selection_.insert( entity.GetId() );
        }
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::SelectColor
// Created: JSR 2013-06-07
// -----------------------------------------------------------------------------
template<>
void EntityLayer< kernel::Entity_ABC >::SelectColor( const kernel::Entity_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::ContextMenu
// Created: JSR 2013-06-07
// -----------------------------------------------------------------------------
template<>
void EntityLayer<kernel::Entity_ABC>::ContextMenu( const kernel::GraphicalEntity_ABC& , const geometry::Point2f& , const QPoint&  )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::FillContextMenu
// Created: JSR 2013-06-07
// -----------------------------------------------------------------------------
template<>
void EntityLayer< kernel::Entity_ABC >::FillContextMenu( const kernel::GraphicalEntity_ABC& , kernel::ContextMenu& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::IsIn
// Created: JSR 2013-06-07
// -----------------------------------------------------------------------------
template<>
bool EntityLayer< kernel::Entity_ABC >::IsIn( const kernel::GraphicalEntity_ABC& ) const
{
    return false;
}
