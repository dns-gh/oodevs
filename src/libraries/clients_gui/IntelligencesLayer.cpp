// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "IntelligencesLayer.h"
#include "ValuedDragObject.h"
#include "clients_kernel/IntelligencePrototype.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: IntelligencesLayer constructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
IntelligencesLayer::IntelligencesLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                                        View_ABC& view, const Profile_ABC& profile, IntelligenceFactory_ABC& factory, const gui::LayerFilter_ABC& filter )
    : EntityLayer< Intelligence_ABC >( controllers, tools, strategy, view, profile, filter )
    , factory_( factory )
    , selectedIntelligence_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligencesLayer destructor
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
IntelligencesLayer::~IntelligencesLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligencesLayer::NotifySelected
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void IntelligencesLayer::NotifySelected( const kernel::Intelligence_ABC* element )
{
    selectedIntelligence_ = element;
    EntityLayer< kernel::Intelligence_ABC >::NotifySelected( element );
}

// -----------------------------------------------------------------------------
// Name: IntelligencesLayer::HandleKeyPress
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
bool IntelligencesLayer::HandleKeyPress( QKeyEvent* k )
{
    if( !selectedIntelligence_ )
        return false;
    const int key = k->key();
    if( key == Qt::Key_Backspace || key == Qt::Key_Delete )
    {
        Delete( *selectedIntelligence_ );
        selectedIntelligence_ = 0;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: IntelligencesLayer::HandleEnterDragEvent
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
bool IntelligencesLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& )
{
    // $$$$ SBO 2007-10-12: TODO: handle changing position
    return   ValuedDragObject::Provides< const IntelligencePrototype >( event )
        || ( ValuedDragObject::Provides< Positions >( event ) && selectedIntelligence_ );
}

// -----------------------------------------------------------------------------
// Name: IntelligencesLayer::HandleDropEvent
// Created: SBO 2007-10-12
// -----------------------------------------------------------------------------
bool IntelligencesLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( const IntelligencePrototype* droppedItem = ValuedDragObject::GetValue< const IntelligencePrototype >( event ) )
    {
        droppedItem->CreateIntelligence( factory_, point );
        return true;
    }
    if( Positions* position = ValuedDragObject::GetValue< Positions >( event ) )
    {
        if( !selectedIntelligence_ )
            return false;
        if( event->source() || position->GetPosition().Distance( point ) > 100 )
            Move( *position, point );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: IntelligencesLayer::HandleMousePress
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
bool IntelligencesLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    bool result = EntityLayer< kernel::Intelligence_ABC >::HandleMousePress( event, point );
    if( ( event->button() & Qt::LeftButton ) != 0 && event->state() == Qt::NoButton && IsEligibleForDrag( point ) )
    {
        Q3DragObject* drag = new ValuedDragObject( selectedIntelligence_->Retrieve< Positions >() );
        drag->drag();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: IntelligencesLayer::IsEligibleForDrag
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
bool IntelligencesLayer::IsEligibleForDrag( const geometry::Point2f& point ) const
{
    return selectedIntelligence_ && IsInSelection( *selectedIntelligence_, point );
}
