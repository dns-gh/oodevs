// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "RotatingLog.h"
#include "LogFactory_ABC.h"
#include "Log_ABC.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: RotatingLog constructor
// Created: MCO 2011-06-26
// -----------------------------------------------------------------------------
RotatingLog::RotatingLog( tools::LogFactory_ABC& factory, const tools::Path& filename, std::size_t files, std::size_t size )
    : factory_ ( factory )
    , filename_( filename )
    , files_   ( files )
    , file_    ( FindOldestFile() )
    , size_    ( size )
    , count_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RotatingLog destructor
// Created: MCO 2013-08-20
// -----------------------------------------------------------------------------
RotatingLog::~RotatingLog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RotatingLog::DoWrite
// Created: MCO 2011-06-26
// -----------------------------------------------------------------------------
void RotatingLog::DoWrite( const std::string& line )
{
    if( ! pLog_.get() )
        pLog_ = factory_.CreateLog( GetFileName( file_ ), count_ );
    count_ += pLog_->Write( line );
    if( size_ == 0 )
        return;
    if( count_ >= size_ )
    {
        pLog_.reset();
        count_ = 0;
        if( ++file_ >= files_ )
            file_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: RotatingLog::GetFileName
// Created: MCO 2013-08-02
// -----------------------------------------------------------------------------
tools::Path RotatingLog::GetFileName( std::size_t file ) const
{
    if( file < 1 )
        return filename_;
    return filename_.Parent() / filename_.BaseName() +
                    "." + tools::Path::FromUTF8( boost::lexical_cast< std::string >( file ) ) + filename_.Extension();
}

//-----------------------------------------------------------------------------
// Name: RotatingLog::FindOldestFile
// Created:  MMC 2012-03-02
//-----------------------------------------------------------------------------
std::size_t RotatingLog::FindOldestFile() const
{
    if( ! filename_.Exists() )
        return 0;
    std::size_t oldest = 0;
    std::time_t minTime = filename_.LastWriteTime();
    for( std::size_t i = 1; i < files_; ++i )
    {
        const tools::Path filename = GetFileName( i );
        if( ! filename.Exists() )
            return i;
        const std::time_t curTime = filename.LastWriteTime();
        if( curTime < minTime )
        {
            minTime = curTime;
            oldest = i;
        }
    }
    return oldest;
}
