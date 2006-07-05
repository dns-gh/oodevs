// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ConcentrationDetections.h"
#include "Attr_Def.h"
#include "Controller.h"
#include "Population.h"

// -----------------------------------------------------------------------------
// Name: ConcentrationDetections constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ConcentrationDetections::ConcentrationDetections( Controller& controller, const Resolver_ABC< Population >& resolver )
    : controller_( controller )
    , resolver_( resolver )
{

}

// -----------------------------------------------------------------------------
// Name: ConcentrationDetections destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ConcentrationDetections::~ConcentrationDetections()
{

}

// -----------------------------------------------------------------------------
// Name: ConcentrationDetections::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ConcentrationDetections::Update( const ConcentrationDetectionMessage& message )
{
    unsigned long  nPopulationID;
    unsigned long  nConcentrationID;
    unsigned char  nVisType;

    message >> nPopulationID >> nConcentrationID >> nVisType;
    Population& popu = resolver_.Get( nPopulationID );
    const PopulationConcentration* pConcentration = & popu.GetConcentration( nConcentrationID );

    if( nVisType == eVisTypeInvisible )
        perceivedConcentrations_.erase( pConcentration );
    else
        perceivedConcentrations_.insert( pConcentration );
    controller_.Update( *this );
}   
