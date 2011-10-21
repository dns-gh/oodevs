// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "GhostsLayer.h"
#include "preparation/GhostModel.h"
#include "preparation/GhostPositions.h"
#include "preparation/Model.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/GhostPrototype.h"
#include "clients_kernel/Moveable_ABC.h"
#include "clients_gui/ValuedDragObject.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: GhostsLayer constructor
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
GhostsLayer::GhostsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                          gui::View_ABC& view, Model& model, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter )
    : gui::EntityLayer< Ghost_ABC >( controllers, tools, strategy, view, profile, filter )
    , model_            ( model )
    , selectedGhost_    ( controllers )
    , selectedAutomat_  ( controllers )
    , selectedFormation_( controllers )
    , dummy_( new QWidget() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer destructor
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
GhostsLayer::~GhostsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::HandleEnterDragEvent
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostsLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& /*point*/ )
{
    return ( gui::ValuedDragObject::Provides< const GhostPrototype >( event ) && ( selectedAutomat_ || selectedFormation_ ) )
        || ( gui::ValuedDragObject::Provides< const GhostPositions >( event ) && selectedGhost_ );
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::HandleDropEvent
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( const GhostPrototype* droppedItem = gui::ValuedDragObject::GetValue< const GhostPrototype >( event ) )
    {
        Entity_ABC* selectedEntity = selectedFormation_.ConstCast();
        if( !selectedEntity )
            selectedEntity = selectedAutomat_.ConstCast();
        if( !selectedEntity )
            return false;
        model_.ghosts_.Create( *selectedEntity, *droppedItem, point );
        return true;
    }
    if( GhostPositions* position = gui::ValuedDragObject::GetValue< GhostPositions >( event ) )
    {
        if( !selectedGhost_ )
            return false;
        // if the events comes from the list or if far enough
        if( event->source() || position->GetPosition( true ).Distance( point ) > 100 )
            position->Move( point );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::HandleKeyPress
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostsLayer::HandleKeyPress( QKeyEvent* key )
{
    if( key->key() == Qt::Key_Delete && selectedGhost_ )
    {
        delete (const Ghost_ABC*)selectedGhost_;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::HandleMousePress
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    bool result = EntityLayer< kernel::Ghost_ABC >::HandleMousePress( event, point );
    if( ( event->button() & Qt::LeftButton ) != 0 && event->state() == Qt::NoButton && IsEligibleForDrag( point ) )
    {
        const GhostPositions* pos = static_cast< const GhostPositions* >( selectedGhost_->Retrieve< Positions >() );
        Q3DragObject* drag = new gui::ValuedDragObject( pos, dummy_ );
        drag->dragMove();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::BeforeSelection
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostsLayer::BeforeSelection()
{
    selectedGhost_ = 0;
    selectedAutomat_ = 0;
    selectedFormation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::AfterSelection
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostsLayer::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::Select
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostsLayer::Select( const kernel::Ghost_ABC& element )
{
    selectedGhost_ = &element;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::Select
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostsLayer::Select( const kernel::Automat_ABC& element )
{
    selectedAutomat_ = &element;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::Select
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostsLayer::Select( const kernel::Formation_ABC& element )
{
    selectedFormation_ = &element;
}

// -----------------------------------------------------------------------------
// Name: GhostsLayer::IsEligibleForDrag
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostsLayer::IsEligibleForDrag( const geometry::Point2f& point )
{
    return selectedGhost_ && IsInSelection( *selectedGhost_, point );
}
