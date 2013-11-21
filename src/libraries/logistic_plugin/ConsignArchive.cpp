// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "ConsignArchive.h"
#include <tools/StdFileWrapper.h>
#include <winsock2.h>
#include <sstream>

using namespace plugins::logistic;

ConsignArchive::ConsignArchive( const tools::Path& basePath, uint32_t maxSize )
    : basePath_( basePath )
    , maxSize_( maxSize )
    , size_( 0 )
    , index_( 0 )
{
}

ConsignArchive::~ConsignArchive()
{
}

void ConsignArchive::Write( const void* data, uint32_t length )
{
    if( !output_ )
    {
        std::stringstream ss;
        ss << "." << index_++;
        const tools::Path path = basePath_ + tools::Path::FromUTF8( ss.str() );
        output_.reset( new tools::Ofstream( path,
                    std::ios::out | std::ios::binary | std::ios::trunc ));
        uint8_t version = 0;
        output_->write( reinterpret_cast< const char* >( &version ), sizeof( version ) );
        size_ = sizeof( version );
    }

    const uint32_t len = htonl( length );
    output_->write( reinterpret_cast< const char* >( &len ), sizeof( len ) );
    output_->write( static_cast< const char* >( data ), length );

    const uint32_t oldSize = size_;
    size_ = size_ + length + sizeof( len );
    if( size_ >= maxSize_ || size_ < oldSize )
    {
        output_.reset();
        size_ = 0;
    }
}

void ConsignArchive::Flush()
{
    if( output_ )
        output_->flush();
}
