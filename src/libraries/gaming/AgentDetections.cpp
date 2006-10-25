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
#include "clients_kernel/Controller.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentDetections constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentDetections::AgentDetections( Controller& controller, const Resolver_ABC< Agent_ABC >& resolver, const Entity_ABC& holder )
    : controller_( controller )
    , resolver_  ( resolver )
    , holder_    ( holder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentDetections destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentDetections::~AgentDetections()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentDetections::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentDetections::DoUpdate( const DetectionMessage& message )
{
    unsigned long id;
    unsigned char nVisType;
    unsigned char nMaxVisType; // $$$$ AGE 2006-02-14: deal with it
    bool bRecordMode;

    message >> id >> nVisType >> nMaxVisType >> bRecordMode;
    Agent_ABC* agent = & resolver_.Get( id );

    // 4 = eRecorded
    detections_[ agent ] = bRecordMode ? E_UnitVisType( 4 ) : E_UnitVisType( nVisType );

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentDetections::Draw
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentDetections::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    // $$$$ AGE 2006-04-21: viewport
    if( ! tools.ShouldDisplay( "VisionLines" ) || detections_.empty() )
        return;
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glLineWidth( 1.f );
    for( CIT_AgentDetections it = detections_.begin(); it != detections_.end(); ++it )
    {
        const Agent_ABC& agent = *it->first;
        if( ! IsSameTeam( agent ) && it->second != eVisTypeInvisible )
        {
            if( it->second == eVisTypeRecognized )
                glColor4f( COLOR_RECO );
            else if( it->second == eVisTypeIdentified )
                glColor4f( COLOR_IDENTIFIED );
            else if( it->second == E_UnitVisType( 4 ) )
                glColor4f( COLOR_RECORDED );
            else
                glColor4f( COLOR_DETECTED );
            tools.DrawLine( where, it->first->Get< Positions >().GetPosition() );
        }
    }
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: AgentDetections::IsSameTeam
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
bool AgentDetections::IsSameTeam( const Entity_ABC& entity ) const
{
    return entity. Get< CommunicationHierarchies >().IsSubordinateOf(
           holder_.Get< CommunicationHierarchies >().GetTop() );
}
