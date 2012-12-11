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
#include "tools/ExerciseConfig.h"
#include "tools/Loader_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

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
void GaugeTypes::Load( const tools::ExerciseConfig& config, const std::string& filename )
{
    config.GetLoader().LoadFile( filename, boost::bind( &GaugeTypes::Read, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: GaugeTypes::Load
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void GaugeTypes::Read( xml::xistream& xis )
{
    xis >> xml::start( "templates" )
            >> xml::list( "template", *this, &GaugeTypes::ReadTemplate )
        >> xml::end;
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
        throw MASA_EXCEPTION( "No indicator gauge template." );
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
