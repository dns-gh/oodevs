// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "GaugeFactory.h"
#include "Gauge.h"
#include "GaugeTypes.h"
#include "tools/GeneralConfig.h"

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: GaugeFactory constructor
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
GaugeFactory::GaugeFactory()
    : types_( new GaugeTypes() )
{
    types_->Load( tools::GeneralConfig::BuildResourceChildFile( "IndicatorGaugeTemplates.xml" ) );
}

// -----------------------------------------------------------------------------
// Name: GaugeFactory destructor
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
GaugeFactory::~GaugeFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GaugeFactory::Create
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
Gauge* GaugeFactory::Create( xml::xistream& xis ) const
{
    Gauge* gauge = new Gauge( xis, *types_ );
    return gauge;
}
