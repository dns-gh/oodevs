// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************
#include "Facade.h"
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem.hpp>

#pragma warning( push )
#pragma warning( disable : 4244 4245 )
#include <boost/crc.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;
using namespace orbat_generator;

// -----------------------------------------------------------------------------
// Name: Facade constructor
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
Facade::Facade( xml::xistream& xis, bfs::path base )
{
    std::string type = xis.attribute< std::string >( "type", "error" );
    xml::xifstream physXis( ( base / "physical.xml" ).string() );
    std::string automateFile;
    std::string crowdFile;
    unsigned int partyQty;
    std::vector< std::string > formationsRecord;
    unsigned int unitQty;
    unsigned int automateQty;
    unsigned int formationQty;
    unsigned int crowdQty;
    physXis >> xml::start( "physical" )
              >> xml::start( "automats" )
                >> xml::attribute( "file", automateFile )
              >> xml::end
              >> xml::start( "populations" )
                >> xml::attribute( "file", crowdFile );
    xml::xifstream automateXis( ( base / automateFile ).string() );
    xml::xifstream crowdXis( ( base / crowdFile ).string() );
    RetrieveData( xis, partyQty, formationQty, automateQty, unitQty, crowdQty, formationsRecord );
    if( partyQty > formationQty )
        partyQty = formationQty;
    if( formationQty > automateQty )
        formationQty = automateQty;
    for( unsigned int it = 0; it < partyQty; ++it )
        parties_.push_back(  boost::shared_ptr< Party >( new Party( static_cast< PartySide >( it % 3 ) ) ) );
    for( unsigned int it = 0; it < formationQty; ++it )
        parties_[ it % partyQty ]->AddFormation( boost::shared_ptr< Formation >( new Formation( formationsRecord[ it ] ) ) );
    automateXis >> xml::start( "automats" )
                  >> xml::list( "automat", *this, &Facade::FillAutomate, automateQty, unitQty, type );
    crowdXis >> xml::start( "populations" )
                  >> xml::list( "population", *this, &Facade::FillCrowds, crowdQty );
    for( unsigned int it = 0; it < automates_.size(); ++it )
        parties_[ it % parties_.size() ]->AddAutomate( automates_[ it ] );
}

// -----------------------------------------------------------------------------
// Name: Facade destructor
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
Facade::~Facade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Facade::ListExcluded
// Created: RCD 2011-03-08
// -----------------------------------------------------------------------------
void Facade::ListExcluded( xml::xistream& xis )
{
    excludes_.push_back( xis.attribute< std::string >( "name", "none" ) );
}

// -----------------------------------------------------------------------------
// Name: Facade::RetrieveData
// Created: RCD 2011-03-08
// -----------------------------------------------------------------------------
void Facade::RetrieveData( xml::xistream& xis, unsigned int& partyQty, unsigned int& formationQty, unsigned int& automateQty,
                           unsigned int& unitQty, unsigned int& crowdQty, std::vector< std::string >& formationsRecord )
{
    std::string pattern;
    std::string crowdPattern;
    std::string automatePattern;
    std::string formationPattern;
    std::string partyPattern;
    std::string unitPattern;
    xis >> xml::start( "structure" )
          >> xml::attribute( "point", defaultPoint_ )
          >> xml::attribute( "pattern", pattern )
          >> xml::start( "party" )
            >> xml::attribute( "quantity", partyQty )
            >> xml::attribute( "pattern", partyPattern )
          >> xml::end
          >> xml::list( "formation", *this, &Facade::ListFormations, formationsRecord, formationPattern );
    formationQty = formationsRecord.size();
    xis >> xml::start( "automat" )
          >> xml::attribute( "quantity", automateQty )
          >> xml::attribute( "pattern", automatePattern )
        >> xml::end
        >> xml::start( "unit" )
          >> xml::attribute( "quantity", unitQty )
          >> xml::attribute( "pattern", unitPattern )
        >> xml::end
        >> xml::start( "crowd" )
          >> xml::attribute( "quantity", crowdQty )
          >> xml::attribute( "pattern", crowdPattern )
        >> xml::end
      >> xml::end
      >> xml::start( "excludes" )
        >> xml::list( "exclude", *this, &Facade::ListExcluded );
    idNameGen_.Init( pattern, partyPattern, formationPattern, automatePattern, unitPattern, crowdPattern );
}

