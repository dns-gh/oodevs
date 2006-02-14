// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "FlowDetections.h"
#include "Controller.h"
#include "Population.h"

// -----------------------------------------------------------------------------
// Name: FlowDetections constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
FlowDetections::FlowDetections( Controller& controller, const Resolver_ABC< Population >& resolver )
    : controller_( controller )
    , resolver_( resolver )
{

}
    
// -----------------------------------------------------------------------------
// Name: FlowDetections destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
FlowDetections::~FlowDetections()
{

}
    
// -----------------------------------------------------------------------------
// Name: FlowDetections::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void FlowDetections::Update( const FlowDetectionMessage& message )
{
    unsigned long populationId;
    unsigned long flowId;

    message >> populationId >> flowId;
    const Population& popu = resolver_.Get( populationId );
    const PopulationFlow* flow = & popu.GetFlow( flowId );

    unsigned long size;
    message >> size;
    if( ! size )
        perceivedFlows_.erase( flow );
    else
    {
        T_FlowChunk& perceivedChunk = perceivedFlows_[ flow ];
        perceivedChunk.reserve( size );
        for( unsigned long i = 0; i < size; ++i )
        {
            MT_Vector2D vTmp;
            message >> vTmp;
            perceivedChunk.push_back( vTmp );
        }
    }
    controller_.Update( *this );
}
