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
MT_FileLogger::MT_FileLogger( const char* strFileName , unsigned int maxFiles, int maxSize, int nLogLevels, bool bClearPreviousLog, E_Type type )
    : MT_Logger_ABC( nLogLevels, type )
    , fileName_( strFileName )
    , bClearPreviousLog_( bClearPreviousLog )
    , maxFiles_( maxFiles < 1 ? 1 : maxFiles )
    , maxSize_( maxSize )
    , filesCount_( 1 )
    , sizeCount_( 0 )
{
    bfs::path pathFileName( fileName_ );
    fileNameNoExtension_ = pathFileName.parent_path().string() + "/" + pathFileName.stem();
    fileNameExtension_ = pathFileName.extension();
    OpenNewOfstream( fileName_ );
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

//-----------------------------------------------------------------------------
// Name: MT_FileLogger OpenNewOfstream
// Created:  MMC 2012-01-31
//-----------------------------------------------------------------------------
void MT_FileLogger::OpenNewOfstream( const std::string fileName )
{
    if ( file_.get() )
        file_->close();

    file_.reset( new std::ofstream );
    if( bClearPreviousLog_ )
        file_->open( fileName, std::ios::out | std::ios::trunc );
    else
        file_->open( fileName, std::ios::out | std::ios::app );
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

    int messageSize = static_cast< int >( messageStream.str().size() );
    sizeCount_ += messageSize;
    if( maxSize_ > 0 && sizeCount_ > maxSize_ )
    {
        ++filesCount_;
        sizeCount_ = messageSize;
        if ( filesCount_ > maxFiles_ )
            filesCount_ = 1;

        if( filesCount_ == 1 )
            OpenNewOfstream( fileName_ );
        else
            OpenNewOfstream( fileNameNoExtension_ + ( "." + boost::lexical_cast< std::string >( filesCount_ - 1 ) ) + fileNameExtension_ );       
    }
    *file_ << messageStream.str();
    file_->flush();
}