// -----------------------------------------------------------------------------
// Name: Facade::FillCrowds
// Created: RCD 2011-03-08
// -----------------------------------------------------------------------------
void Facade::FillCrowds( xml::xistream& xis, unsigned int& crowdQty )
{
    bool excluded = false;
    for( unsigned int it = 0; it < excludes_.size(); ++it )
        excluded = ( excludes_[ it ] == xis.attribute( "name", "error" ) || excluded );
    if( crowdQty > 0 && !excluded )
    {
        crowds_.push_back( xis.attribute< std::string >( "name", "none" ) );
        --crowdQty;
    }
}

// -----------------------------------------------------------------------------
// Name: Facade::ListFormations
// Created: RCD 2011-03-04
// -----------------------------------------------------------------------------
void Facade::ListFormations( xml::xistream& xis, std::vector< std::string >& formationsRecord, std::string& formationPattern )
{
    std::string formationType = xis.attribute< std::string >( "level", "i" );
    for ( unsigned int it = 0; it < xis.attribute< unsigned int >( "quantity", 1 ); ++it )
        formationsRecord.push_back( xis.attribute< std::string >( "level", "o" ) );
    formationPattern = xis.attribute< std::string >( "pattern", "formation" );
}

// -----------------------------------------------------------------------------
// Name: Facade::FillAutomate
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
void Facade::FillAutomate( xml::xistream& xis, unsigned int& automateQty, unsigned int& pionQty, const std::string& odbType )
{
    bool excluded = false;
    for( unsigned int it = 0; it < excludes_.size(); ++it )
        excluded = ( excludes_[ it ] == xis.attribute( "name", "error" ) || excluded );
    if( automateQty > 0 && !excluded )
    {
        unsigned int pionsParAutomate = pionQty / automateQty;
        if ( pionQty % automateQty > 0 )
            ++pionsParAutomate;
        pionQty -= pionsParAutomate;
        std::string type = xis.attribute( "name", "Error" );
        pions_.clear();
        xis >> xml::list( "unit", *this, &Facade::FillPion, pionsParAutomate );
        automates_.push_back( boost::shared_ptr< Automate >( new Automate( type, pions_, odbType ) ) );
        pionQty += pionsParAutomate;
        pionQty <= 0 ? automateQty = 0 : --automateQty;
    }
}

// -----------------------------------------------------------------------------
// Name: Facade::FillPion
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
void Facade::FillPion( xml::xistream& xis, unsigned int& pionsParAutomate )
{
    bool excluded = false;
    for( unsigned int it = 0; it < excludes_.size(); ++it )
        excluded = ( excludes_[ it ] == xis.attribute( "type", "error" ) || excluded );
    if( !excluded )
    {
        std::string posi = defaultPoint_;
        unsigned int qty = xis.attribute< int >( "max-occurs", 1 );
        if ( qty > pionsParAutomate )
            qty = pionsParAutomate;
        std::string type = xis.attribute( "type", "Error" );
        bool posteDeCommandement = xis.attribute( "command-post", "Error" ) == "true";
        for( unsigned int it = 0; it < qty; ++it )
            if( qty > 0 )
            {
                pions_.push_back( boost::shared_ptr< Pion >( new Pion( posteDeCommandement, type, posi ) ) );
                --pionsParAutomate;
            }
    }
}

