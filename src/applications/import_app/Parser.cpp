// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Parser.h"

#include "Position.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace
{
    std::string GetMappingFile( const std::string& rootDir, const std::string& exerciseFile )
    {
        xml::xifstream xis( rootDir + exerciseFile );
        std::string data;
        std::string physical;
        xis >> xml::start( "exercise" )
            >> xml::start( "model" )
            >> xml::attribute( "dataset", data )
            >> xml::attribute( "physical", physical );
        return rootDir + "/data/models/" + data + "/physical/" + physical + "/mapping.xml";
    }
}

// -----------------------------------------------------------------------------
// Name: Parser constructor
// Created: LDC 2010-07-06
// -----------------------------------------------------------------------------
Parser::Parser( const std::string& inputFile, const std::string& rootDir, const std::string& outputFile )
: outDir_( rootDir + outputFile )
, xis_( inputFile )
, xos_( outDir_ + "/orbat.xml" )
, xosWeather_ ( outDir_ + "/weather.xml" )
, mapping_( GetMappingFile( rootDir, outputFile + "/exercise.xml" ) )
, plan_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Parser destructor
// Created: LDC 2010-07-06
// -----------------------------------------------------------------------------
Parser::~Parser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Parser::Generate
// Created: LDC 2010-07-06
// -----------------------------------------------------------------------------
void Parser::Generate()
{
    plan_ = 0;
    xis_ >> xml::start( "scenario" )
            >> xml::start( "identification" )
                >> xml::start( "terrain" )
                >> xml::end()
                >> xml::start( "weather" )
                >> xml::end()
                >> xml::start( "mission" )
                >> xml::end()
            >> xml::end()
            >> xml::start( "terrain" )
            >> xml::end();
    ReadSides();
    ReadPlan();
    xis_ >> xml::end();
    WriteWeather();
    WriteOrbat();
    mapping_.LogWarnings( std::cerr );
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadRelations
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Parser::ReadRelations()
{
   xis_ >> xml::start( "ns5:diplomacy-relationships" )
            >> xml::list( "ns5:relations", *this, &Parser::ReadRelation )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadRelation
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Parser::ReadRelation( xml::xistream& xis )
{
    std::string source;
    std::string target;
    std::string type;
    xis >> xml::start( "ns5:source-side-ref" )
            >> xml::content( "ns2:id", source )
         >> xml::end()
         >> xml::start( "ns5:target-side-ref" )
            >> xml::content( "ns2:id", target )
         >> xml::end()
         >> xml::start( "ns5:relationship" )
            >> xml::start( "ns2:value" )
                >> xml::content( "ns2:enumeration-value", type )
            >> xml::end()
         >> xml::end();
    diplomacies_[ source ].push_back( Diplomacy( target, type ) );
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadSides
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Parser::ReadSides()
{
    xis_  >> xml::start( "sides" );
    ReadRelations();
    xis_ >> xml::list( "ns5:content", *this, &Parser::ReadSide );
    xis_ >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadSide
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Parser::ReadSide( xml::xistream& xis )
{
    std::string id, name;
    xis >> xml::attribute( "id", id )
        >> xml::content( "ns2:name", name );
    unsigned int sideID = mapping_.AddId( id );
    sides_[ id ] = Side( sideID, name, mapping_ );
    sides_[ id ].ReadObjects( xis );
    sides_[ id ].ReadTactical( xis );
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadPlan
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Parser::ReadPlan()
{
    xis_ >> xml::start( "plan" )
            >> xml::start( "content" )
                >> xml::start( "ns6:sides" )
                    >> xml::list( "ns5:relations", *this, &Parser::ReadRelation )
                    >> xml::start( "ns6:content" )
                        >> xml::start( "ns6:order-of-battle" )
                            >> xml::start( "ns6:content" )
                                >> xml::start( "ns6:members" )
                                    >> xml::list( "ns6:content", *this, &Parser::PlaceEntity )
                                >> xml::end()
                            >> xml::end()
                        >> xml::end()
                    >> xml::end()
                >> xml::end()
            >> xml::end()
            >> xml::list( "content", *this, &Parser::ReadNextPlan )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadNextPlan
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Parser::ReadNextPlan( xml::xistream& xis )
{
    if( plan_ )
    {
        std::string name;
        std::string date( "2000-01-01T12:00:00" );
        xis >> xml::content( "ns2:name", name );
        if( name.empty() )
            name = std::string( "Temps de manoeuvre " ) + boost::lexical_cast< std::string >( plan_ );
        xis >> xml::optional() >> xml::start( "ns6:beginTrigger" )
                >> xml::optional() >> xml::content( "ns6:date-trigger", date )
            >> xml::end();
        date = date.substr( 0, 19 );
        xml::xofstream xos( outDir_ + "/" + name + ".ord" );
        xos << xml::start( "actions" );
        xis >> xml::start( "ns6:sides" )
                >> xml::start( "ns6:content" )
                    >> xml::start( "ns6:order-of-battle" )
                        >> xml::start( "ns6:content" )
                            >> xml::start( "ns6:members" )
                                >> xml::list( "ns6:content", *this, &Parser::WriteUnitInOrd, xos, name, date )
                            >> xml::end()
                        >> xml::end()
                    >> xml::end()
                >> xml::end()
            >> xml::end();
        xos << xml::end();
    }
    ++plan_;
}

// -----------------------------------------------------------------------------
// Name: Parser::WriteUnitInOrd
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Parser::WriteUnitInOrd( xml::xistream& xis, xml::xostream& xos, const std::string& timeName, const std::string& date )
{
    xis >> xml::optional() >> xml::start( "ns6:members" )
            >> xml::list( "ns6:content", *this, &Parser::WriteUnitInOrd, xos, timeName, date )
        >> xml::end();
    std::string id;
    xis >> xml::start( "ns6:entity-ref" )
            >> xml::content( "ns2:id", id )
        >> xml::end()
        >> xml::start( "ns6:location" )
            >> xml::start( "ns4:gdc"  );
    Position position( xis );
    xis     >> xml::end()
        >> xml::end();
    xos << xml::start( "action" )
            << xml::attribute( "id", "teleport" )
            << xml::attribute( "name", timeName )
            << xml::attribute( "target", mapping_[ id ] )
            << xml::attribute( "time", date )
            << xml::attribute( "type", "magicunit" )
            << xml::start( "parameter" )
                << xml::attribute( "name", "Location" )
                << xml::attribute( "type", "point" )
                << xml::start( "location" )
                    << xml::attribute( "type", "point" )
                    << xml::start( "point" );
    position.WriteAttribute( "coordinates", xos );
    xos             << xml::end()
                << xml::end()
            << xml::end()
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Parser::PlaceEntity
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Parser::PlaceEntity( xml::xistream& xis )
{
    std::string id;
    xis  >> xml::start( "ns6:entity-ref" )
            >> xml::content( "ns2:id", id )
         >> xml::end()
         >> xml::start( "ns6:location" )
            >> xml::start( "ns4:gdc"  );
    Position position( xis );
    xis  >> xml::end();
    Entity* entity = Entity::Find( id );
    if( entity )
        entity->SetPosition( position );
    else
        mapping_.AddWarning( id );
    xis >> xml::optional() >> xml::start( "ns6:members" )
            >> xml::list( "ns6:content", *this, &Parser::PlaceEntity )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Parser::WriteWeather
// Created: LDC 2010-07-06
// -----------------------------------------------------------------------------
void Parser::WriteWeather()
{
    xis_ >> xml::start( "scenario" )
            >> xml::start( "plan" )
                >> xml::start( "content" )
                    >> xml::start( "ns6:beginTrigger" )
                        >> xml::optional()
                        >> xml::content( "ns6:date-trigger", dateScenario_ )
                    >> xml::end()
                >> xml::end()
            >> xml::end()
        >> xml::end();
    std::string formattedDate = dateScenario_.substr( 0, 4 ) + dateScenario_.substr( 5, 2 ) + dateScenario_.substr( 8, 5 ) + dateScenario_.substr( 14, 2 ) + dateScenario_.substr( 17, 2 );
    xosWeather_ << xml::start( "weather" )
        << xml::start( "exercise-date" )
            << xml::attribute( "value", formattedDate ) 
        << xml::end()
        << xml::start( "ephemerides" )
            << xml::attribute( "moon", "JourSansNuage")
            << xml::attribute( "sunrise", "7h30m0s" )
            << xml::attribute( "sunset", "21h30m0s" )
        << xml::end()
        << xml::start( "theater" )
            << xml::start( "wind" )
                << xml::attribute( "direction", 0 )
                << xml::attribute( "speed", 0 )
            << xml::end()
            << xml::start( "cloud-cover" )
                << xml::attribute( "ceiling", 10000 )
                << xml::attribute( "density", 0 )
                << xml::attribute( "floor", 1000 )
            << xml::end()
            << xml::start( "precipitation" )
                << xml::attribute( "value", "PasDePrecipitation" )
            << xml::end()
        << xml::end()
        << xml::start( "local-weather" )
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Parser::WriteOrbat
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Parser::WriteOrbat()
{
    xos_ << xml::start( "orbat" )
            << xml::start( "dotations" )
                << xml::attribute( "infinite", false )
            << xml::end();
    WriteSides();
    WriteDiplomacies();  
}

// -----------------------------------------------------------------------------
// Name: Parser::WriteSides
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Parser::WriteSides()
{
    xos_ << xml::start( "sides" );
    for( std::map< std::string, Side >::const_iterator it = sides_.begin(); it != sides_.end(); ++it )
        it->second.Write( xos_ );
    xos_ << xml::end();  
}

// -----------------------------------------------------------------------------
// Name: Parser::WriteDiplomacies
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Parser::WriteDiplomacies()
{
    xos_ << xml::start( "diplomacies" );
    for(std::map< std::string, std::vector< Diplomacy > >::const_iterator it = diplomacies_.begin(); it != diplomacies_.end(); ++it )
    {
        xos_ << xml::start( "side" )
            << xml::attribute( "id", mapping_[ it->first ] );
        for( std::vector< Diplomacy >::const_iterator itDiplomacy = it->second.begin(); itDiplomacy != it->second.end(); ++itDiplomacy )
            itDiplomacy->Write( xos_, mapping_ );
        xos_ << xml::end();
    }
    xos_ << xml::end();
}
