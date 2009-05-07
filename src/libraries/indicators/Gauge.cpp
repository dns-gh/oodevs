// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "Gauge.h"
#include "GaugeNormalizer.h"
#include "GaugeType.h"
#include "GaugeTypes.h"
#include <xeumeuleu/xml.h>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: Gauge constructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
Gauge::Gauge( xml::xistream& xis, const GaugeTypes& types )
    : type_( types.Get( xml::attribute< std::string >( xis, "type" ).c_str() ) )
    , normalizer_( new GaugeNormalizer( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Gauge constructor
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
Gauge::Gauge( const Gauge& gauge )
    : type_( gauge.type_ )
    , normalizer_( new GaugeNormalizer( *gauge.normalizer_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Gauge destructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
Gauge::~Gauge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Gauge::Display
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void Gauge::Display( kernel::Displayer_ABC& displayer, double value ) const
{
    type_.Display( displayer, normalizer_->Normalize( value ) );
}
