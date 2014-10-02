// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef POOL_ABC_H
#define POOL_ABC_H

#include <boost/noncopyable.hpp>
#include <functional>

namespace boost
{
    template< typename T > class shared_future;
}

namespace runtime
{
// =============================================================================
/** @class  Pool_ABC
    @brief  Pool_ABC interface
*/
// Created: BAX 2012-04-16
// =============================================================================
struct Pool_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Pool_ABC() {}
    virtual ~Pool_ABC() {}
    //@}

    //! @name Typedef helper
    //@{
    typedef std::function< void() > Task;
    typedef boost::shared_future< void > Future;
    //@}

    //! @name Methods
    //@{
    virtual Future Post( const Task& task ) = 0;
    virtual void Stop() = 0;
    //@}
};
}

#endif // POOL_ABC_H
