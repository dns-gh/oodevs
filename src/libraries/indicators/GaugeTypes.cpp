// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "GaugeTypes.h"
#include "Gauge.h"
#include "GaugeType.h"
#include "GaugeTypeFactory.h"
#include <xeumeuleu/xml.h>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: GaugeTypes constructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
GaugeTypes::GaugeTypes()
    : factory_( new GaugeTypeFactory() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GaugeTypes destructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
GaugeTypes::~GaugeTypes()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: GaugeTypes::Load
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void GaugeTypes::Load( const std::string& filename )
{
    xml::xifstream xis( filename );
    xis >> xml::start( "templates" )
            >> xml::list( "template", *this, &GaugeTypes::ReadTemplate )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: GaugeTypes::ReadTemplate
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void GaugeTypes::ReadTemplate( xml::xistream& xis )
{
    if( GaugeType* type = factory_->Create( xis ) )
        Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: GaugeTypes::Purge
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void GaugeTypes::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: GaugeTypes::Create
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
Gauge* GaugeTypes::Create() const
{
    tools::Iterator< const GaugeType& > it( CreateIterator() );
    if( !it.HasMoreElements() )
        throw std::runtime_error( __FUNCTION__ " no indicator gauge template." );
    Gauge* gauge = new Gauge( it.NextElement() );
    return gauge;
}

// -----------------------------------------------------------------------------
// Name: GaugeTypes::Create
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
Gauge* GaugeTypes::Create( xml::xistream& xis ) const
{
    Gauge* gauge = new Gauge( xis, *this );
    return gauge;
}
