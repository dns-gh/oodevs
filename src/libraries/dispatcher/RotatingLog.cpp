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
// Name: RotatingLog::DoWrite
// Created: MCO 2011-06-26
// -----------------------------------------------------------------------------
void RotatingLog::DoWrite( const std::string& line )
{
    if( size_ > 0 )
    {
        unsigned int messageSize = sizeInBytes_ ? static_cast< unsigned int >( line.size() ) : 1;
        count_ += messageSize;
        if( count_ > size_ )
        {
            if( pLog_.get() )
                pLog_.reset();
            count_ = messageSize;
            if( ++file_ > files_ )
                file_ = 1;
            if( file_ == 1 )
                pLog_ = factory_.CreateLog( filename_ );
            else
                pLog_ = factory_.CreateLog( fileNameNoExtension_ + ( "." + boost::lexical_cast< std::string >( file_ - 1 ) ) + extension_ );
        }
    }
    if( !pLog_.get() )
        pLog_ = factory_.CreateLog( filename_ );
    pLog_->Write( line );
}
