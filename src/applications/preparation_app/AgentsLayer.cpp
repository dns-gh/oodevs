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
#include "preparation/AgentPositions.h"
#include "preparation/HierarchyTemplate.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Team_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: SBO 2006-08-31
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controllers& controllers, const GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, Model& model, ModelBuilder& modelBuilder, const Profile_ABC& profile )
    : gui::AgentsLayer  ( controllers, tools, strategy, view, profile )
    , model_            ( model )
    , modelBuilder_     ( modelBuilder )
    , selectedAgent_    ( controllers )
    , selectedAutomat_  ( controllers )
    , selectedFormation_( controllers )
    , selectedTeam_     ( controllers )
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
// Name: AgentsLayer::BeforeSelection
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsLayer::BeforeSelection()
{
    selectedAgent_ = 0;
    selectedAutomat_ = 0;
    selectedFormation_ = 0;
    selectedTeam_ = 0;
}
    
// -----------------------------------------------------------------------------
// Name: AgentsLayer::AfterSelection
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsLayer::AfterSelection()
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
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsLayer::Select( const kernel::Automat_ABC& element )
{
    selectedAutomat_ = &element;
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
// Name: AgentsLayer::Select
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void AgentsLayer::Select( const kernel::Team_ABC& element )
{
    selectedTeam_ = &element;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleEnterDragEvent
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& )
{
    // $$$$ SBO 2006-09-29: Unify mime-types
    return ( event->provides( "Agent" )                    && selectedAgent_     )
        || ( event->provides( "csword/AgentType" )         && selectedAutomat_   )
        || ( event->provides( "csword/AutomatType" )       && selectedFormation_ )
        || ( event->provides( "csword/HierarchyTemplate" ) && IsValidTemplate( event ) );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::IsValidTemplate
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool AgentsLayer::IsValidTemplate( QDragEnterEvent* event ) const
{
    if( !selectedFormation_ && !selectedTeam_ )
        return false;
    QByteArray tmp = event->encodedData( "csword/HierarchyTemplate" );
    const HierarchyTemplate* droppedItem = *reinterpret_cast< const HierarchyTemplate** >( tmp.data() );
    if( ! droppedItem )
        return false;
    return selectedTeam_ ? droppedItem->IsCompatible( *selectedTeam_ )
                         : droppedItem->IsCompatible( *selectedFormation_ );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleDropEvent
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( event->provides( "Agent" ) )
    {
        if( !selectedAgent_ )
            return false;
        if( const AgentPositions* position = static_cast< const AgentPositions* >( selectedAgent_->Retrieve< Positions >() ) )
        {
            const_cast< AgentPositions* >( position )->Set( point );
            return true;
        }
    }
    else if( event->provides( "csword/AgentType" ) )
    {
        if( !selectedAutomat_ )
            return false;
        QByteArray tmp = event->encodedData( "csword/AgentType" );
        const AgentType* droppedItem = *reinterpret_cast< const AgentType** >( tmp.data() );
        if( droppedItem )
        {
            model_.agents_.CreateAgent( *selectedAutomat_.ConstCast(), *droppedItem, point );
            return true;
        }
    }
    else if( event->provides( "csword/AutomatType" ) )
    {
        if( !selectedFormation_ )
            return false;
        QByteArray tmp = event->encodedData( "csword/AutomatType" );
        const AutomatType* droppedItem = *reinterpret_cast< const AutomatType** >( tmp.data() );
        if( droppedItem )
        {
            model_.agents_.CreateAutomat( *selectedFormation_.ConstCast(), *droppedItem, point );
            return true;
        }
    }
    else if( event->provides( "csword/HierarchyTemplate" ) )
    {
         // $$$$ AGE 2007-05-30: test selectedParent !
        if( !selectedFormation_ )
            return false;
        QByteArray tmp = event->encodedData( "csword/HierarchyTemplate" );
        const HierarchyTemplate* droppedItem = *reinterpret_cast< const HierarchyTemplate** >( tmp.data() );
        if( droppedItem )
        {
            droppedItem->Instanciate( *selectedFormation_.ConstCast(), point );
            return true;
        }
    }
    return false;
}
