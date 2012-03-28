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
    fileNameNoExtension_ = pathFileName.parent_path().string() + "/" + pathFileName.stem();
    fileNameExtension_ = pathFileName.extension();
    filesCount_ = GetOldestFile();
    std::string startFileName = GetFileName( filesCount_ );
    OpenNewOfstream( startFileName, bClearPreviousLog );
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
    unsigned int ComputeSize( const std::string& fileName )
    {
        unsigned int count = 0;
        boost::filesystem::path p( fileName ); 
        try
        {
            count = static_cast< unsigned int >( boost::filesystem::file_size( p ) );
        }
        catch( ... ) { count = 0; }
        return count;
    }

    unsigned int ComputeLines( const std::string& fileName )
    {
        unsigned int count = 0;
        try
        {
            std::ifstream file( fileName.c_str() );
            if( file && !file.fail() )
            {
                std::string currentLine;
                while( std::getline( file, currentLine ) )
                    ++count;
            }
        }
        catch( ... ) { count = 0; }
        return count;
    }
}

//-----------------------------------------------------------------------------
// Name: MT_FileLogger OpenNewOfstream
// Created:  MMC 2012-01-31
//-----------------------------------------------------------------------------
void MT_FileLogger::OpenNewOfstream( const std::string fileName, bool clearPrevious /* = true */ )
{
    if ( file_.get() )
        file_->close();

    file_.reset( new std::ofstream );
    if( clearPrevious || maxFiles_ > 1 )
        file_->open( fileName.c_str(), std::ios::out | std::ios::trunc );
    else
    {
        if( maxSize_ > 0 && bfs::exists( fileName ) )
        {
            if( sizeInBytes_ )
                sizeCount_ = static_cast< int >( ComputeSize( fileName ) );
            else
                sizeCount_ = static_cast< int >( ComputeLines( fileName ) );
        }

        file_->open( fileName.c_str(), std::ios::out | std::ios::app );
    }
}

//-----------------------------------------------------------------------------
// Name: MT_FileLogger::LogString
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
void MT_FileLogger::LogString( E_LogLevel nLogLevel, const char* strMessage, const char* strContext, int nCode )
{
    boost::mutex::scoped_lock locker( mutex_ );

    std::stringstream messageStream ( std::stringstream::in | std::stringstream::out );
    messageStream << "[" << GetTimestampAsString() << "] ";
    messageStream << "<" << GetTypeAsString() << "> ";
    messageStream << "<" << GetLogLevelAsString( nLogLevel ) << "> ";
    if( strMessage )
        messageStream << strMessage;
    if( nCode != -1 )
        messageStream << "(" << nCode << ") ";
    if( strContext )
        messageStream << "[Context: " << strContext << "]";
    messageStream << std::endl;

    if( maxSize_ > 0 )
    {
        int messageSize = sizeInBytes_ ? static_cast< int >( messageStream.str().size() ) : 1;
        sizeCount_ += messageSize;
        if( sizeCount_ > maxSize_ )
        {
            sizeCount_ = messageSize;
            if ( ++filesCount_ > maxFiles_ )
                filesCount_ = 1;
            OpenNewOfstream( GetFileName( filesCount_ ) );     
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
