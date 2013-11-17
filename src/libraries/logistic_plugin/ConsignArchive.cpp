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

tools::Path ConsignArchive::GetFilename( uint32_t index ) const
{
    std::stringstream ss;
    ss << "." << index;
    return basePath_ + tools::Path::FromUTF8( ss.str() );
}

ConsignArchive::Offset ConsignArchive::Write( const void* data, uint32_t length )
{
    if( !output_ )
    {
        output_.reset( new tools::Ofstream( GetFilename( ++index_ ),
                    std::ios::out | std::ios::binary | std::ios::trunc ));
        uint8_t version = 0;
        output_->write( reinterpret_cast< const char* >( &version ), sizeof( version ) );
        size_ = sizeof( version );
    }

    Offset offset;
    offset.file = index_;
    offset.offset = size_;

    const uint32_t len = htonl( length );
    output_->write( reinterpret_cast< const char* >( &len ), sizeof( len ) );
    output_->write( reinterpret_cast< const char* >( data ), length );

    const uint32_t oldSize = size_;
    size_ = size_ + length + sizeof( len );
    if( size_ >= maxSize_ || size_ < oldSize )
    {
        output_.reset();
        size_ = 0;
    }

    return offset;
}

bool ConsignArchive::Read( uint32_t file, uint32_t offset, std::vector< uint8_t >& output ) const
{
    // Not awesome, but I am not sure caching files would help much. It would
    // be better to batch requests per files, in order to avoid seeking, short
    // of delegating all this to a proper database.
    tools::Ifstream fp( GetFilename( file ), std::ios::in | std::ios::binary );    
    if( !fp.is_open() )
        return false;
    fp.seekg( offset, std::ios::beg );
    uint32_t length = 0;
    fp.read( reinterpret_cast< char* >( &length ), sizeof( length ) );
    if( fp.gcount() != sizeof( length ) || !fp || !length )
        return false;
    length = ntohl( length );
    output.resize( length );
    fp.read( reinterpret_cast< char* >( &output[0] ), length );
    if( fp.gcount() != length || !fp )
        return false;
    return true;
}

void ConsignArchive::Flush()
{
    if( output_ )
        output_->flush();
}
