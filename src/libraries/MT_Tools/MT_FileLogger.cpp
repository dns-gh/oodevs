// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_FileLogger.h"
#include <cstdio>
#include <ctime>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>

namespace bfs = boost::filesystem;

#ifdef WIN32
#   define snprintf _snprintf
#endif

//-----------------------------------------------------------------------------
// Name: MT_FileLogger constructor
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
MT_FileLogger::MT_FileLogger( const char* strFileName , unsigned int maxFiles, int maxSize, int nLogLevels, bool bClearPreviousLog, E_Type type, bool sizeInBytes )
    : MT_Logger_ABC( nLogLevels, type )
    , fileName_( strFileName )
    , maxFiles_( maxFiles < 1 ? 1 : maxFiles )
    , maxSize_( maxSize )
    , filesCount_( 1 )
    , sizeCount_( 0 )
    , sizeInBytes_( sizeInBytes )
{
    bfs::path pathFileName( fileName_ );
    fileNameNoExtension_ = pathFileName.parent_path().string() + "/" + pathFileName.stem().string();
    fileNameExtension_ = pathFileName.extension().string();
    filesCount_ = GetOldestFile();
    std::string startFileName = GetFileName( filesCount_ );
    sizeCount_ = static_cast< int >( OpenNewOfstream( startFileName, bClearPreviousLog ) );
}

//-----------------------------------------------------------------------------
// Name: MT_FileLogger constructor
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
MT_FileLogger::~MT_FileLogger()
{
    if ( file_.get() )
        file_->close();
}

namespace
{
    unsigned int ComputeLines( const std::string& fileName )
    {
        unsigned int count = 0;
        std::ifstream file( fileName.c_str() );
        if( file && !file.fail() )
        {
            std::string currentLine;
            while( std::getline( file, currentLine ) )
                ++count;
        }
        file.close();
        return count;
    }
}

//-----------------------------------------------------------------------------
// Name: MT_FileLogger OpenNewOfstream
// Created:  MMC 2012-01-31
//-----------------------------------------------------------------------------
unsigned int MT_FileLogger::OpenNewOfstream( const std::string& fileName, bool clearPrevious /* = true */ )
{
    unsigned int size = 0;
    if ( file_.get() )
        file_->close();

    file_.reset( new std::ofstream );
    if( clearPrevious || maxFiles_ > 1 )
        file_->open( fileName.c_str(), std::ios::out | std::ios::trunc );
    else
    {
        try
        {
            if( maxSize_ > 0 && bfs::exists( fileName ) )
            {
                if( sizeInBytes_ )
                    size = static_cast< unsigned int >( bfs::file_size( bfs::path( fileName ) ) );
                else
                    size = ComputeLines( fileName );
            }
        }
        catch( ... ) 
        { size = 0; }

        file_->open( fileName.c_str(), std::ios::out | std::ios::app );
    }
    return size;
}

//-----------------------------------------------------------------------------
// Name: MT_FileLogger::LogString
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
void MT_FileLogger::LogString( E_LogLevel nLogLevel, const char* strMessage, const char* strContext, int nCode )
{
    boost::mutex::scoped_lock locker( mutex_ );

    std::stringstream messageStream;
    MakeString( nLogLevel, strMessage, strContext, nCode, messageStream );

    if( maxSize_ > 0 )
    {
        int messageSize = sizeInBytes_ ? static_cast< int >( messageStream.str().size() ) : 1;
        sizeCount_ += messageSize;
        if( sizeCount_ > maxSize_ )
        {
            if ( ++filesCount_ > maxFiles_ )
                filesCount_ = 1;
            sizeCount_ = messageSize + static_cast< int >( OpenNewOfstream( GetFileName( filesCount_ ) ) );
        }
    }
    *file_ << messageStream.str();
    file_->flush();
}

//-----------------------------------------------------------------------------
// Name: MT_FileLogger::GetOldestFile
// Created:  MMC 2012-03-02
//-----------------------------------------------------------------------------
std::string MT_FileLogger::GetFileName( unsigned int fileCount )
{
    if( fileCount <= 1 )
        return fileName_;
    return fileNameNoExtension_ + ( "." + boost::lexical_cast< std::string >( fileCount - 1 ) ) + fileNameExtension_;
}

//-----------------------------------------------------------------------------
// Name: MT_FileLogger::GetOldestFile
// Created:  MMC 2012-03-02
//-----------------------------------------------------------------------------
unsigned int MT_FileLogger::GetOldestFile()
{
    if( maxFiles_ == 1 )
        return 1;

    unsigned int oldest = 1;
    try
    {
        std::string fileName = GetFileName( 1 );
        if( bfs::exists( fileName ) )
        {
            std::time_t minTime = bfs::last_write_time( fileName );
            for( unsigned int i = 2; i <= maxFiles_; ++i )
            {
                fileName = GetFileName( i );
                if( bfs::exists( fileName ) )
                {
                    std::time_t curTime = bfs::last_write_time( fileName );
                    if( curTime < minTime )
                    {
                        minTime = curTime;
                        oldest = i;
                    }
                }
                else
                {
                    oldest = i;
                    break;
                }
            }
        }
    }
    catch( ... ) { oldest = 1; }
    return oldest;
}
