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
#include <iostream>
#include <string>

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
    explicit StreamBuffer( std::istream& src );
    virtual ~StreamBuffer();
    //@}

    //! @name Methods
    //@{
    bool        Eof() const;
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
    void FillAtLeast( size_t size );
    //@}

private:
    std::istream& src_;
    char buffer_[4096];
    size_t skip_;
    size_t size_;
};
}

#endif // STREAM_BUFFER_H
