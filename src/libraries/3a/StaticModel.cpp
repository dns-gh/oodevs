// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "StaticModel.h"
#include "tools/ExerciseConfig.h"
#include "PowerIndicators.h"
#include "Extractors.h"

using namespace aar;

// -----------------------------------------------------------------------------
// Name: StaticModel constructor
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
StaticModel::StaticModel( const tools::ExerciseConfig& config )
    : powerIndicators_( new PowerIndicators() )
{
    powerIndicators_->Load( config );
}

// -----------------------------------------------------------------------------
// Name: StaticModel destructor
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
StaticModel::~StaticModel()
{
    powerIndicators_->Purge();
}

// -----------------------------------------------------------------------------
// Name: StaticModel::ComputePower
// Created: ABR 2011-02-11
// -----------------------------------------------------------------------------
unsigned int StaticModel::ComputePower( unsigned long uid, const extractors::PowerExtractor_ABC& extractor ) const
{
    return extractor.GetPowerValue( powerIndicators_->tools::Resolver< PowerIndicator, unsigned long >::Get( uid ) );
}
