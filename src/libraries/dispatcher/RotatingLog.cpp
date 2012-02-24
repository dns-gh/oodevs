// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "RotatingLog.h"
#include "LogFactory_ABC.h"
#include "Log_ABC.h"
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>

using namespace dispatcher;
namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: RotatingLog constructor
// Created: MCO 2011-06-26
// -----------------------------------------------------------------------------
RotatingLog::RotatingLog( dispatcher::LogFactory_ABC& factory, const std::string& filename,
                          unsigned int files, unsigned int size, bool sizeInBytes /* = false */ )
    : factory_ ( factory )
    , filename_( filename )
    , files_   ( files )
    , file_    ( 1 )
    , size_    ( size )
    , count_   ( 0 )
    , sizeInBytes_( sizeInBytes )
{
    bfs::path pathFileName( filename_ );
    std::string pathName = pathFileName.parent_path().string();
    if( pathName.empty() )
        fileNameNoExtension_ = pathFileName.stem();
    else
        fileNameNoExtension_ = pathFileName.parent_path().string() + "/" + pathFileName.stem();
    extension_ = pathFileName.extension();
}

// -----------------------------------------------------------------------------
// Name: RotatingLog::CreateFileLog
// Created: MMC 2012-02-21
// -----------------------------------------------------------------------------
void RotatingLog::CreateFileLog()
{
    pLog_.reset();
    if( file_ == 1 )
        pLog_ = factory_.CreateLog( filename_ );
    else
        pLog_ = factory_.CreateLog( fileNameNoExtension_ + ( "." + boost::lexical_cast< std::string >( file_ - 1 ) ) + extension_ );
}

// -----------------------------------------------------------------------------
// Name: RotatingLog::DoWrite
// Created: MCO 2011-06-26
// -----------------------------------------------------------------------------
void RotatingLog::DoWrite( const std::string& line )
{
    if( sizeInBytes_ )
    {
        unsigned int messageSize = static_cast< unsigned int >( line.size() );
        count_ += messageSize;
        if ( !pLog_.get() )
            pLog_ = factory_.CreateLog( filename_ );
        else if( size_ > 0 && count_ > size_ )
        {
            count_ = messageSize;
            ++file_;
            if( file_ > files_ )
                file_ = 1;
            CreateFileLog();
        }
        pLog_->Write( line );
    }
    else
    {
        if( size_ > 0 && count_ >= size_ )
        {
            ++file_;
            if( file_ > files_ )
                file_ = 1;
        }
        if( !pLog_.get() )
            pLog_ = factory_.CreateLog( filename_ );
        else
            CreateFileLog();
        pLog_->Write( line );
        ++count_;
    }
}
