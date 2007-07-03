// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PopulationsLayer.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "preparation/Model.h"
#include "preparation/AgentsModel.h"
#include "preparation/PopulationPositions.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationsLayer constructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
PopulationsLayer::PopulationsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, Model& model, const kernel::Profile_ABC& profile )
    : gui::PopulationsLayer( controllers, tools, strategy, view, profile )
    , model_( model )
    , selectedEntity_( controllers )
    , selectedPopulation_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer destructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
PopulationsLayer::~PopulationsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::HandleEnterDragEvent
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
bool PopulationsLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& )
{
    return event->provides( "csword/PopulationType" )
        || ( selectedPopulation_ && event->provides( "population" ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::HandleDropEvent
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
bool PopulationsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( event->provides( "csword/PopulationType" ) && selectedEntity_ )
    {
        const kernel::Hierarchies* hierarchies = selectedEntity_->Retrieve< kernel::TacticalHierarchies >();
        if( !hierarchies )
            if( !( hierarchies = selectedEntity_->Retrieve< kernel::CommunicationHierarchies >() ) )
                return false;
        const kernel::Entity_ABC& top = hierarchies->GetTop();
        if( const kernel::Team_ABC* team = dynamic_cast< const kernel::Team_ABC* >( &top ) )
        {
            QByteArray tmp = event->encodedData( "csword/PopulationType" );
            const PopulationType* droppedItem = *reinterpret_cast< const PopulationType** >( tmp.data() );
            if( droppedItem )
            {
                model_.agents_.CreatePopulation( *const_cast< kernel::Team_ABC* >( team ), *droppedItem, point );
                return true;
            }
        }
    }
    else if( event->provides( "population" ) && selectedPopulation_ )
    {
        if( const PopulationPositions* positions = static_cast< const PopulationPositions* >( selectedPopulation_->Retrieve< Positions >() ) )
        {
            const_cast< PopulationPositions* >( positions )->Set( point );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::HandleKeyPress
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
bool PopulationsLayer::HandleKeyPress( QKeyEvent* key )
{
    if( key->key() == Qt::Key_Delete && selectedPopulation_ )
    {
        delete (const Population_ABC*)selectedPopulation_;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::BeforeSelection
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationsLayer::BeforeSelection()
{
    selectedEntity_ = 0;
    selectedPopulation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::AfterSelection
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationsLayer::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::Select
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationsLayer::Select( const kernel::Entity_ABC& element )
{
    selectedEntity_ = &element;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::Select
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationsLayer::Select( const kernel::Population_ABC& element )
{
    selectedPopulation_ = &element;
}

// $$$$ AGE 2007-07-03: ^c^v...

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::HandleMousePress
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
bool PopulationsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    bool result = gui::PopulationsLayer::HandleMousePress( event, point );
    if( ( event->button() & Qt::LeftButton ) != 0 && event->state() == Qt::NoButton && IsEligibleForDrag( point ) )
    {
        QDragObject* drag = new QStoredDrag( "Population" );
        drag->drag();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: PopulationsLayer::IsEligibleForDrag
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
bool PopulationsLayer::IsEligibleForDrag( const geometry::Point2f& point )
{
    return selectedPopulation_ && IsInSelection( *selectedPopulation_, point );
}
