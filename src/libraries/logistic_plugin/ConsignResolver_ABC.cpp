// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ConsignResolver_ABC.h"
#include "clients_kernel/Tools.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

namespace bfs = boost::filesystem;
namespace bg = boost::gregorian;
namespace bpt = boost::posix_time;

namespace
{
    std::string EscapeRegex( const std::string& s )
    {
        std::string escaped;
        escaped.reserve( 2*s.size() );
        for( size_t i = 0; i != s.size(); ++i )
        {
            if( !std::isalnum( s[i] ))
                escaped.push_back( '\\' );
            escaped.push_back( s[i] );
        }
        return escaped;
    }

    void RemoveFiles( std::vector< bfs::path >& filesToRemove )
    {
        for( auto it = filesToRemove.begin(); it != filesToRemove.end(); ++it )
        {
            try
            {
                bfs::remove( *it );
            }
            catch( ... )
            {
                // NOTHING
            }
        }
    }

    int ReadTickFromLine( const std::string& line )
    {
        size_t first = line.find_first_of( ';' );
        size_t second = line.find_first_of( ';', first + 1 );
        // consign ; tick ; ... : Grab the number between '; ' and ' ;':
        std::string number = line.substr( first + 2, second - first -3 );
        return boost::lexical_cast< int, std::string >( number.c_str() );
    }

    int ReadFirstTick( std::ifstream& is )
    {
        try
        {
            std::string line;
            std::getline( is, line );
            //Skip first line
            std::getline( is, line );
            return ReadTickFromLine( line );
        }
        catch( ... )
        {
            return 0;
        }
    }

    int ReadLastTick( std::ifstream& is )
    {
        try
        {
            bool found = false;
            is.seekg( 0, std::ios_base::end );
            int size = (int)is.tellg();
            for( int i = size - 3; is.good() && !found; --i ) // -3 in order to get past the last \n.
            {
                is.seekg( i, std::ios_base::beg );
                bool good = is.good();
                char ch = is.get();
                bool good2 = is.good();
                if( ( int ) is.tellg() <= 1 )
                    return 0; // No usable line since first line is text, not numbers.
                else if( ch == '\n' )
                    found = true;
            }
            if( !is.good() )
                return 0;
            std::string lastLine;            
            std::getline( is, lastLine );
            return ReadTickFromLine( lastLine );
        }
        catch( ... )
        {
            return 0;
        }
    }

    void Trim( const bfs::path& fileToTrim, int tick )
    {
        std::ifstream is;
        is.open( fileToTrim.string().c_str() );
        std::string line;
        std::getline( is, line );
        std::stringstream os;
        int currentTick = 0;
        while( tick >= currentTick )
        {
            os << line;
            os << std::endl;
            std::getline( is, line );
            currentTick = ReadTickFromLine( line );
        }
        is.close();
        std::ofstream outfile;
        outfile.open( fileToTrim.string().c_str(), std::ios_base::out );
        outfile << os.str();
        outfile.close();
    }
}

namespace plugins
{
namespace logistic
{
    // -----------------------------------------------------------------------------
    // Name: ConsignResolver_ABC constructor
    // Created: MMC 2012-08-06
    // -----------------------------------------------------------------------------
    ConsignResolver_ABC::ConsignResolver_ABC( const std::string& name, const NameResolver_ABC& nameResolver, int maxHistoricFiles, int maxFileLines )
        : name_( name ), curTick_( 0 ), nameResolver_( nameResolver ), curFileIndex_( 0 ), curLineIndex_( 0 )
        , maxLinesInFile_( maxFileLines ), daysBeforeToKeep_( maxHistoricFiles )
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
    }
    
    // -----------------------------------------------------------------------------
    // Name: ConsignResolver_ABC::Receive
    // Created: LDC 2012-12-14
    // -----------------------------------------------------------------------------
    bool ConsignResolver_ABC::Receive( const sword::SimToClient& message )
    {
        if( IsManageable( message ) )
        {
            bg::date today = bpt::second_clock::local_time().date();
            return Receive( message, today );
        }
        return false;
    }

