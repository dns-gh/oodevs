// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EntityLayer.h"
#include "DrawVisitor.h"
#include "InformationToolTip.h"
#include "PickingSelector.h"
#include "Viewport_ABC.h"
#include "Viewport2d.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "ENT/ENT_Tr.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::EntityLayerBase
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
EntityLayerBase::EntityLayerBase( Controllers& controllers,
                                  GLView_ABC& view,
                                  ColorStrategy_ABC& strategy,
                                  const Profile_ABC& profile,
                                  E_LayerTypes type )
    : Layer( controllers, view, type )
    , profile_    ( profile )
    , strategy_   ( strategy )
    , infoTooltip_( new InformationToolTip() )
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
    const bool pickingMode = view_.GetPickingSelector().IsPickingMode();

    // Draw not selected and not highlighted entities
    for( auto it = entities_.begin(); it != entities_.end(); ++it )
        if( *it != selected_ && !( *it )->IsHighlighted() )
            Draw( **it, viewport, pickingMode );
    // Then draw not selected and highlighted entities
    for( auto it = entities_.begin(); it != entities_.end(); ++it )
        if( *it != selected_ && ( *it )->IsHighlighted() )
            Draw( **it, viewport, pickingMode );
    // Finaly, selected entity will be drawn by SelectionLayer

    if( !pickingMode )
        infoTooltip_->Draw();
}

geometry::Point2f EntityLayerBase::GetPosition( const Entity_ABC& entity ) const
{
    if( auto pos = entity.Retrieve< kernel::UrbanPositions_ABC >() )
        return pos->Barycenter();
    if( auto pos = entity.Retrieve< kernel::Positions >() )
        return pos->GetPosition();
    return geometry::Point2f();
}

void EntityLayerBase::SetHotpoint( const kernel::Entity_ABC& /*entity*/, Viewport_ABC& viewport, const geometry::Point2f& where ) const
{
    viewport.SetHotpoint( where );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void EntityLayerBase::Draw( const Entity_ABC& entity, Viewport_ABC& viewport, bool pickingMode )
{
    if( ShouldDisplay( entity ) && view_.GetPickingSelector().ShouldDisplay( GetType() ) )
    {
        SelectColor( entity );
        const geometry::Point2f position = GetPosition( entity );
        SetHotpoint( entity, viewport, position );

        DrawVisitor drawer;
        entity.Apply( drawer );

        if( pickingMode )
        {
            view_.RenderPicking( std::make_pair( entity.GetId(), GetType() ) );
            drawer.Pick( position, viewport, view_ );
            view_.Picking();
        }
        else
            drawer.Draw( position, viewport, view_ );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::ShouldDisplay
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
bool EntityLayerBase::ShouldDisplay( const Entity_ABC& entity )
{
    return profile_.IsVisible( entity );
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::HandleMouseDoubleClick
// Created: JSR 2012-06-01
// -----------------------------------------------------------------------------
bool EntityLayerBase::HandleMouseDoubleClick( QMouseEvent* event, const geometry::Point2f& point )
{
    return HandleMousePress( event, point );
}

const kernel::Entity_ABC* EntityLayerBase::FindEntity( unsigned int id ) const
{
    for( auto it = entities_.begin(); it != entities_.end(); ++it )
        if( (*it)->GetId() == id )
            return *it;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::Select
// Created: AGE 2006-08-03
// -----------------------------------------------------------------------------
void EntityLayerBase::Select( unsigned int id, bool control )
{
    if( const kernel::Entity_ABC* entity = FindEntity( id ) )
    {
        controllers_.actions_.SetSelected( *entity, control );
        selected_ = entity;
    }
}

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::ContextMenu
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void EntityLayerBase::ContextMenu( const kernel::GraphicalEntity_ABC& selectable, const geometry::Point2f&, const QPoint& where )
{
    selectable.ContextMenu( controllers_.actions_, where, this );
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
void EntityLayerBase::SelectEntity( const Entity_ABC* entity )
{
    selected_ = std::find( entities_.begin(), entities_.end(), entity ) != entities_.end() ? entity : nullptr;
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
    bool IsValid( const kernel::Entity_ABC& entity, const GLView_ABC::T_ObjectPicking& selection, E_LayerTypes type )
    {
        return entity.GetId() == selection.first && type == selection.second;
    }

    bool IsInLayerSelection(  const kernel::Entity_ABC& entity, const GLView_ABC::T_ObjectsPicking& selection, E_LayerTypes type )
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
        if( ShouldDisplay( **it ) && IsInLayerSelection( entity, selection, GetType() ) )
        {
            extractedElement[ this ].push_back( &entity );
            selection_.insert( entity.GetId() );
        }
    }
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
        if( ShouldDisplay( entity ) && IsValid( entity, selection, GetType() ) )
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
    GLView_ABC::T_ObjectsPicking selection;
    view_.FillSelection( point, selection, GetType() );
    if( !selection.empty() )
        for( auto it = entities_.begin(); it != entities_.end(); ++it )
        {
            const kernel::Entity_ABC& entity = **it;
            if( ShouldDisplay( **it ) && IsInLayerSelection( entity, selection, GetType() ) )
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
void EntityLayer< kernel::Entity_ABC >::FillContextMenu( unsigned int, kernel::ContextMenu& )
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
