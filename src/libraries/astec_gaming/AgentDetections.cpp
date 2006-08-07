// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "AgentDetections.h"
#include "Controller.h"
#include "Positions.h"
#include "Agent_ABC.h"
#include "astec_gui/GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: AgentDetections constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentDetections::AgentDetections( Controller& controller, const Resolver_ABC< Agent_ABC >& resolver, const Team& team )
    : controller_( controller )
    , resolver_  ( resolver )
    , team_      ( team )
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
        if( ! agent.IsInTeam( team_ ) && it->second != eVisTypeInvisible )
        {
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
    }
    glPopAttrib();
}
