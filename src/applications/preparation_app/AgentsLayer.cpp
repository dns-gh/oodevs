// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "AgentsLayer.h"
#include "ModelBuilder.h"
#include "preparation/Model.h"
#include "preparation/AgentsModel.h"
#include "clients_kernel/Formation_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: SBO 2006-08-31
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controllers& controllers, const GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, Model& model, ModelBuilder& modelBuilder )
    : gui::AgentsLayer( controllers, tools, strategy, view )
    , model_( model )
    , modelBuilder_( modelBuilder )
    , selectedAgent_( controllers )
    , selectedFormation_( controllers )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: AgentsLayer destructor
// Created: SBO 2006-08-31
// -----------------------------------------------------------------------------
AgentsLayer::~AgentsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::Select
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
void AgentsLayer::Select( const kernel::Agent_ABC& element )
{
    selectedAgent_ = &element;
}
    
// -----------------------------------------------------------------------------
// Name: AgentsLayer::Select
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
void AgentsLayer::Select( const kernel::Formation_ABC& element )
{
    selectedFormation_ = &element;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleEnterDragEvent
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& )
{
    return event->provides( "astec/AgentType" ) || event->provides( "astec/AutomatType" );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleDropEvent
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( event->provides( "astec/AgentType" ) )
    {
        if( !selectedAgent_ || !selectedAgent_->GetAutomatType() )
            return false;
        QByteArray tmp = event->encodedData( "astec/AgentType" );
        const AgentType* droppedItem = *reinterpret_cast< const AgentType** >( tmp.data() );
        if( droppedItem )
        {
            model_.agents_.CreateAgent( *selectedAgent_.ConstCast(), *droppedItem, point );
            return true;
        }
    }
    else if( event->provides( "astec/AutomatType" ) )
    {
        if( !selectedFormation_ )
            return false;
        QByteArray tmp = event->encodedData( "astec/AutomatType" );
        const AutomatType* droppedItem = *reinterpret_cast< const AutomatType** >( tmp.data() );
        if( droppedItem )
        {
            model_.agents_.CreateAgent( *selectedFormation_.ConstCast(), *droppedItem, point );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleKeyPress
// Created: SBO 2006-09-05
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleKeyPress( QKeyEvent* key )
{
    if( key->key() == Qt::Key_Delete )
        return modelBuilder_.OnDelete();
    return false;
}
