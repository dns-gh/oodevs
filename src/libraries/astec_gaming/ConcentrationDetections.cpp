// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "ConcentrationDetections.h"
#include "Attr_Def.h"
#include "astec_kernel/Controller.h"
#include "Population_ABC.h"

// -----------------------------------------------------------------------------
// Name: ConcentrationDetections constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ConcentrationDetections::ConcentrationDetections( Controller& controller, const Resolver_ABC< Population_ABC >& resolver )
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
    Population_ABC& popu = resolver_.Get( nPopulationID );
    const PopulationConcentration* pConcentration = & popu.GetConcentration( nConcentrationID );

    if( nVisType == eVisTypeInvisible )
        perceivedConcentrations_.erase( pConcentration );
    else
        perceivedConcentrations_.insert( pConcentration );
    controller_.Update( *this );
}   
