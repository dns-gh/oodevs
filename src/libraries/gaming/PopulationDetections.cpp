// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationDetections.h"
#include "clients_kernel/Population_ABC.h"
#include "PopulationFlow.h"
#include "PopulationConcentration.h"
#include "Attr_def.h"

// -----------------------------------------------------------------------------
// Name: PopulationDetections constructor
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
PopulationDetections::PopulationDetections( Controller& controller, const Resolver_ABC< Population_ABC >& resolver )
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

    const Population_ABC* pPopulation = & resolver_.Get( nPopulationID );
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
    const Population_ABC* pPopulation = & resolver_.Get( nPopulationID );

    const PopulationPart_ABC* pFlow = & pPopulation->GetFlow( nFlowID );

    unsigned long nNbrPoints;
    message >> nNbrPoints;

    shape_.clear(); shape_.reserve( nNbrPoints );
    for( unsigned i = 0; i < nNbrPoints; ++i )
    {
        double x, y;
        message >> x >> y;
        shape_.push_back( geometry::Point2f( float( x ), float( y ) ) );
    }

    if( nNbrPoints )
        perceived_.insert( pFlow );
    else
        perceived_.erase( pFlow );
}

// -----------------------------------------------------------------------------
// Name: PopulationDetections::DoUpdate
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void PopulationDetections::DoUpdate( const PopulationCollisionMessage& message )
{
    // $$$$ AGE 2006-03-17: 
}

// -----------------------------------------------------------------------------
// Name: PopulationDetections::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void PopulationDetections::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    // $$$$ AGE 2006-04-06: draw something
}


