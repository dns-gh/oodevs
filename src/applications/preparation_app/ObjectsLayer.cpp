// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ObjectsLayer.h"
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Object_ABC.h"
#include "preparation/AltitudeModifierAttribute.h"
#include "preparation/FloodAttribute.h"
#include "preparation/ObjectPositions.h"

namespace
{
    class LayerDrag : public QDrag
    {
    public:
        LayerDrag( const ObjectPositions* pos, const kernel::Entity_ABC* entity, QWidget* dragSource )
            : QDrag( dragSource )
            , entity_( entity )
        {
            QMimeData* mimeData = new QMimeData;
            QByteArray encodedData;
            QDataStream stream( &encodedData, QIODevice::WriteOnly );
            stream << reinterpret_cast< unsigned int >( pos );
            mimeData->setData( QString( typeid( ObjectPositions ).name() ), encodedData );
            setMimeData( mimeData );
            if( entity_ )
                if( const AltitudeModifierAttribute* attribute = entity_->Retrieve< AltitudeModifierAttribute >() )
                    attribute->BeginDrag();
        }
        virtual ~LayerDrag()
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
ObjectsLayer::ObjectsLayer( kernel::Controllers& controllers,
                            gui::GLView_ABC& view,
                            gui::ColorStrategy_ABC& strategy,
                            const kernel::Profile_ABC& profile,
                            gui::TerrainPicker& picker )
    : gui::ObjectsLayer( controllers, view, strategy, profile, picker )
    , selected_( controllers )
    , dummy_( new QWidget() )
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
        delete static_cast< const kernel::Object_ABC* >( selected_ );
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
    return dnd::HasData< ObjectPositions >( event ) || dnd::HasData< kernel::Object_ABC >( event );
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::HandleEnterDragEvent
// Created: JSR 2012-06-26
// -----------------------------------------------------------------------------
bool ObjectsLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point )
{
    oldPosition_ = geometry::Point2f();
    if( const kernel::Object_ABC* entity = dnd::FindSafeData< kernel::Object_ABC >( event ) )
    {
        if( const kernel::Positions* position = entity->Retrieve< kernel::Positions >() )
            oldPosition_ = position->GetBoundingBox().Center();
    }
    return gui::ObjectsLayer::HandleEnterDragEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::HandleMoveDragEvent
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
bool ObjectsLayer::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    if( dnd::HasData< ObjectPositions >( event ) && selected_ )
    {
        if( ObjectPositions* position = static_cast< ObjectPositions* >( selected_.ConstCast()->Retrieve< kernel::Positions >() ) )
        {
            if( draggingPoint_.Distance( point ) >= 5.f * view_.Pixels( point ) )
            {
                const geometry::Vector2f translation( draggingPoint_, point );
                const geometry::Rectangle2f boundingBox = position->GetBoundingBox() + translation;
                if( boundingBox.Intersect( world_ ) == boundingBox )
                {
                    position->Translate( draggingPoint_, translation, 5.f * view_.Pixels( point ) );
                    draggingPoint_ = point;
                }
            }
        }
        return true;
    }
    if( kernel::Object_ABC* entity = dnd::FindSafeData< kernel::Object_ABC >( event ) )
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
    if( dnd::HasData< ObjectPositions >( event ) && selected_ )
    {
        draggingPoint_.Set( 0, 0 );
        return true;
    }
    if( dnd::HasData< kernel::Object_ABC >( event ) )
        oldPosition_ = geometry::Point2f();
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::HandleLeaveDragEvent
// Created: JSR 2012-06-26
// -----------------------------------------------------------------------------
bool ObjectsLayer::HandleLeaveDragEvent( QDragLeaveEvent* /*event*/ )
{
    if( selected_ && !oldPosition_.IsZero() )
    {
        static_cast< ObjectPositions* >( selected_.ConstCast()->Retrieve< kernel::Positions >() )->Move( oldPosition_ );
        oldPosition_ = geometry::Point2f();
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
    if( event->button() == Qt::LeftButton && event->buttons() != Qt::NoButton && IsEligibleForDrag() )
        if( const ObjectPositions* pos = static_cast< const ObjectPositions* >( selected_->Retrieve< kernel::Positions >() ) )
        {
            draggingPoint_ = point;
            QDrag* drag = new LayerDrag( pos, selected_, dummy_.get() );
            drag->exec();
        }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::IsEligibleForDrag
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
bool ObjectsLayer::IsEligibleForDrag() const
{
    return selected_ && IsInSelection( *selected_ );
}
