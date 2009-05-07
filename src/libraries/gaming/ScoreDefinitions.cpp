// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ScoreDefinitions.h"
#include "ScoreDefinition.h"
#include "indicators/GaugeFactory.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: ScoreDefinitions constructor
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
ScoreDefinitions::ScoreDefinitions( const indicators::Primitives& primitives )
    : primitives_( primitives )
    , gaugeFactory_( new indicators::GaugeFactory() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoreDefinitions destructor
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
ScoreDefinitions::~ScoreDefinitions()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: ScoreDefinitions::Purge
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void ScoreDefinitions::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ScoreDefinitions::Load
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void ScoreDefinitions::Load( const std::string& file )
{
    xml::xifstream xis( file );
    xis >> xml::start( "scores" )
            >> xml::list( "score", *this, &ScoreDefinitions::ReadDefinition )
        >> xml::end();
}


// -----------------------------------------------------------------------------
// Name: ScoreDefinitions::ReadDefinition
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void ScoreDefinitions::ReadDefinition( xml::xistream& xis )
{
    try
    {
        std::auto_ptr< ScoreDefinition > def( new ScoreDefinition( xis, primitives_, *gaugeFactory_ ) );
        Register( def->GetName(), *def.release() );
    }
    catch( ... )
    {
        // $$$$ SBO 2009-04-29: scores file contains errors, try to continue
    }
}
