// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ScoreFactory.h"
#include "Score.h"

// -----------------------------------------------------------------------------
// Name: ScoreFactory constructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
ScoreFactory::ScoreFactory( kernel::Controller& controller, const indicators::Primitives& indicators, const indicators::GaugeFactory_ABC& gaugeFactory )
    : controller_( controller )
    , indicators_( indicators )
    , gaugeFactory_( gaugeFactory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoreFactory destructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
ScoreFactory::~ScoreFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoreFactory::CreateScore
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
Score_ABC* ScoreFactory::CreateScore( xml::xistream& xis ) const
{
    return new Score( xis, controller_, indicators_, gaugeFactory_ );
}

// -----------------------------------------------------------------------------
// Name: ScoreFactory::CreateScore
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
Score_ABC* ScoreFactory::CreateScore( const QString& name ) const
{
    return new Score( name, controller_, indicators_, gaugeFactory_ );
}
