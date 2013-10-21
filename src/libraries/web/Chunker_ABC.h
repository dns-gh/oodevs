// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CHUNKER_ABC_H
#define CHUNKER_ABC_H

#include <boost/noncopyable.hpp>
#include <string>

namespace boost
{
    template< typename T > class shared_ptr;
}

namespace io
{
    struct Writer_ABC;
}

namespace web
{
    struct Reply_ABC;
}

namespace web
{
// -----------------------------------------------------------------------------
// Name: Chunker_ABC
// Created: BAX 2012-09-11
// -----------------------------------------------------------------------------
struct Chunker_ABC : public boost::noncopyable
{
             Chunker_ABC() {}
    virtual ~Chunker_ABC() {}
    virtual void            SetHeader( const std::string& key, const std::string& value ) = 0;
    virtual void            SetName( const std::string& name ) = 0;
    virtual io::Writer_ABC& OpenWriter() = 0;
};

boost::shared_ptr< Chunker_ABC > MakeChunker( Reply_ABC& rpy );
}

#endif // CHUNKER_ABC_H
