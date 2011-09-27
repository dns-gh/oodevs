// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************
#include "orbat_generator_pch.h"
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

namespace
{
    std::string ReadFilename( xml::xisubstream xis, const std::string& node )
    {
        std::string result;
        xis >> xml::start( "physical" )
                >> xml::start( node )
                    >> xml::attribute( "file", result );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: Facade constructor
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
Facade::Facade( xml::xistream& xis, const bfs::path& base )
    : idNameGen_( xis )
{
    unsigned int unitQuantity = 0;
    unsigned int automateQuantity = 0;
    unsigned int crowdQuantity = 0;
    RetrieveData( xis, automateQuantity, unitQuantity, crowdQuantity );
    xml::xifstream physXis( ( base / "physical.xml" ).string() );
    const std::string automateFile = ReadFilename( physXis, "automats" );
    xml::xifstream automateXis( ( base / automateFile ).string() );
    ReadAutomates( automateXis, xis.attribute< std::string >( "type" ), automateQuantity, unitQuantity );
    const std::string crowdFile = ReadFilename( physXis, "populations" );
    xml::xifstream crowdXis( ( base / crowdFile ).string() );
    ReadCrowds( crowdXis, crowdQuantity );
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
void Facade::RetrieveData( xml::xisubstream xis, unsigned int& automateQuantity,
                           unsigned int& unitQuantity, unsigned int& crowdQuantity )
{
    unsigned int partyQuantity = 0;
    unsigned int formationQuantity = 0;
    unsigned int objectQuantity = 0;
    std::vector< std::string > formationsRecord;
	std::string firstCoord;
	std::string secondCoord;
	xis >> xml::start( "structure" )
          >> xml::start( "party" )
            >> xml::attribute( "quantity", partyQuantity )
          >> xml::end
          >> xml::start( "object" )
            >> xml::attribute( "quantity", objectQuantity )
          >> xml::end
          >> xml::list( "formation", *this, &Facade::ListFormations, formationsRecord );
    formationQuantity = formationsRecord.size();
    xis   >> xml::start( "automat" )
            >> xml::attribute( "quantity", automateQuantity )
          >> xml::end
          >> xml::start( "unit" )
            >> xml::attribute( "quantity", unitQuantity )
          >> xml::end
          >> xml::start( "crowd" )
            >> xml::attribute( "quantity", crowdQuantity )
          >> xml::end
        >> xml::end
        >> xml::start( "excludes" )
          >> xml::list( "exclude", *this, &Facade::ListExcluded );
    if( partyQuantity > formationQuantity )
        partyQuantity = formationQuantity;
    for( unsigned int it = 0; it < partyQuantity; ++it )
        parties_.push_back(  boost::shared_ptr< Party >( new Party( static_cast< PartySide >( it % 3 ) ) ) );
    if( formationQuantity > automateQuantity )
        formationQuantity = automateQuantity;
    for( unsigned int it = 0; it < formationQuantity; ++it )
        parties_[ it % partyQuantity ]->AddFormation( boost::shared_ptr< Formation >( new Formation( formationsRecord[ it ] ) ) );
}

// -----------------------------------------------------------------------------
// Name: Facade::ReadCrowds
// Created: HBD 2011-03-22
// -----------------------------------------------------------------------------
void Facade::ReadCrowds( xml::xisubstream xis, unsigned int crowdQuantity )
{
    xis >> xml::start( "populations" )
            >> xml::list( "population", *this, &Facade::FillCrowds );
    crowds_.resize( crowdQuantity );
    if( crowds_.size() > 0 )
        parties_.push_back(  boost::shared_ptr< Party >( new Party( static_cast< PartySide >( 2 ) ) ) );
}

// -----------------------------------------------------------------------------
// Name: Facade::FillCrowds
// Created: RCD 2011-03-08
// -----------------------------------------------------------------------------
void Facade::FillCrowds( xml::xistream& xis )
{
    const std::string name = xis.attribute< std::string >( "name" );
    if( std::find( excludes_.begin(), excludes_.end(), name ) == excludes_.end() )
        crowds_.push_back( name );
}

// -----------------------------------------------------------------------------
// Name: Facade::ListFormations
// Created: RCD 2011-03-04
// -----------------------------------------------------------------------------
void Facade::ListFormations( xml::xistream& xis, std::vector< std::string >& formationsRecord )
{
    const std::string formationType = xis.attribute< std::string >( "level", "i" );
    for ( unsigned int it = 0; it < xis.attribute< unsigned int >( "quantity", 1 ); ++it )
        formationsRecord.push_back( xis.attribute< std::string >( "level", "o" ) );
}

// -----------------------------------------------------------------------------
// Name: Facade::ReadAutomates
// Created: HBD 2011-03-22
// -----------------------------------------------------------------------------
void Facade::ReadAutomates( xml::xisubstream xis, const std::string& type, unsigned int automateQuantity, unsigned int unitQuantity )
{
    xis >> xml::start( "automats" )
            >> xml::list( "automat", *this, &Facade::FillAutomate, automateQuantity, unitQuantity, type );
    for( unsigned int it = 0; it < automates_.size(); ++it )
        parties_[ it % parties_.size() ]->AddAutomate( automates_[ it ] );
}

// -----------------------------------------------------------------------------
// Name: Facade::FillAutomate
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
void Facade::FillAutomate( xml::xistream& xis, unsigned int& automateQuantity, unsigned int& pionQuantity, const std::string& odbType )
{
    const std::string name = xis.attribute< std::string >( "name" );
    if( automateQuantity > 0 && std::find( excludes_.begin(), excludes_.end(), name ) == excludes_.end() )
    {
        unsigned int pionsParAutomate = pionQuantity / automateQuantity;
        if ( pionQuantity % automateQuantity > 0 )
            ++pionsParAutomate;
        pionQuantity -= pionsParAutomate;
        std::string type = xis.attribute( "name", "Error" );
        pions_.clear();
        xis >> xml::list( "unit", *this, &Facade::FillPion, pionsParAutomate );
        automates_.push_back( boost::shared_ptr< Automate >( new Automate( type, pions_, odbType ) ) );
        pionQuantity += pionsParAutomate;
        pionQuantity <= 0 ? automateQuantity = 0 : --automateQuantity;
    }
}

// -----------------------------------------------------------------------------
// Name: Facade::FillPion
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
void Facade::FillPion( xml::xistream& xis, unsigned int& pionsParAutomate )
{
    const std::string name = xis.attribute( "name", "Error" );
    if( std::find( excludes_.begin(), excludes_.end(), name ) == excludes_.end() )
    {
        unsigned int quantity = xis.attribute< int >( "max-occurs", 1 );
        if ( quantity > pionsParAutomate )
            quantity = pionsParAutomate;
        std::string type = xis.attribute< std::string >( "type" );
        bool commandPost = xis.attribute( "command-post", "false" ) == "true";
        for( unsigned int it = 0; it < quantity; ++it )
            if( quantity > 0 )
            {
                pions_.push_back( boost::shared_ptr< Pion >( new Pion( commandPost, type ) ) );
                --pionsParAutomate;
            }
    }
}

// -----------------------------------------------------------------------------
// Name: Facade::CreateOrbat
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
void Facade::CreateOrbat( xml::xostream& xos )
{
    idNameGen_.RewindAll();
    xos << xml::start( "orbat" )
            << xml::attribute( "model-version", "4.4.1" )
            << xml::attribute( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" )
            << xml::attribute( "xsi:noNamespaceSchemaLocation", "schemas/4.3.0/exercise/orbat.xsd" )
            << xml::start( "parties" );
    int sizeModificator;
    crowds_.size() > 0 ? sizeModificator = -1 : sizeModificator = 0;
    for( unsigned int it = 0; it < parties_.size() + sizeModificator; ++it )
        parties_[ it ]->InsertIntoOrbat( xos, idNameGen_ );
    if( crowds_.size() > 0 )
        AddCrowds( xos );
    xos   << xml::end
            << xml::start( "diplomacy" );
    for( unsigned int it = 0; it < parties_.size(); ++it )
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
        << xml::attribute< unsigned int >( "id", ( id +1 ) * 1000 )
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
            << xml::attribute< std::string >( "position", idNameGen_.ComputeCoord() )
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
std::string Facade::ComputeSignature( std::istream& is ) const
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