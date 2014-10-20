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
#include "clients_gui/GLView_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "protocol/Protocol.h"

using namespace kernel;

namespace
{

bool CanShareKnowledge( const Entity_ABC& e1, const Entity_ABC& e2 )
{
    const CommunicationHierarchies* h1 = e1.Retrieve< CommunicationHierarchies >();
    const CommunicationHierarchies* h2 = e2.Retrieve< CommunicationHierarchies >();
    if( !h1->CanReceive() || !h2->CanReceive() )
        return false;
    const Entity_ABC* firstKnowledgeGroup = &e2;
    const Entity_ABC* currentEntity = &e2;
    for( ;; )
    {
        h2 = currentEntity->Retrieve< CommunicationHierarchies >();
        const Entity_ABC* superior = h2->GetSuperior();
        if( !superior )
            break;
        firstKnowledgeGroup = currentEntity;
        currentEntity = superior;
    }
    currentEntity = &e1;
    while( currentEntity )
    {
        h1 = currentEntity->Retrieve< CommunicationHierarchies >();
        currentEntity = h1->GetSuperior();
        if( currentEntity == firstKnowledgeGroup )
            return true;
    }
    return false;
}

}  //namespace

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
void AgentDetections::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& , gui::GLView_ABC& tools ) const
{
    static const float colors[4][4] =
    {
        { COLOR_RECOGNIZED },
        { COLOR_IDENTIFIED },
        { COLOR_RECORDED },
        { COLOR_DETECTED }
    };
    if( ! tools.ShouldDisplay( "VisionLines" ) || detections_.empty() || holder_.IsAggregated() )
        return;
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    for( auto it = detections_.begin(); it != detections_.end(); ++it )
    {
        const Agent_ABC& agent = *it->first;
        if( ! CanShareKnowledge( holder_, agent ) && it->second != sword::UnitVisibility::invisible )
        {
            if( it->second == sword::UnitVisibility::recognized )
                glColor4f( colors[0][0], colors[0][1], colors[0][2], tools.GetCurrentAlpha() );
            else if( it->second == sword::UnitVisibility::identified )
                glColor4f( colors[1][0], colors[1][1], colors[1][2], tools.GetCurrentAlpha() );
            else if( it->second == sword::UnitVisibility::recorded )
                glColor4f( colors[2][0], colors[2][1], colors[2][2], tools.GetCurrentAlpha() );
            else
                glColor4f( colors[3][0], colors[3][1], colors[3][2], tools.GetCurrentAlpha() );
            tools.DrawLine( where, it->first->Get< Positions >().GetPosition(), 1.f );
        }
    }
    glPopAttrib();
}
