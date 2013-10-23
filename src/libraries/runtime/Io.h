// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef IO_H
#define IO_H

#include <stddef.h>
#include <boost/noncopyable.hpp>

namespace boost
{
    template< typename T > class shared_ptr;
}

namespace io
{
// =============================================================================
/** @class  Writer_ABC
    @brief  Writer_ABC interface
*/
// Created: BAX 2012-09-13
// =============================================================================
struct Writer_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Writer_ABC() {}
    virtual ~Writer_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual int Write( const void* data, size_t size ) = 0;
    //@}
};

// =============================================================================
/** @class  Reader_ABC
    @brief  Reader_ABC interface
*/
// Created: BAX 2012-09-13
// =============================================================================
struct Reader_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Reader_ABC() {}
    virtual ~Reader_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual int Read( void* data, size_t size ) = 0;
    //@}
};

// =============================================================================
/** @class  Pipe_ABC
    @brief  Pipe_ABC interface
*/
// Created: BAX 2012-09-14
// =============================================================================
struct Pipe_ABC : public Writer_ABC, public Reader_ABC
{
    //! @name Constructors/Destructor
    //@{
             Pipe_ABC() {}
    virtual ~Pipe_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual int  Write( const void* data, size_t size ) = 0;
    virtual int  Read ( void* data, size_t size ) = 0;
    virtual void Close() = 0;
    //@}
};

boost::shared_ptr< Pipe_ABC > MakePipe();
}

#endif // IO_H