    // -----------------------------------------------------------------------------
    // Name: ConsignResolver_ABC::Receive
    // Created: MMC 2012-08-06
    // -----------------------------------------------------------------------------
    bool ConsignResolver_ABC::Receive( const sword::SimToClient& message, const boost::gregorian::date& today )
    {
        if( !IsEmptyLineMessage( message ) )
            CheckOutputFile( today );
        if( output_.is_open() )
        {
            try
            {
                ManageMessage( message );
            }
            catch ( ... )
            {
                // NOTHING
            }
        }
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
    void ConsignResolver_ABC::SetNewFile( const boost::gregorian::date& today )
    {
        if( today != fileDate_ )
        {
            if( fileDate_ == boost::gregorian::date() )
                InitFileIndex( today );
            else
                curFileIndex_ = 0;
        }
        fileDate_ = today;
        // Move current to newFileName
        if( output_.is_open() )
            output_.close();
        if( !fileName_.empty() && bfs::exists( fileName_ ) )
        {
            std::string newFileName( name_ );
            AppendDateWithExtension( newFileName, today, curFileIndex_ );
            curLineIndex_ = 0;
            bfs::path from( fileName_ );
            bfs::path to( newFileName );
            bfs::rename( from, to );
            curFileIndex_++;
        }
        fileName_ = name_ + "current.csv";
    }

    // -----------------------------------------------------------------------------
    // Name: ConsignResolver_ABC::ForceNewFile
    // Created: LDC 2012-12-14
    // -----------------------------------------------------------------------------
    void ConsignResolver_ABC::ForceNewFile()
    {
        bg::date today = bpt::second_clock::local_time().date();
        SetNewFile( today );
    }

    // -----------------------------------------------------------------------------
    // Name: ConsignResolver_ABC::InitFileIndex
    // Created: MMC 2012-09-12
    // -----------------------------------------------------------------------------
    void ConsignResolver_ABC::InitFileIndex( const boost::gregorian::date& today )
    {
        const bfs::path curPath( name_ );
        boost::regex todayRegex(
            EscapeRegex( curPath.filename() ) + "\\." + to_iso_string( today ) + "\\.(\\d+)\\.csv$");

        boost::smatch m;
        bfs::directory_iterator end;
        for( bfs::directory_iterator dir_it( curPath.parent_path() ) ; dir_it != end ; ++dir_it )
        {
            if( bfs::is_regular_file( dir_it->status() ) )
            {
                const std::string fileName = dir_it->path().filename();
                if(boost::regex_match( fileName, m, todayRegex ) )
                {
                    const int curIndex = boost::lexical_cast< int >( m.str( 1 ) ) + 1;
                    if( curIndex > curFileIndex_ )
                        curFileIndex_ = curIndex;
                }
            }
        }
    }

    // -----------------------------------------------------------------------------
    // Name: ConsignResolver_ABC::RemoveOldFiles
    // Created: MMC 2012-08-27
    // -----------------------------------------------------------------------------
    void ConsignResolver_ABC::RemoveOldFiles( const boost::gregorian::date& today )
    {
        const bfs::path curPath( name_ );
        boost::regex fileRegex( EscapeRegex( curPath.filename() ) + "\\.(\\d{8})\\.\\d+\\.csv$");
        const std::string minDate = to_iso_string( today - bg::days( daysBeforeToKeep_ ) );

        boost::smatch match;
        std::vector< bfs::path > filesToRemove;
        bfs::directory_iterator end;
        for( bfs::directory_iterator dir_it( curPath.parent_path() ) ; dir_it != end ; ++dir_it )
        {
            if( bfs::is_regular_file( dir_it->status() ) )
            {
                const std::string fileName = dir_it->path().filename();
                // ISO date lexicographic order matches the chronological one
                if( boost::regex_match( fileName, match, fileRegex ) && match.str(1) < minDate)
                    filesToRemove.push_back( dir_it->path() );
            }
        }
        RemoveFiles( filesToRemove );
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
    void ConsignResolver_ABC::CheckOutputFile( const boost::gregorian::date& today )
    {
        if( fileDate_ != today || curLineIndex_ >= maxLinesInFile_ || !output_.is_open() )
        {
            SetNewFile( today );
            OpenFile();
            RemoveOldFiles( today );
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

    // -----------------------------------------------------------------------------
    // Name: ConsignResolver_ABC::SetHeader
    // Created: MMC 2012-09-11
    // -----------------------------------------------------------------------------
    int ConsignResolver_ABC::GetConsignCount() const
    {
        return static_cast< int >( consignsData_.size() );
    }
    
    // -----------------------------------------------------------------------------
    // Name: ConsignResolver_ABC::ClearFilesAfterTick
    // Created: LDC 2013-02-21
    // -----------------------------------------------------------------------------
    void ConsignResolver_ABC::ClearFilesAfterTick( int tick )
    {
        const bfs::path curPath( name_ );
        std::vector< bfs::path > filesToRemove;
        bfs::directory_iterator end;
        bfs::path fileToTrim;
        int lastReadTick = 0;
        std::string regexp = EscapeRegex( curPath.filename() ) + ".*.csv$";
        boost::regex fileRegex( regexp );
        boost::smatch match;
        for( bfs::directory_iterator dir_it( curPath.parent_path() ) ; dir_it != end ; ++dir_it )
        {
            if( bfs::is_regular_file( dir_it->status() ) )
            {
                const std::string fileName = dir_it->path().filename();
                if( !boost::regex_match( fileName, match, fileRegex ) )
                    continue;
                std::ifstream is;
                is.open( ( curPath.parent_path() / fileName ).string().c_str() );
                int firstTick = ReadFirstTick( is );
                if( firstTick > tick )
                    filesToRemove.push_back( dir_it->path() );
                else
                {
                    if( firstTick > lastReadTick )
                    {
                        lastReadTick = ReadLastTick( is );
                        if( lastReadTick > tick )
                            fileToTrim = dir_it->path();
                    }
                }
                is.close();
            }
        }
        RemoveFiles( filesToRemove );
        if( bfs::exists( fileToTrim ) )
            Trim( fileToTrim, tick );
    }

}  // namespace logistic
}  // namespace plugins
