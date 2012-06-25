// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ObjectsLayer.h"
#include "clients_gui/ValuedDragObject.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Object_ABC.h"
#include "preparation/AltitudeModifierAttribute.h"
#include "preparation/FloodAttribute.h"
#include "preparation/ObjectPositions.h"

namespace
{
    class LayerValuedDragObject : public gui::ValuedDragObject
    {
    public:
        LayerValuedDragObject( const ObjectPositions* pos, const kernel::Entity_ABC* entity, QWidget* dragSource = 0 )
            : ValuedDragObject( pos, dragSource )
            , entity_    ( entity )
        {
            if( entity_ )
                if( const AltitudeModifierAttribute* attribute = entity_->Retrieve< AltitudeModifierAttribute >() )
                    attribute->BeginDrag();
        }
        virtual ~LayerValuedDragObject()
        {
            if( entity_)
            {
                if( const AltitudeModifierAttribute* attribute = entity_->Retrieve< AltitudeModifierAttribute >() )
                    attribute->EndDrag();
                if( const FloodAttribute* flood = entity_->Retrieve< FloodAttribute >() )
                    flood->EndDrag();
            }
        }
    private:
        const kernel::Entity_ABC* entity_;
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer constructor
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
ObjectsLayer::ObjectsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile, gui::TerrainPicker& picker )
    : gui::ObjectsLayer( controllers, tools, strategy, view, profile, picker )
    , selected_( controllers )
    , tools_   ( tools )
    , dummy_   ( new QWidget )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer destructor
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
ObjectsLayer::~ObjectsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::HandleKeyPress
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
bool ObjectsLayer::HandleKeyPress( QKeyEvent* key )
{
    if( selected_ && key->key() == Qt::Key_Delete )
    {
        delete (const kernel::Object_ABC*)selected_;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::CanDrop
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
bool ObjectsLayer::CanDrop( QDragMoveEvent* event, const geometry::Point2f& ) const
{
    if( !selected_ )
        return false;
    if( gui::ValuedDragObject::Provides< const ObjectPositions >( event ) )
        return true;
    if( kernel::Entity_ABC* entity = gui::ValuedDragObject::GetValue< kernel::Entity_ABC >( event ) )
        return entity->GetTypeName() == kernel::Object_ABC::typeName_;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::HandleMoveDragEvent
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
bool ObjectsLayer::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    if( ObjectPositions* position = gui::ValuedDragObject::GetValue< ObjectPositions >( event ) )
    {
        if( !selected_ )
            return false;
        if( draggingPoint_.Distance( point ) >= 5.f * tools_.Pixels( point ) )
        {
            const geometry::Vector2f translation( draggingPoint_, point );
            const geometry::Rectangle2f boundingBox = position->GetBoundingBox() + translation;
            if( boundingBox.Intersect( world_ ) == boundingBox )
            {
                position->Translate( draggingPoint_, translation, 5.f * tools_.Pixels( point ) );
                draggingPoint_ = point;
            }
        }
        return true;
    }
    else if( kernel::Object_ABC* entity = dynamic_cast< kernel::Object_ABC* >( gui::ValuedDragObject::GetValue< kernel::Entity_ABC >( event ) ) )
    {
        ObjectPositions* position = static_cast< ObjectPositions* >( entity->Retrieve< kernel::Positions >() );
        const geometry::Point2f translation = point - position->GetBoundingBox().Center().ToVector();
        const geometry::Rectangle2f boundingBox = position->GetBoundingBox() + translation.ToVector();
        if( boundingBox.Intersect( world_ ) == boundingBox )
        {
            position->Move( point );
            draggingPoint_ = point;
        }
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::HandleDropEvent
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
bool ObjectsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& )
{
    if( selected_ && gui::ValuedDragObject::GetValue< ObjectPositions >( event ) )
    {
        draggingPoint_.Set( 0, 0 );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::NotifySelectionChanged
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void ObjectsLayer::NotifySelectionChanged( const std::vector< const kernel::Object_ABC* >& elements )
{
    selected_ = elements.size() == 1 ? elements.front() : 0;
    gui::ObjectsLayer::NotifySelectionChanged( elements );
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::HandleMousePress
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
bool ObjectsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    bool result = gui::ObjectsLayer::HandleMousePress( event, point );
    if( ( event->button() & Qt::LeftButton ) != 0 && event->state() == Qt::NoButton && IsEligibleForDrag( point ) )
    {
        if( const ObjectPositions* pos = static_cast< const ObjectPositions* >( selected_->Retrieve< kernel::Positions >() ) )
        {
            draggingPoint_ = point;
            Q3DragObject* drag = new LayerValuedDragObject( pos, selected_, dummy_.get() );
            drag->dragMove();
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::IsEligibleForDrag
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
bool ObjectsLayer::IsEligibleForDrag( const geometry::Point2f& point ) const
{
    return selected_ && IsInSelection( *selected_, point );
}
