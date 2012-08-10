// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ConsignResolver_ABC.h"
#include <boost/filesystem/operations.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/Formation_ABC.h"

using namespace plugins::logistic;
namespace bpt = boost::posix_time;
namespace bfs = boost::filesystem;
namespace bg = boost::gregorian;

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
ConsignResolver_ABC::ConsignResolver_ABC( const std::string& name, const std::string& header, const dispatcher::Model_ABC& model )
    : name_( name ), header_( header ), curTick_( 0 ), model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC destructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
ConsignResolver_ABC::~ConsignResolver_ABC()
{
    for ( std::map< int, ConsignData_ABC* >::iterator it = consignsData_.begin(); it != consignsData_.end(); ++it )
        delete it->second;
    consignsData_.clear();

    if( output_.is_open() )
        output_.close();
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::Receive
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::Receive( const sword::SimToClient& message )
{
    if( !IsManageable( message ) )
        return;
    CheckOutputFile();
    if( output_.is_open() )
        ManageMessage( message );
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::AppendDateWithExtension
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::AppendDateWithExtension( std::string& fileName, const bg::date& d )
{
    fileName += "." + to_iso_string( d ) + ".csv";
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::CheckOutputFile
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::CheckOutputFile()
{
    bg::date today = bpt::second_clock::local_time().date();
    if( fileDate_ != today )
    {
        if( output_.is_open() )
            output_.close();
        fileDate_ = today;
        fileName_ = name_;
        AppendDateWithExtension( fileName_, today );
        std::string fileDayBeforeYesterday( name_ );
        AppendDateWithExtension( fileDayBeforeYesterday, today - bg::days( 2 ) );
        try
        {
            if( bfs::exists( fileDayBeforeYesterday ) )
                bfs::remove( fileDayBeforeYesterday );
        }
        catch( ... )
        {
            // NOTHING
        }
    }

    if( !fileName_.empty() )
    {
        try
        {
            if( bfs::exists( fileName_ ) )
            {
                if( !output_.is_open() )
                    output_.open( fileName_, std::ios_base::out | std::ios_base::app );
            }
            else
            {
                if( output_.is_open() )
                    output_.close();
                output_.open( fileName_, std::ios_base::out | std::ios_base::app );
                output_ << header_ << std::endl;
            }
        }
        catch( ... )
        {
            // NOTHING
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::SetTime
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::SetTime( int tick, const std::string& simTime )
{
    curTick_ = tick;
    simTime_ = simTime;
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetAgentName
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetAgentName( int id, std::string& name )
{
    dispatcher::Agent_ABC* pAgent = model_.Agents().Find( static_cast< unsigned int >( id ) );
    if( pAgent )
        pAgent->GetExtension( "NomLong" , name );
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetAutomatName
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetAutomatName( int id, std::string& name )
{
    dispatcher::Automat_ABC* pAutomat = model_.Automats().Find( static_cast< unsigned int >( id ) );
    if( pAutomat )
        pAutomat->GetExtension( "NomLong" , name );
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetFormationName
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetFormationName( int id, std::string& name )
{
    dispatcher::Formation_ABC* pFormation = model_.Formations().Find( static_cast< unsigned int >( id ) );
    if( pFormation )
        pFormation->GetExtension( "NomLong" , name );
}