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
#include "indicators/GaugeFactory_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ScoreDefinitions constructor
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
ScoreDefinitions::ScoreDefinitions( const indicators::Primitives& primitives, const indicators::GaugeFactory_ABC& factory )
    : primitives_( primitives )
    , gaugeFactory_( factory )
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
// Name: ScoreDefinitions::Update
// Created: SBO 2009-12-18
// -----------------------------------------------------------------------------
void ScoreDefinitions::Update( const sword::AarInformation& asnMsg )
{
    try
    {
        xml::xistringstream xis( asnMsg.information() );
        xis >> xml::start( "scores" )
                >> xml::list( "score", *this, &ScoreDefinitions::ReadDefinition )
            >> xml::end;
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( asnMsg.information() << "Error reading indicator definitions: " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreDefinitions::ReadDefinition
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void ScoreDefinitions::ReadDefinition( xml::xistream& xis )
{
    try
    {
        std::unique_ptr< ScoreDefinition > def( new ScoreDefinition( xis, primitives_, gaugeFactory_ ) );
        Register( def->GetName(), *def.release() );
    }
    catch( ... ) {}
}
