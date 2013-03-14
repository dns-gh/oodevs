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
#include "tools/FileWrapper.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#ifdef WIN32
#   define snprintf _snprintf
#endif

//-----------------------------------------------------------------------------
// Name: MT_FileLogger constructor
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
MT_FileLogger::MT_FileLogger( const tools::Path& strFileName , unsigned int maxFiles, int maxSize, int nLogLevels, bool bClearPreviousLog, E_Type type, bool sizeInBytes )
    : MT_Logger_ABC( nLogLevels, type )
    , fileName_( strFileName )
    , maxFiles_( maxFiles < 1 ? 1 : maxFiles )
    , maxSize_( maxSize )
    , filesCount_( 1 )
    , sizeCount_( 0 )
    , sizeInBytes_( sizeInBytes )
{
    fileNameNoExtension_ = fileName_.Parent() / fileName_.BaseName();
    fileNameExtension_ = fileName_.Extension();
    filesCount_ = GetOldestFile();
    sizeCount_ = static_cast< int >( OpenNewOfstream( GetFileName( filesCount_ ), bClearPreviousLog ) );
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
    unsigned int ComputeLines( const tools::Path& fileName )
    {
        unsigned int count = 0;
        tools::Ifstream file( fileName );
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
unsigned int MT_FileLogger::OpenNewOfstream( const tools::Path& fileName, bool clearPrevious /* = true */ )
{
    unsigned int size = 0;
    if ( file_.get() )
        file_->close();

    if( clearPrevious || maxFiles_ > 1 )
        file_.reset( new tools::Ofstream( fileName, std::ios::out | std::ios::trunc ) );
    else
    {
        try
        {
            if( maxSize_ > 0 && fileName.Exists() )
            {
                if( sizeInBytes_ )
                    size = fileName.FileSize();
                else
                    size = ComputeLines( fileName );
            }
        }
        catch( ... ) 
        { size = 0; }

        file_.reset( new tools::Ofstream( fileName, std::ios::out | std::ios::app ) );
    }
    return size;
}

void MT_FileLogger::WriteString( const std::string& s )
{
    boost::mutex::scoped_lock locker( mutex_ );
    if( maxSize_ > 0 )
    {
        int messageSize = sizeInBytes_ ? static_cast< int >( s.size() ) : 1;
        sizeCount_ += messageSize;
        if( sizeCount_ > maxSize_ )
        {
            if ( ++filesCount_ > maxFiles_ )
                filesCount_ = 1;
            sizeCount_ = messageSize + static_cast< int >( OpenNewOfstream( GetFileName( filesCount_ ) ) );
        }
    }
    *file_ << s;
    file_->flush();
}

//-----------------------------------------------------------------------------
// Name: MT_FileLogger::GetOldestFile
// Created:  MMC 2012-03-02
//-----------------------------------------------------------------------------
tools::Path MT_FileLogger::GetFileName( unsigned int fileCount ) const
{
    if( fileCount <= 1 )
        return fileName_;
    return fileNameNoExtension_ + std::string( "." + boost::lexical_cast< std::string >( fileCount - 1 ) ).c_str() + fileNameExtension_;
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
        tools::Path fileName = GetFileName( 1 );
        if( fileName.Exists() )
        {
            std::time_t minTime = fileName.LastWriteTime();
            for( unsigned int i = 2; i <= maxFiles_; ++i )
            {
                fileName = GetFileName( i );
                if( fileName.Exists() )
                {
                    std::time_t curTime = fileName.LastWriteTime();
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
