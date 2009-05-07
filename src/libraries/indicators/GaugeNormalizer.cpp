// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "GaugeNormalizer.h"
#include "GaugeType.h"
#include <boost/foreach.hpp>
#include <limits>
#include <xeumeuleu/xml.h>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: GaugeNormalizer constructor
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
GaugeNormalizer::GaugeNormalizer( xml::xistream& xis )
{
    xis >> xml::list( "interval", *this, &GaugeNormalizer::LoadInterval );
}

// -----------------------------------------------------------------------------
// Name: GaugeNormalizer constructor
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
GaugeNormalizer::GaugeNormalizer( const GaugeNormalizer& source )
    : intervals_( source.intervals_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GaugeNormalizer destructor
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
GaugeNormalizer::~GaugeNormalizer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GaugeNormalizer::LoadInterval
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
void GaugeNormalizer::LoadInterval( xml::xistream& xis )
{
    const double min = xml::attribute< double >( xis, "min", std::numeric_limits< double >::min() );
    const double max = xml::attribute< double >( xis, "max", std::numeric_limits< double >::max() );
    const double key = xml::attribute< double >( xis, "key" );
    intervals_[ std::make_pair( min, max ) ] = key;
}

// -----------------------------------------------------------------------------
// Name: GaugeNormalizer::Normalize
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
double GaugeNormalizer::Normalize( double value ) const
{
    BOOST_FOREACH( const T_Intervals::value_type& interval, intervals_ )
        if( value > interval.first.first && value <= interval.first.second )
            return interval.second;
    return value;
}
