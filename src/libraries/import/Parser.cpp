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
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

namespace
{
    std::string GetMappingFile( const std::string& rootDir, const std::string& exerciseFile )
    {
        std::string data, physical;
        xml::xifstream xis( rootDir + exerciseFile );
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
Parser::Parser( const std::string& inputFile, const std::string& rootDir, const std::string& exerciseFile, const std::string& outputDir )
    : outputDir_( outputDir )
    , xis_      ( inputFile )
    , log_      ( ( outputDir + "/traduction.log" ).c_str() )
    , mapping_  ( GetMappingFile( rootDir, exerciseFile ) )
    , plan_     ( 0 )
{
    boost::filesystem::create_directories( outputDir );
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
                >> xml::end
                >> xml::start( "weather" )
                >> xml::end
                >> xml::start( "mission" )
                >> xml::end
            >> xml::end
            >> xml::start( "terrain" )
            >> xml::end
            >> xml::start( "sides" )
                >> xml::start( "ns5:diplomacy-relationships" )
                    >> xml::list( "ns5:relations", *this, &Parser::ReadRelation )
                >> xml::end
                >> xml::list( "ns5:content", *this, &Parser::ReadSide )
            >> xml::end
            >> xml::start( "plan" )
                >> xml::start( "content" )
                    >> xml::start( "ns6:sides" )
                        >> xml::list( "ns5:relations", *this, &Parser::ReadRelation )
                        >> xml::start( "ns6:content" )
                            >> xml::start( "ns6:order-of-battle" )
                                >> xml::start( "ns6:content" )
                                    >> xml::start( "ns6:members" )
                                        >> xml::list( "ns6:content", *this, &Parser::PlaceEntity )
                                    >> xml::end
                                >> xml::end
                            >> xml::end
                        >> xml::end
                    >> xml::end
                >> xml::end
                >> xml::list( "content", *this, &Parser::ReadNextPlan )
            >> xml::end
         >> xml::end;
    WriteWeather( xis_ );
    WriteOrbat();
    mapping_.LogWarnings( log_ );
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadRelation
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Parser::ReadRelation( xml::xistream& xis )
{
    std::string source;
    xis >> xml::start( "ns5:source-side-ref" )
            >> xml::content( "ns2:id", source )
         >> xml::end;
    diplomacies_[ source ].push_back( Diplomacy( xis, mapping_ ) );
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadSide
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Parser::ReadSide( xml::xistream& xis )
{
    std::string id;
    xis >> xml::attribute( "id", id );
    sides_[ id ] = Side( xis, mapping_ );
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadNextPlan
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Parser::ReadNextPlan( xml::xistream& xis )
{
    std::string name;
    std::string date( "2000-01-01T12:00:00" );
    xis >> xml::content( "ns2:name", name )
        >> xml::optional >> xml::start( "ns6:beginTrigger" )
            >> xml::optional >> xml::content( "ns6:date-trigger", date )
        >> xml::end
        >> xml::start( "ns6:sides" )
            >> xml::list( "ns6:content", *this, &Parser::ReadPlanDatum )
        >> xml::end;
    if( name.empty() )
        name = "Temps de manoeuvre " + boost::lexical_cast< std::string >( plan_ ); // $$$$ MCO : shouldn't the prefix be TM ? or default from XSD ?
    date = date.substr( 0, 19 );
    xml::xofstream xos( outputDir_ + "/" + name + ".ord" );
    xos << xml::start( "actions" );
    xis >> xml::start( "ns6:sides" )
            >> xml::start( "ns6:content" )
                >> xml::start( "ns6:order-of-battle" )
                    >> xml::start( "ns6:content" )
                        >> xml::start( "ns6:members" )
                            >> xml::list( "ns6:content", *this, &Parser::WriteMissionInOrd, xos, name, date );
    if( plan_ ) // don't teleport units when they are already placed
        xis                >> xml::list( "ns6:content", *this, &Parser::WriteUnitInOrd, xos, name, date );
    xis                 >> xml::end
                    >> xml::end
                >> xml::end
            >> xml::end
        >> xml::end;
    xos << xml::end;
    ++plan_;
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadPlanDatum
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Parser::ReadPlanDatum( xml::xistream& xis )
{
    xis >> xml::start( "ns6:data" )
            >> xml::optional >> xml::start( "ns6:tacticals" )
                >> xml::list( "ns6:content", *this, &Parser::ReadTactical )
            >> xml::end
            >> xml::optional >> xml::start( "ns6:missions" )
                >> xml::list( "ns6:content", *this, &Parser::ReadMission );
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadTactical
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Parser::ReadTactical( xml::xistream& xis )
{
    std::string id;
    std::vector< std::vector< Position > > positionsList;
    xis >> xml::attribute( "id", id )
        >> xml::list( "ns6:content", *this, &Parser::ReadTacticalPointList, positionsList );
    tacticals_[ id ] = positionsList;
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadTacticalPointList
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Parser::ReadTacticalPointList( xml::xistream& xis, std::vector< std::vector< Position > >& positionsList )
{
    std::vector< Position > positions;
    xis >> xml::list( "ns6:points", *this, &Parser::ReadTacticalPoint, positions );
    positionsList.push_back( positions );
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadTacticalPoint
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Parser::ReadTacticalPoint( xml::xistream& xis, std::vector< Position >& positions )
{
    xis >> xml::start( "ns4:gdc" );
    positions.push_back( Position( xis ) );
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadMission
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Parser::ReadMission( xml::xistream& xis )
{
    std::string id, tacId, missionId;
    xis >> xml::attribute( "id", id )
        >> xml::start( "ns6:entity-ref" )
            >> xml::content( "ns2:id", missionId )
        >> xml::end
        >> xml::optional >> xml::start( "ns6:tacticals" )
            >> xml::start( "ns6:tac-data-ref" ) // xml::list... there may be several args.
                >> xml::content( "ns2:id", tacId );
    missions_[ id ] = Mission( missionId, tacticals_[ tacId ], mapping_ );
}

// -----------------------------------------------------------------------------
// Name: Parser::WriteUnitInOrd
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Parser::WriteUnitInOrd( xml::xistream& xis, xml::xosubstream xos, const std::string& timeName, const std::string& date )
{
    xis >> xml::optional >> xml::start( "ns6:members" )
            >> xml::list( "ns6:content", *this, &Parser::WriteUnitInOrd, xos, timeName, date )
        >> xml::end;
    std::string id;
    xis >> xml::start( "ns6:entity-ref" )
            >> xml::content( "ns2:id", id )
        >> xml::end
        >> xml::start( "ns6:location" )
            >> xml::start( "ns4:gdc" );
    const Position position( xis );
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
                    << xml::start( "point" )
                        << xml::attribute( "coordinates", position );
}

// -----------------------------------------------------------------------------
// Name: Parser::WriteMissionInOrd
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Parser::WriteMissionInOrd( xml::xistream& xis, xml::xostream& xos, const std::string& timeName, const std::string& date )
{
    std::string id, missionId;
    xis >> xml::optional >> xml::start( "ns6:members" )
            >> xml::list( "ns6:content", *this, &Parser::WriteMissionInOrd, xos, timeName, date )
        >> xml::end
        >> xml::start( "ns6:entity-ref" )
            >> xml::content( "ns2:id", id )
        >> xml::end
        >> xml::optional >> xml::start( "ns6:missionId"  )
            >> xml::content( "ns2:id", missionId )
        >> xml::end;
    if( !missionId.empty() && missions_.find( missionId ) != missions_.end() )
    {
        xos << xml::start( "action" )
                << xml::attribute( "target", mapping_[ id ] )
                << xml::attribute( "time", date )
                << xml::attribute( "type", "mission" )
                << missions_[ missionId ]
            << xml::end;
    } // $$$$ MCO : else warning ?
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
         >> xml::end
         >> xml::start( "ns6:location" )
            >> xml::start( "ns4:gdc"  );
    Position position( xis );
    xis  >> xml::end;
    Entity* entity = Entity::Find( id );
    if( entity )
        entity->SetPosition( position );
    else
        mapping_.AddWarning( id );
    xis >> xml::optional >> xml::start( "ns6:members" )
            >> xml::list( "ns6:content", *this, &Parser::PlaceEntity );
}

// -----------------------------------------------------------------------------
// Name: Parser::WriteWeather
// Created: LDC 2010-07-06
// -----------------------------------------------------------------------------
void Parser::WriteWeather( xml::xisubstream xis )
{
    std::string date;
    xis >> xml::start( "scenario" )
            >> xml::start( "plan" )
                >> xml::start( "content" )
                    >> xml::start( "ns6:beginTrigger" )
                        >> xml::optional >> xml::content( "ns6:date-trigger", date );
    const std::string formattedDate = date.substr( 0, 4 ) + date.substr( 5, 2 ) + date.substr( 8, 5 ) + date.substr( 14, 2 ) + date.substr( 17, 2 );
    xml::xofstream xos( outputDir_ + "/weather.xml" );
    xos << xml::start( "weather" ) // $$$$ MCO : whole lotta hard-coded stuff
        << xml::start( "exercise-date" )
            << xml::attribute( "value", formattedDate )
        << xml::end
        << xml::start( "ephemerides" )
            << xml::attribute( "moon", "JourSansNuage")
            << xml::attribute( "sunrise", "7h30m0s" )
            << xml::attribute( "sunset", "21h30m0s" )
        << xml::end
        << xml::start( "theater" )
            << xml::start( "wind" )
                << xml::attribute( "direction", 0 )
                << xml::attribute( "speed", 0 )
            << xml::end
            << xml::start( "cloud-cover" )
                << xml::attribute( "ceiling", 10000 )
                << xml::attribute( "density", 0 )
                << xml::attribute( "floor", 1000 )
            << xml::end
            << xml::start( "precipitation" )
                << xml::attribute( "value", "PasDePrecipitation" )
            << xml::end
        << xml::end
        << xml::start( "local-weather" );
}

// -----------------------------------------------------------------------------
// Name: Parser::WriteOrbat
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Parser::WriteOrbat()
{
    xml::xofstream xos( outputDir_ + "/orbat.xml" );
    xos << xml::start( "orbat" )
            << xml::start( "dotations" )
                << xml::attribute( "infinite", false )
            << xml::end
            << xml::start( "sides" );
    for( std::map< std::string, Side >::const_iterator it = sides_.begin(); it != sides_.end(); ++it )
        xos << it->second;
    xos    << xml::end
            << xml::start( "diplomacies" );
    for( std::map< std::string, std::vector< Diplomacy > >::const_iterator it = diplomacies_.begin(); it != diplomacies_.end(); ++it )
    {
        xos << xml::start( "side" )
                << xml::attribute( "id", mapping_[ it->first ] );
        for( std::vector< Diplomacy >::const_iterator itDiplomacy = it->second.begin(); itDiplomacy != it->second.end(); ++itDiplomacy )
            xos << *itDiplomacy;
        xos << xml::end;
    }
}
