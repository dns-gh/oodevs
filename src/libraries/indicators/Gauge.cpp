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
#include <xeumeuleu/xml.hpp>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: Gauge constructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
Gauge::Gauge( xml::xistream& xis, const GaugeTypes& types )
    : type_( types.Get( xis.attribute< std::string >( "type" ).c_str() ) )
    , normalizer_( new GaugeNormalizer( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Gauge constructor
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
Gauge::Gauge( const GaugeType& type )
    : type_( type )
    , normalizer_( new GaugeNormalizer() )
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
// Name: Gauge::GetType
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
const GaugeType& Gauge::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Gauge::GetNormalizer
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
const GaugeNormalizer& Gauge::GetNormalizer() const
{
    if( !normalizer_.get() )
        throw MASA_EXCEPTION( "Invalid gauge normalizer." );
    return *normalizer_;
}

// -----------------------------------------------------------------------------
// Name: Gauge::SetNormalizer
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void Gauge::SetNormalizer( const GaugeNormalizer& normalizer )
{
    normalizer_.reset( new GaugeNormalizer( normalizer ) );
}

// -----------------------------------------------------------------------------
// Name: Gauge::Display
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void Gauge::Display( kernel::Displayer_ABC& displayer, double value ) const
{
    type_.Display( displayer, normalizer_->Normalize( value ) );
}

// -----------------------------------------------------------------------------
// Name: Gauge::Display
// Created: JSR 2012-10-25
// -----------------------------------------------------------------------------
void Gauge::Display( QTreeWidgetItem* item, kernel::DisplayExtractor_ABC& extractor, int col, double value ) const
{
    type_.Display( item, extractor, col, normalizer_->Normalize( value ) );
}

// -----------------------------------------------------------------------------
// Name: Gauge::Serialize
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void Gauge::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "type", type_.GetName() );
    GetNormalizer().Serialize( xos );
}
