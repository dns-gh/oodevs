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

#include <boost/noncopyable.hpp>

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
    virtual size_t Write( const void* data, size_t size ) = 0;
    //@}
};
}

#endif // IO_H
