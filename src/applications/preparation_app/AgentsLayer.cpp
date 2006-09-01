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
#include "preparation/Model.h"
#include "preparation/AgentsModel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: SBO 2006-08-31
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controllers& controllers, const GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, Model& model )
    : gui::AgentsLayer( controllers, tools, strategy, view )
    , model_( model )
    , selectedAgent_( controllers )
    , selectedGroup_( controllers )
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
void AgentsLayer::Select( const kernel::KnowledgeGroup_ABC& element )
{
    selectedGroup_ = &element;
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
            model_.agents_.CreateAgent( *selectedAgent_, *droppedItem, point );
            return true;
        }
    }
    else if( event->provides( "astec/AutomatType" ) )
    {
        if( !selectedGroup_ )
            return false;
        QByteArray tmp = event->encodedData( "astec/AutomatType" );
        const AutomatType* droppedItem = *reinterpret_cast< const AutomatType** >( tmp.data() );
        if( droppedItem )
        {
            model_.agents_.CreateAgent( *selectedGroup_, *droppedItem, point );
            return true;
        }
    }
    return false;
}
