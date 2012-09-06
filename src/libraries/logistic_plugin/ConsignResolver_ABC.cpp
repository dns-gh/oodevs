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
#include "clients_kernel/Tools.h"

using namespace plugins::logistic;
namespace bpt = boost::posix_time;
namespace bfs = boost::filesystem;
namespace bg = boost::gregorian;


namespace
{
    const int maxFileIndex = 1000;
    const int maxFileIndexToRemove = 20;
    const int maxFileDaysToRemove = 20;
    const int maxLines = 50000;
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
ConsignResolver_ABC::ConsignResolver_ABC( const std::string& name, const NameResolver_ABC& nameResolver )
    : name_( name ), curTick_( 0 ), nameResolver_( nameResolver ), curLineIndex_( 0 )
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
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::Receive
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
bool ConsignResolver_ABC::Receive( const sword::SimToClient& message )
{
    if( !IsManageable( message ) )
        return false;
    CheckOutputFile();
    if( output_.is_open() )
        ManageMessage( message );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetConsign
// Created: MMC 2012-08-23
// -----------------------------------------------------------------------------
ConsignData_ABC& ConsignResolver_ABC::GetConsign( int requestId )
{
    std::map< int, ConsignData_ABC* >::iterator it = consignsData_.find( requestId );
    if( it == consignsData_.end() )
    {
        ConsignData_ABC* pConsign = CreateConsignData( requestId );
        consignsData_[ requestId ] = pConsign;
        return *pConsign;
    }
    return *it->second;
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::DestroyConsignData
// Created: MMC 2012-09-03
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::DestroyConsignData( int requestId )
{
    std::map< int, ConsignData_ABC* >::iterator it = consignsData_.find( requestId );
    if( it != consignsData_.end() )
    {
        delete it->second;
        consignsData_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::AppendDateWithExtension
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::AppendDateWithExtension( std::string& fileName, const bg::date& d, int fileIndex )
{
    fileName += "." + to_iso_string( d ) + "." + boost::lexical_cast< std::string >( fileIndex ) + ".csv";
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::SetNewFile
// Created: MMC 2012-08-27
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::SetNewFile()
{
    bg::date today = bpt::second_clock::local_time().date();
    std::string newFileName;
    int fileIndex = 0;
    while( fileIndex < maxFileIndex )
    {
        newFileName = name_ ;
        AppendDateWithExtension( newFileName, today, fileIndex );
        try
        {
            if( !bfs::exists( newFileName ) )
                break;
        }
        catch( ... )
        {
            break;
        }
        ++fileIndex;
    }
    curLineIndex_ = 0;
    fileDate_ = today;
    fileName_ = newFileName;
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::RemoveOldFiles
// Created: MMC 2012-08-27
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::RemoveOldFiles()
{
    bg::date today = bpt::second_clock::local_time().date();
    for( int before = 2; before < maxFileDaysToRemove; ++before )
        for( int index = 0; index < maxFileIndexToRemove; ++index )
        {
            std::string fileDayBeforeYesterday( name_ );
            AppendDateWithExtension( fileDayBeforeYesterday, today - bg::days( before ), index );
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
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::OpenFile
// Created: MMC 2012-08-27
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::OpenFile()
{
    if( output_.is_open() )
        output_.close();
    if( !fileName_.empty() )
    {
        try
        {
            if( bfs::exists( fileName_ ) )
            {
                if( !output_.is_open() )
                    output_.open( fileName_.c_str(), std::ios_base::out | std::ios_base::app );
            }
            else
            {
                if( output_.is_open() )
                    output_.close();
                output_.open( fileName_.c_str(), std::ios_base::out | std::ios_base::app );
                output_ << header_;
            }
        }
        catch( ... )
        {
            // NOTHING
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::CheckOutputFile
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::CheckOutputFile()
{
    bg::date today = bpt::second_clock::local_time().date();
    if( fileDate_ != today || curLineIndex_ > maxLines )
    {
        SetNewFile();
        OpenFile();
        RemoveOldFiles();
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
// Name: ConsignResolver_ABC::GetSimTime
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetSimTime( std::string& simTime, std::string& tick ) const
{
    if( !simTime_.empty() )
        simTime = simTime_;
    if( curTick_ >= 0 )
        tick = boost::lexical_cast< std::string >( curTick_ );
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetNameResolver
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
const NameResolver_ABC& ConsignResolver_ABC::GetNameResolver() const
{
    return nameResolver_;
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::SetHeader
// Created: MMC 2012-08-23
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::SetHeader( const ConsignData_ABC& consign )
{
    std::stringstream header;
    consign >> header;
    header_ = header.str();
}
