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
#include <xeumeuleu/xml.h>

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
void ScoreDefinitions::Update( const MsgsAarToClient::MsgAarInformation& asnMsg )
{
    try
    {
        xml::xistringstream xis( asnMsg.information() );
        xis >> xml::start( "scores" )
                >> xml::list( "score", *this, &ScoreDefinitions::ReadDefinition )
            >> xml::end();
    }
    catch( std::exception& e )
    {
        std::cout << asnMsg.information() << std::endl << std::endl;
        std::cout << "Error reading indicator definitions: " << e.what() << std::endl;
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
        std::auto_ptr< ScoreDefinition > def( new ScoreDefinition( xis, primitives_, gaugeFactory_ ) );
        Register( def->GetName(), *def.release() );
    }
    catch( ... ) {}
}
