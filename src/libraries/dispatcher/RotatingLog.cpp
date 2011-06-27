// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "RotatingLog.h"
#include "LogFactory_ABC.h"
#include "Log_ABC.h"
#include <boost/lexical_cast.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: RotatingLog constructor
// Created: MCO 2011-06-26
// -----------------------------------------------------------------------------
RotatingLog::RotatingLog( dispatcher::LogFactory_ABC& factory, const std::string& filename,
                          unsigned int files, unsigned int size )
    : factory_ ( factory )
    , filename_( filename )
    , files_   ( files )
    , file_    ( files )
    , size_    ( size )
    , count_   ( size )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RotatingLog::DoWrite
// Created: MCO 2011-06-26
// -----------------------------------------------------------------------------
void RotatingLog::DoWrite( const std::string& line )
{
    if( count_ == size_ )
    {
        count_ = 0;
        ++file_;
        if( file_ > files_ )
            file_ = 1;
        pLog_.reset();
        pLog_ = factory_.CreateLog( filename_ +
            ( file_ == 1 ? "" : ( "." + boost::lexical_cast< std::string >( file_ ) ) ) );
    }
    pLog_->Write( line );
    ++count_;
}
