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
#include <boost/make_shared.hpp>
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

ConsignOffset ConsignArchive::Write( const void* data, uint32_t length )
{
    if( !output_ )
    {
        output_.reset( new tools::Fstream( GetFilename( ++index_ ),
                    std::ios::out | std::ios::in | std::ios::binary | std::ios::trunc ));
        uint8_t version = 0;
        output_->write( reinterpret_cast< const char* >( &version ), sizeof( version ) );
        size_ = sizeof( version );
    }

    ConsignOffset offset;
    offset.file = index_;
    offset.offset = size_;

    const uint32_t len = htonl( length );
    // Need to seek before writing because of interleaved reads/writes
    output_->seekg( 0, output_->end );
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

boost::shared_ptr< tools::Fstream > ConsignArchive::GetFile( uint32_t index ) const
{
    if( index == index_ && output_ )
    {
        // If we do not reuse the current file we might race with data not
        // flushed to disk yet.
        return output_;
    }
    else
    {
        return boost::make_shared< tools::Fstream >( GetFilename( index ),
               std::ios::in | std::ios::binary );    
    }
}


void ConsignArchive::ReadMany( const std::vector< ConsignOffset >& offsets,
        const std::function< void( std::vector< uint8_t >& )>& callback ) const
{
    std::vector< uint8_t > output;

    uint32_t file = 0;
    boost::shared_ptr< tools::Fstream > fp;
    for( auto it = offsets.cbegin(); it != offsets.cend(); ++it )
    {
        if( file != it->file )
        {
            file = it->file;
            fp = GetFile( file );
        }
        if( !fp->is_open() )
            continue;
        fp->clear();
        fp->seekg( it->offset, std::ios::beg );
        uint32_t length = 0;
        fp->read( reinterpret_cast< char* >( &length ), sizeof( length ) );
        if( fp->gcount() != sizeof( length ) || !fp || !length )
            continue;
        length = ntohl( length );
        output.resize( length );
        fp->read( reinterpret_cast< char* >( &output[0] ), length );
        if( fp->gcount() != length || !fp )
            continue;
        callback( output );
    }
}

void ConsignArchive::Flush()
{
    if( output_ )
        output_->flush();
}
