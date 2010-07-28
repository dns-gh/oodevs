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
    const std::string& MakeDirectories( const std::string& dir )
    {
        boost::filesystem::create_directories( dir );
        return dir;
    }
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
    : outputDir_( MakeDirectories( outputDir ) )
    , xis_      ( inputFile )
    , warnings_ ( ( outputDir + "/traduction.log" ).c_str() )
    , mapping_  ( GetMappingFile( rootDir, exerciseFile ), warnings_ )
    , plan_     ( 0 )
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
                >> xml::end
                >> xml::start( "weather" )
                >> xml::end
                >> xml::start( "mission" )
                >> xml::end
            >> xml::end
            >> xml::start( "terrain" )
            >> xml::end
            >> xml::start( "sides" )
                >> xml::start( "diplomacy-relationships" )
                    >> xml::list( "relations", *this, &Parser::ReadRelation )
                >> xml::end
                >> xml::list( "content", *this, &Parser::ReadSide )
            >> xml::end
            >> xml::start( "plan" )
                >> xml::start( "content" )
                    >> xml::start( "sides" )
                        >> xml::list( "relations", *this, &Parser::ReadRelation )
                        >> xml::start( "content" )
                            >> xml::start( "order-of-battle" )
                                >> xml::start( "content" )
                                    >> xml::start( "members" )
                                        >> xml::list( "content", *this, &Parser::PlaceEntity )
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
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadRelation
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Parser::ReadRelation( xml::xistream& xis )
{
    std::string source;
    xis >> xml::start( "source-side-ref" )
            >> xml::content( "id", source )
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
    xis >> xml::content( "name", name )
        >> xml::optional >> xml::start( "beginTrigger" )
            >> xml::optional >> xml::content( "date-trigger", date )
        >> xml::end
        >> xml::start( "sides" )
            >> xml::list( "content", *this, &Parser::ReadPlanDatum )
        >> xml::end;
    if( name.empty() )
        name = "Temps de manoeuvre " + boost::lexical_cast< std::string >( plan_ );
    date = date.substr( 0, 19 );
    xml::xofstream xos( outputDir_ + "/" + name + ".ord" );
    xos << xml::start( "actions" );
    xis >> xml::start( "sides" )
            >> xml::start( "content" )
                >> xml::start( "order-of-battle" )
                    >> xml::start( "content" )
                        >> xml::start( "members" )
                            >> xml::list( "content", *this, &Parser::WriteMissionInOrd, xos, name, date );
    if( plan_ ) // don't teleport units when they are already placed
        xis                >> xml::list( "content", *this, &Parser::WriteUnitInOrd, xos, date );
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
    xis >> xml::start( "data" )
            >> xml::optional >> xml::start( "tacticals" )
                >> xml::list( "content", *this, &Parser::ReadTactical )
            >> xml::end
            >> xml::optional >> xml::start( "missions" )
                >> xml::list( "content", *this, &Parser::ReadMission );
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
        >> xml::list( "content", *this, &Parser::ReadTacticalPointList, positionsList );
    tacticals_[ id ] = positionsList;
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadTacticalPointList
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Parser::ReadTacticalPointList( xml::xistream& xis, std::vector< std::vector< Position > >& positionsList )
{
    std::vector< Position > positions;
    xis >> xml::list( "points", *this, &Parser::ReadTacticalPoint, positions );
    positionsList.push_back( positions );
}

// -----------------------------------------------------------------------------
// Name: Parser::ReadTacticalPoint
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Parser::ReadTacticalPoint( xml::xistream& xis, std::vector< Position >& positions )
{
    xis >> xml::start( "gdc" );
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
        >> xml::start( "entity-ref" )
            >> xml::content( "id", missionId )
        >> xml::end
        >> xml::optional >> xml::start( "tacticals" )
            >> xml::start( "tac-data-ref" ) // $$$$ LDC : xml::list... there may be several args.
                >> xml::content( "id", tacId );
    missions_[ id ] = Mission( missionId, tacticals_[ tacId ], mapping_ );
}

// -----------------------------------------------------------------------------
// Name: Parser::WriteUnitInOrd
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Parser::WriteUnitInOrd( xml::xistream& xis, xml::xosubstream xos, const std::string& date )
{
    xis >> xml::optional >> xml::start( "members" )
            >> xml::list( "content", *this, &Parser::WriteUnitInOrd, xos, date )
        >> xml::end;
    std::string id;
    xis >> xml::start( "entity-ref" )
            >> xml::content( "id", id )
        >> xml::end
        >> xml::start( "location" )
            >> xml::start( "gdc" );
    const Position position( xis );
    xos << xml::start( "action" )
            << xml::attribute( "id", "teleport" )
            << xml::attribute( "name", "" )
            << xml::attribute( "target", mapping_[ id ] )
            << xml::attribute( "time", date )
            << xml::attribute( "type", "magicunit" )
            << xml::start( "parameters" )
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
    xis >> xml::optional >> xml::start( "members" )
            >> xml::list( "content", *this, &Parser::WriteMissionInOrd, xos, timeName, date )
        >> xml::end
        >> xml::start( "entity-ref" )
            >> xml::content( "id", id )
        >> xml::end
        >> xml::optional >> xml::start( "missionId"  )
            >> xml::content( "id", missionId )
        >> xml::end;
    if( missionId.empty() )
        return;
    if( missions_.find( missionId ) != missions_.end() )
    {
        xos << xml::start( "action" )
                << xml::attribute( "target", mapping_[ id ] )
                << xml::attribute( "time", date )
                << xml::attribute( "type", "mission" )
                << missions_[ missionId ]
            << xml::end;
    }
    else
        warnings_.Add( missionId );
}

// -----------------------------------------------------------------------------
// Name: Parser::PlaceEntity
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Parser::PlaceEntity( xml::xistream& xis )
{
    std::string id;
    xis  >> xml::start( "entity-ref" )
            >> xml::content( "id", id )
         >> xml::end
         >> xml::start( "location" )
            >> xml::start( "gdc"  );
    Position position( xis );
    xis  >> xml::end;
    Entity* entity = Entity::Find( id );
    if( entity )
        entity->SetPosition( position );
    else
        warnings_.Add( id );
    xis >> xml::optional >> xml::start( "members" )
            >> xml::list( "content", *this, &Parser::PlaceEntity );
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
                    >> xml::start( "beginTrigger" )
                        >> xml::optional >> xml::content( "date-trigger", date );
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
