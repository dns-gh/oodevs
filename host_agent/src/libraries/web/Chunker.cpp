// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Chunker_ABC.h"

#include "Reply_ABC.h"

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <sstream>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4702 )
#endif
#include <boost/iostreams/stream.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace web;

namespace
{
static const std::string crlf = "\r\n";

// -----------------------------------------------------------------------------
// Name: Sink
// Created: BAX 2012-09-11
// -----------------------------------------------------------------------------
struct Sink : public boost::iostreams::sink
{
    Sink( Reply_ABC& res )
        : res_  ( &res )
        , write_( false)
    {
        // NOTHING
    }

    virtual ~Sink()
    {
        if( !write_ )
            return;
        std::stringstream chunk;
        chunk << 0 << crlf << crlf;
        WriteChunk( chunk );
    }

    void WriteChunk( const std::stringstream& chunk )
    {
        const std::string data = chunk.str();
        res_->Write( data.c_str(), data.size() );
    }

    std::streamsize write( const void* data, size_t size )
    {
        if( !size )
            return 0;
        write_ = true;
        std::stringstream chunk;
        chunk << std::hex << size << crlf;
        chunk.write( reinterpret_cast< const char* >( data ), size );
        chunk << crlf;
        WriteChunk( chunk );
        return size;
    }

private:
    // we save res_ as a pointer as boost::iostreams copy it everywhere
    Reply_ABC* res_;
    bool write_;
};

// -----------------------------------------------------------------------------
// Name: Chunker
// Created: BAX 2012-09-11
// -----------------------------------------------------------------------------
struct Chunker : public Chunker_ABC
{
    Chunker( Reply_ABC& rpy )
        : rpy_( rpy )
        , io_ ( rpy )
    {
        rpy_.SetStatus( web::OK );
        rpy_.SetHeader( "Content-Type", "application/zip" );
        rpy_.SetHeader( "Transfer-Encoding", "chunked" );
        rpy_.SetHeader( "Connection", "Close" );
    }

    ~Chunker()
    {
        io_.flush();
    }

    std::ostream& SetName( const std::string& name )
    {
        rpy_.SetHeader( "Content-Disposition", "attachment; filename=\"" + name + ".zip\"" );
        rpy_.WriteHeaders();
        return io_;
    }

    Reply_ABC& rpy_;
    boost::iostreams::stream< Sink > io_;
};
}

// -----------------------------------------------------------------------------
// Name: web::MakeChunker
// Created: BAX 2012-09-11
// -----------------------------------------------------------------------------
boost::shared_ptr< Chunker_ABC > web::MakeChunker( Reply_ABC& rpy )
{
    return boost::make_shared< Chunker >( rpy );
}
