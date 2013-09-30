// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentDetections.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentDetections constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentDetections::AgentDetections( Controller& controller, const tools::Resolver_ABC< Agent_ABC >& resolver, const Entity_ABC& holder )
    : controller_( controller )
    , resolver_  ( resolver )
    , holder_    ( holder )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentDetections destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentDetections::~AgentDetections()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentDetections::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentDetections::DoUpdate( const sword::UnitDetection& message )
{
    detections_[ & resolver_.Get( message.detected_unit().id() ) ] = message.current_visibility();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentDetections::NotifyDeleted
// Created: JSR 2012-11-20
// -----------------------------------------------------------------------------
void AgentDetections::NotifyDeleted( const kernel::Agent_ABC& agent )
{
    IT_AgentDetections it = detections_.find( &agent );
    if( it != detections_.end() )
        detections_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: AgentDetections::Draw
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentDetections::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& , gui::GlTools_ABC& tools ) const
{
    if( ! tools.ShouldDisplay( "VisionLines" ) || detections_.empty() )
        return;
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    for( auto it = detections_.begin(); it != detections_.end(); ++it )
    {
        const Agent_ABC& agent = *it->first;
        if( ! IsSameKnowledgeGroup( agent ) && it->second != sword::UnitVisibility::invisible )
        {
            if( it->second == sword::UnitVisibility::recognized )
                glColor4f( COLOR_RECO );
            else if( it->second == sword::UnitVisibility::identified )
                glColor4f( COLOR_IDENTIFIED );
            else if( it->second == sword::UnitVisibility::recorded )
                glColor4f( COLOR_RECORDED );
            else
                glColor4f( COLOR_DETECTED );
            tools.DrawLine( where, it->first->Get< Positions >().GetPosition(), 1.f );
        }
    }
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: AgentDetections::IsSameKnowledgeGroup
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
bool AgentDetections::IsSameKnowledgeGroup( const Entity_ABC& entity ) const
{
    const CommunicationHierarchies* entityHierarchy = entity.Retrieve< CommunicationHierarchies >();
    const CommunicationHierarchies* holderHierarchy = holder_.Retrieve< CommunicationHierarchies >();
    if( !holderHierarchy->CanReceive() || !entityHierarchy->CanReceive() )
        return false;
    const Entity_ABC* firstKnowledgeGroup = &entity;
    const Entity_ABC* currentEntity = &entity;
    for( ;; )
    {
        entityHierarchy = currentEntity->Retrieve< CommunicationHierarchies >();
        const Entity_ABC* superior = entityHierarchy->GetSuperior();
        if( superior )
            firstKnowledgeGroup = currentEntity;
        else
            break;
        currentEntity = superior;
    }
    currentEntity = &holder_;
    while( currentEntity )
    {
        entityHierarchy = currentEntity->Retrieve< CommunicationHierarchies >();
        currentEntity = entityHierarchy->GetSuperior();
        if( currentEntity == firstKnowledgeGroup )
            return true;
    }
    return false;
}
