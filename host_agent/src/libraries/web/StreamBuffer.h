// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef STREAM_BUFFER_H
#define STREAM_BUFFER_H

#include <boost/noncopyable.hpp>
#include <string>
#include <vector>

namespace io
{
    struct Reader_ABC;
}

namespace web
{
// =============================================================================
/** @class  StreamBuffer
    @brief  StreamBuffer class definition
*/
// Created: BAX 2012-05-03
// =============================================================================
class StreamBuffer : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit StreamBuffer( io::Reader_ABC& src );
    virtual ~StreamBuffer();
    //@}

    //! @name Methods
    //@{
    bool        Eof();
    char        Peek();
    char        Get();
    size_t      Peek( char** dst );
    void        Skip( size_t offset );
    size_t      Read( char* dst, size_t size );
    std::string GetLine();
    //@}

private:
    //! @name Private Methods
    //@{
    void SkipLocally( size_t size );
    void FillAtLeast( size_t size );
    //@}

private:
    io::Reader_ABC& src_;
    std::vector< char > buffer_;
    size_t skip_;
    size_t size_;
    bool eof_;
};
}

#endif // STREAM_BUFFER_H
