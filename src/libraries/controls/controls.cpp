// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#include "controls.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include "proto/controls.pb.h"
#include "proto/controls.pb.cc"
#ifdef _MSC_VER
#pragma warning( pop )
#endif

#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/interprocess/streams/vectorstream.hpp>

namespace tic = timeline::controls;
namespace bip = boost::interprocess;

namespace
{
    template< typename T >
    size_t Marshall( void* dst, size_t size, const T& src )
    {
        const size_t need = src.ByteSize();
        if( !dst )
            return need;
        if( need > size )
            return SIZE_MAX;
        bip::obufferstream stream( reinterpret_cast< char* >( dst ), size );
        src.SerializeToOstream( &stream );
        return need;
    }

    template< typename T >
    void Unmarshall( T& dst, const void* src, size_t size )
    {
        bip::ibufferstream stream( reinterpret_cast< const char* >( src ), size );
        dst.ParseFromIstream( &stream );
    }
}

size_t tic::ResizeClient( void* data, size_t size )
{
    timeline::ClientCommand cmd;
    cmd.set_type( timeline::CLIENT_RESIZE );
    return Marshall( data, size, cmd );
}

size_t tic::QuitClient( void* data, size_t size )
{
    timeline::ClientCommand cmd;
    cmd.set_type( timeline::CLIENT_QUIT );
    return Marshall( data, size, cmd );
}

void tic::ParseClient( Handler_ABC& handler, const void* data, size_t size )
{
    timeline::ClientCommand cmd;
    Unmarshall( cmd, data, size );
    switch( cmd.type() )
    {
        case timeline::CLIENT_RESIZE: return handler.OnResizeClient();
        case timeline::CLIENT_QUIT:   return handler.OnQuitClient();
    }
}