// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationDetections.h"
#include "Population.h"
#include "PopulationFlow.h"
#include "PopulationConcentration.h"
#include "Attr_def.h"

// -----------------------------------------------------------------------------
// Name: PopulationDetections constructor
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
PopulationDetections::PopulationDetections( Controller& controller, const Resolver_ABC< Population >& resolver )
    : controller_( controller )
    , resolver_( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationDetections destructor
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
PopulationDetections::~PopulationDetections()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationDetections::DoUpdate
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationDetections::DoUpdate( const ConcentrationDetectionMessage& message )
{
    unsigned long nPopulationID;
    unsigned long nConcentrationID;
    char nVisType;

    message >> nPopulationID >> nConcentrationID >> nVisType;

    const Population* pPopulation = & resolver_.Get( nPopulationID );
    const PopulationPart_ABC* pConcentration = & pPopulation->GetConcentration( nConcentrationID );
    if( nVisType == eVisTypeInvisible )
        perceived_.erase( pConcentration );
    else
        perceived_.insert( pConcentration );
}

// -----------------------------------------------------------------------------
// Name: PopulationDetections::DoUpdate
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationDetections::DoUpdate( const FlowDetectionMessage& message )
{
    unsigned long nPopulationID;
    unsigned long nFlowID;

    message >> nPopulationID >> nFlowID;
    const Population* pPopulation = & resolver_.Get( nPopulationID );

    const PopulationPart_ABC* pFlow = & pPopulation->GetFlow( nFlowID );

    unsigned long nNbrPoints;
    message >> nNbrPoints;
    // $$$$ AGE 2006-02-27: 
//
//    T_PointVector shape;
//    for( uint i = 0; i < nNbrPoints; ++i )
//    {
//        MT_Vector2D vTmp;
//        input >> vTmp;
//        shape.push_back( vTmp );
//    }

    if( nNbrPoints )
        perceived_.insert( pFlow );
    else
        perceived_.erase( pFlow );
}
