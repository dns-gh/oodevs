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
#include <boost/function.hpp>
#include <boost/thread/future.hpp>

namespace host
{
// =============================================================================
/** @class  Pool_ABC
    @brief  Pool_ABC interface
*/
// Created: BAX 2012-04-16
// =============================================================================
class Pool_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Pool_ABC() {}
    virtual ~Pool_ABC() {}
    //@}

    //! @name Typedef helper
    //@{
    typedef boost::function< void() > Task;
    typedef boost::shared_future< void > Future;
    //@}

    //! @name Methods
    //@{
    virtual Future Post( const Task& ) = 0;
    virtual void Stop() = 0;
    //@}
};

}

#endif // POOL_H
