// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UrbanLayer.h"
#include "geometry/Types.h"
#include "clients_gui/View_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "Urban/TerrainObject_ABC.h"
#include "urban/Drawer_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Polygon.h"
#include "clients_kernel/Circle.h"
#include "clients_gui/UrbanDrawer.h"
#include "clients_gui/TerrainObjectProxy.h"

using namespace gui;
// -----------------------------------------------------------------------------
// Name: EntityLayerBase::UrbanLayer
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
UrbanLayer::UrbanLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools/*, ColorStrategy_ABC& strategy, View_ABC& view */)
: controllers_   ( controllers )
, tools_        ( tools )
, urbanDrawer_  ( new UrbanDrawer( tools ) )
, selectionArea_( 0 )
, tooltiped_    ( std::numeric_limits< unsigned >::max() )
, selectionMode_( false )
, selectedObject_( 0 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::~UrbanLayer
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
UrbanLayer::~UrbanLayer()
{
    controllers_.Unregister( *this );
    objects_.clear();
    delete selectionArea_;
    delete urbanDrawer_;
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::Paint
// Created: SLG 2006-03-23
// -----------------------------------------------------------------------------
void UrbanLayer::Paint( kernel::Viewport_ABC& /*viewport*/ )
{      
    for( IT_TerrainObjects it = objects_.begin(); it != objects_.end(); ++it )
    {
        const TerrainObjectProxy* object = (*it);
        object->object_->SetSelected( object == selectedObject_ );
        object->object_->Draw( *urbanDrawer_ );
    }
}

// -----------------------------------------------------------------------------
// Name: EditorLayer::Reset
// Created: RPD 2009-10-14
// -----------------------------------------------------------------------------
void UrbanLayer::Reset2d()
{
    objects_.clear();
    selectedObject_ = 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::NotifyCreated
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
void UrbanLayer::NotifyCreated( const TerrainObjectProxy& object )
{
    if( std::find( objects_.begin(), objects_.end(), &object ) == objects_.end() )
        objects_.push_back( &object );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::NotifyDeleted
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
void UrbanLayer::NotifyDeleted( const TerrainObjectProxy& object )
{
    IT_TerrainObjects it = std::find( objects_.begin(), objects_.end(), &object );
    if ( it != objects_.end() )
        objects_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::NotifyActivated
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
void UrbanLayer::NotifyActivated( const TerrainObjectProxy& /*object*/ )
{
    //view_.CenterOn( entity.Get< Positions >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::NotifySelected
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
void UrbanLayer::NotifySelected( const TerrainObjectProxy* /*object*/ )
{
    /*IT_TerrainObjects it = std::find( *objects_.begin(), *objects_.end(), object );
    if( it != objects_.end() )
    selected_ = it - objects_.begin();*/
}


// -----------------------------------------------------------------------------
// Name: UrbanLayer::HandleMousePress
// Created: SLG 2009-08-09
// -----------------------------------------------------------------------------
bool UrbanLayer::HandleMousePress( QMouseEvent* input, const geometry::Point2f& point )
{
    const int button = input->button();
    if( button != Qt::LeftButton && button != Qt::RightButton )
        return false;
    if( button == Qt::RightButton )
    {
        for( IT_TerrainObjects it = objects_.begin(); it != objects_.end(); ++it )
        {
            const TerrainObjectProxy* object = (*it);
            if( object->object_->GetFootprint()->IsInside( point ) && input->state() )
            {
                if ( object == selectedObject_ )
                {
                    selectedObject_ = 0;
                    return false;
                }
                selectedObject_ = object;
                controllers_.actions_.ContextMenu( *object, input->globalPos() );
                return true;
            }
        }
    }
    else if( button == Qt::LeftButton )
    {
        for( IT_TerrainObjects it = objects_.begin(); it != objects_.end(); ++it )
        {
            const TerrainObjectProxy* object = (*it);
            if( object->object_->GetFootprint()->IsInside( point ) && input->state() )
            {
                if ( object == selectedObject_ )
                {
                    selectedObject_ = 0;
                    return false;
                }
                selectedObject_ = object;
                controllers_.actions_.Select( *static_cast< const kernel::Entity_ABC* >( object ) );
                return true;
            }
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::HandleKeyPress
// Created: RPD 2009-10-15
// -----------------------------------------------------------------------------
bool UrbanLayer::HandleKeyPress( QKeyEvent* input )
{
    switch ( input->key() )
    {
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
    {
        selectedObject_ = 0;
        break;
    }

    default:
        break;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::HandleMouseMove
// Created: SLG 2009-08-09
// -----------------------------------------------------------------------------
bool UrbanLayer::HandleMouseMove( QMouseEvent*, const geometry::Point2f& point )
{
    lastPoint_ = point;
    if ( selectionArea_ && selectionMode_ )
    {
        /*geometry::T_Point2fVector points = static_cast< Lines* >(selectionArea_)->GetPoints();
        if ( points.size() != 0 )
        {
            selectedObjects_.clear();
            geometry::Polygon2f::T_Vertices selectionPoints; 
            selectionPoints.push_back( points[0] );
            selectionPoints.push_back( geometry::Point2f( lastPoint_.X(), points[0].Y() ) );
            selectionPoints.push_back( lastPoint_ );
            selectionPoints.push_back( geometry::Point2f( points[0].X(), lastPoint_.Y() ) );
            geometry::Polygon2f polygonSelection( selectionPoints );

            for( IT_TerrainObjects it = objects_.begin(); it != objects_.end(); ++it )
            {
                geometry::Polygon2f::T_Vertices footPrintObject = (**it).GetFootprint()->Vertices();

                for( geometry::Polygon2f::IT_Vertices itPoint = footPrintObject.begin(); itPoint != footPrintObject.end(); ++itPoint )
                {
                    if( polygonSelection.IsInside( *itPoint ) )
                    {
                        selectedObjects_.push_back( *it );
                    }
                }
            }
        }*/
    }
    return selectionArea_ != 0;
}


// -----------------------------------------------------------------------------
// Name: EditorLayer::NotifyDone
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
void UrbanLayer::NotifyDone()
{
    selectionArea_ = 0;
    selectionMode_ = false;
}
