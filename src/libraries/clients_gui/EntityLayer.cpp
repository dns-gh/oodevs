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
#include "GlTooltip.h"
#include "View_ABC.h"
#include "Viewport_ABC.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::EntityLayerBase
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
EntityLayerBase::EntityLayerBase( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const Profile_ABC& profile )
    : controllers_( controllers )
    , profile_    ( profile )
    , tools_      ( tools )
    , strategy_   ( strategy )
    , view_       ( view )
    , tooltip_    ( 0 )
    , tooltiped_  ( controllers )
    , selected_   ( controllers )
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
    for( auto it = entities_.begin(); it != entities_.end(); ++it )
        if( *it != &*selected_ )
            Draw( **it, viewport );
    if( selected_ )
        Draw( *selected_, viewport );
    if( tooltiped_ )
        if( const Positions* positions = tooltiped_->Retrieve< Positions >() )
        {
            const geometry::Point2f position = positions->GetPosition();
            if( !tooltip_.get() )
            {
                std::auto_ptr< kernel::GlTooltip_ABC > tooltip = tools_.CreateTooltip();
                tooltip_ = tooltip;
            }
            tooltip_->Draw( position );
        }
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::Draw( const Entity_ABC& entity, Viewport_ABC& viewport )
{
    if( ShouldDisplay( entity ) )
    {
        SelectColor( entity );
        const Positions& positions = entity.Get< Positions >();
        const geometry::Point2f position = positions.GetPosition();
        viewport.SetHotpoint( position );
        DrawVisitor drawer;
        entity.Apply( drawer );
        drawer.Draw( position, viewport, tools_ );
    }
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
// Name: EntityLayerBase::HandleMousePress
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool EntityLayerBase::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( entities_.empty() || !event || event->buttons() == Qt::NoButton )
        return false;
    const int button = event->button();
    if( button != Qt::LeftButton && button != Qt::RightButton )
        return false;

    auto selected = std::find( entities_.begin(), entities_.end(), selected_.ConstCast() );

    if( selected == entities_.end() ||
        !IsInSelection( *selected_, point ) || !ShouldDisplay( *selected_ ) ||
        ( button == Qt::LeftButton && ++selected == entities_.end() ) )
        selected = entities_.begin();

    for( ; selected < entities_.end(); ++selected )
    {
        tooltiped_ = *selected;
        if( ShouldDisplay( **selected ) && IsInSelection( **selected, point ) )
        {
            if( button == Qt::LeftButton )
                Select( **selected, ( event->modifiers() & Qt::ControlModifier ) != 0, ( event->modifiers() & Qt::ShiftModifier ) != 0 );
            else if( button == Qt::RightButton && !IsReadOnly() )
                ContextMenu( **selected, point, event->globalPos() );
            selected_ = *selected;
            return true;
        }
    }
    return false;
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
void EntityLayerBase::Select( const Entity_ABC& entity, bool control, bool /*shift*/ )
{
    controllers_.actions_.SetSelected( entity, control );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::ContextMenu
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void EntityLayerBase::ContextMenu( const Entity_ABC& entity, const geometry::Point2f&, const QPoint& where )
{
    entity.ContextMenu( controllers_.actions_, where );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::HandleMouseMove
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
bool EntityLayerBase::HandleMouseMove( QMouseEvent* , const geometry::Point2f& point )
{
    if( tooltiped_ && !ShouldDisplayTooltip( *tooltiped_, point ) )
    {
        tooltiped_ = 0;
        if( tooltip_.get() )
            tooltip_->Hide();
        if( selected_ && !DisplayTooltip( *selected_, point ) )
        {
            bool found = false;
            for( auto it = entities_.begin(); it != entities_.end(); ++it )
                found = *it != &*selected_ && DisplayTooltip( **it, point );
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::ShouldDisplayTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
bool EntityLayerBase::ShouldDisplayTooltip( const kernel::Entity_ABC& entity, const geometry::Point2f& point )
{
    return ShouldDisplay( entity ) && IsInSelection( entity, point );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::DisplayTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
bool EntityLayerBase::DisplayTooltip( const kernel::Entity_ABC& entity, const geometry::Point2f& point )
{
    if( !tooltip_.get() )
    {
        std::auto_ptr< kernel::GlTooltip_ABC > tooltip = tools_.CreateTooltip();
        tooltip_ = tooltip;
    }
    if( ShouldDisplayTooltip( entity, point ) && DisplayTooltip( entity, *tooltip_ ) )
    {
        tooltiped_ = &entity;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::DisplayTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
bool EntityLayerBase::DisplayTooltip( const Entity_ABC& entity, Displayer_ABC& displayer )
{
    entity.GetInterfaces().Apply( & Displayable_ABC::DisplayInTooltip, displayer );
    return true;
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::IsInSelection
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool EntityLayerBase::IsInSelection( const Entity_ABC& entity, const geometry::Point2f& point ) const
{
    return entity.Get< Positions >().IsAt( point, 5.f * tools_.Pixels( point ), tools_.GetAdaptiveZoomFactor() );
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
        if( &entity == selected_ )
            selected_ = 0;
        if( &entity == tooltiped_ )
            tooltiped_ = 0;
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
    tooltiped_ = 0;
    kernel::Selectable_ABC::T_Selectables selectables;
    for( auto it = entities_.begin(); it != entities_.end(); ++it )
    {
        if( ShouldDisplay( **it ) && ( IsInside( **it, rectangle ) || IsInSelection( **it, topLeft ) ) )
        {
            selected_ = *it;
            tooltiped_ = *it;
            selectables.push_back( *it );
        }
    }
    if( !selectables.empty() )
        controllers_.actions_.AddToSelection( selectables );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::ExtractElements
// Created: ABR 2013-01-25
// -----------------------------------------------------------------------------
void EntityLayerBase::ExtractElements( T_LayerElements& extractedElement, const geometry::Point2f& point )
{
    for( auto it = entities_.begin(); it != entities_.end(); ++it )
        if( ShouldDisplay( **it ) && IsInSelection( **it, point ) )
            extractedElement[ this ].push_back( *it );
}