// -----------------------------------------------------------------------------
// Name: Facade::CreateOrbat
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
void Facade::CreateOrbat( xml::xostream& xos  )
{
    xos << xml::start( "orbat" )
            << xml::attribute( "model-version", "4.4.1" )
            << xml::attribute( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" )
            << xml::attribute( "xsi:noNamespaceSchemaLocation", "schemas/4.3.0/exercise/orbat.xsd" )
            << xml::start( "parties" );
    for( unsigned int it = 0; it < parties_.size(); it++ )
        parties_[ it ]->InsertIntoOrbat( xos, idNameGen_ );
    if( crowds_.size() > 0 )
        AddCrowds( xos );
    xos   << xml::end
            << xml::start( "diplomacy" );
    for( unsigned int it = 0; it < parties_.size(); it++ )
        parties_[ it ]->GenerateDiplomacy( xos, it, parties_ );
    xos     << xml::end
          << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Facade::AddCrowds
// Created: RCD 2011-03-14
// -----------------------------------------------------------------------------
void Facade::AddCrowds( xml::xostream& orbat )
{
        std::string name = idNameGen_.ComputePartyName();
        unsigned int id = idNameGen_.ComputePartyId();
        parties_.push_back(  boost::shared_ptr< Party >( new Party( static_cast< PartySide >( 2 ) ) ) );
        orbat << xml::start( "party" )
                << xml::attribute< unsigned int >( "id", id )
                << xml::attribute< std::string >( "name", name )
                << xml::attribute< std::string >( "type", "neutral" )
                << xml::start( "objects" )
                << xml::end
                << xml::start( "tactical" )
                << xml::end
                << xml::start( "communication" )
                  << xml::start( "knowledge-group" )
                    << xml::attribute< unsigned int >( "id", id * 1000 )
                    << xml::attribute< std::string >( "name", "Groupe de connaissance" )
                    << xml::attribute< std::string >( "type", "Standard" )
                  << xml::end
                << xml::end
                << xml::start( "populations" );
        for( unsigned int it = 0; it < crowds_.size(); ++it )
        {
            orbat << xml::start( "population" );
            name = idNameGen_.ComputeCrowdName();
            id = idNameGen_.ComputePionId();
            orbat         << xml::attribute< std::string >( "attitude", "calme" )
                          << xml::attribute< unsigned int >( "id", id )
                          << xml::attribute< std::string >( "name", name )
                          << xml::attribute< std::string >( "position", defaultPoint_ )
                          << xml::attribute< std::string >( "type", crowds_[ it ] )
                          << xml::start( "composition" )
                            << xml::attribute( "contaminated", 0 )
                            << xml::attribute( "dead", 0 )
                            << xml::attribute( "healthy", 1000 )
                            << xml::attribute( "wounded", 0 )
                          << xml::end
                        << xml::end;
        }
        orbat         << xml::end
                      << xml::start( "inhabitants" )
                      << xml::end
                      << xml::start( "logistics" )
                    << xml::end
                  << xml::end;
    }

// -----------------------------------------------------------------------------
// Name: Facade::ComputeSignature
// Created: RCD 2011-03-14
// -----------------------------------------------------------------------------
std::string Facade::ComputeSignature( std::istream& is )
{
    static const std::streamsize kBufferSize = 1024;
    static const int kSignatureSize = 31;
    boost::crc_32_type result;
    char buffer[ kBufferSize ];
    is.seekg( 0, std::ios_base::end );
    bool alreadySigned = false;
    int fileLength = is.tellg();
    if( fileLength >= kSignatureSize )
    {
        is.seekg( fileLength - kSignatureSize );
        is.read( buffer, kSignatureSize );
        std::string str( buffer, kSignatureSize );
        alreadySigned = str.find( "<!-- signature=\"" ) == 0 && str.find( "\" -->" ) == 26;
    }
    is.seekg( 0 );
    int sizeRead = 0;
    int dataLength = fileLength - ( alreadySigned ? kSignatureSize : 0 );
    while( is )
    {
        is.read( buffer, kBufferSize );
        int nCount = is.gcount();
        if( sizeRead + nCount > dataLength )
        {
            result.process_bytes( buffer, dataLength - sizeRead );
            break;
        }
        result.process_bytes( buffer, nCount);
        sizeRead += nCount;
    }
    is.clear();
    return ( boost::format( "<!-- signature=\"%010lu\" -->" ) % result.checksum() ).str();
}