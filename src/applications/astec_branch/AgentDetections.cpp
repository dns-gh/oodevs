// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentDetections.h"
#include "Controller.h"
#include "Positions.h"
#include "Agent.h"
#include "GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: AgentDetections constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentDetections::AgentDetections( Controller& controller, const Resolver_ABC< Agent >& resolver )
    : controller_( controller )
    , resolver_( resolver )
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
    Agent* agent = & resolver_.Get( id );

    detections_[ agent ] = bRecordMode ? E_UnitVisType( 4 ) : E_UnitVisType( nVisType ); // $$$$ AGE 2006-02-14: 4 = eRecorded

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentDetections::Draw
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentDetections::Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const
{
    if( ! tools.ShouldDisplay( "VisionLines" ) )
        return;
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glLineWidth( 1.f );
    for( CIT_AgentDetections it = detections_.begin(); it != detections_.end(); ++it )
    {
//        const Agent& agent = *it->first;
//        if( agent.GetKnowledgeGroup()->GetTeam() ...
        if( it->second == eVisTypeRecognized )
            glColor4d( COLOR_RECO );
        else if( it->second == eVisTypeIdentified )
            glColor4d( COLOR_IDENTIFIED );
        else if( it->second == E_UnitVisType( 4 ) )
            glColor4d( COLOR_RECORDED );
        else
            glColor4d( COLOR_DETECTED );
        tools.DrawLine( where, it->first->Get< Positions >().GetPosition() );
    }
    glPopAttrib();
}
