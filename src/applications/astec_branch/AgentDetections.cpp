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

// -----------------------------------------------------------------------------
// Name: AgentDetections constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentDetections::AgentDetections( Controller& controller, const Resolver_ABC< Agent >& resolver )
    : controller_( controller )
    , resolver_( resolver )
{

}

// -----------------------------------------------------------------------------
// Name: AgentDetections destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentDetections::~AgentDetections()
{

}

// -----------------------------------------------------------------------------
// Name: AgentDetections::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentDetections::Update( const DetectionMessage& message )
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
