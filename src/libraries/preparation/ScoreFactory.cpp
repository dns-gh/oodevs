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
#include "Model.h"

// -----------------------------------------------------------------------------
// Name: ScoreFactory constructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
ScoreFactory::ScoreFactory( kernel::Controllers& controllers, const indicators::Primitives& indicators, const indicators::GaugeFactory_ABC& gaugeFactory, const Model& model )
    : controllers_( controllers )
    , indicators_( indicators )
    , gaugeFactory_( gaugeFactory )
    , model_( model )
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
    return new Score( xis, controllers_, indicators_, gaugeFactory_, model_.profiles_ );
}

// -----------------------------------------------------------------------------
// Name: ScoreFactory::CreateScore
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
Score_ABC* ScoreFactory::CreateScore( const QString& name ) const
{
    return new Score( name, controllers_, indicators_, gaugeFactory_ );
}
