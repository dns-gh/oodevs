// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef ASYNC_H
#define ASYNC_H

#include "Pool_ABC.h"

#include <boost/thread/mutex.hpp>
#include <vector>

namespace runtime
{
// =============================================================================
/** @class  Async
    @brief  Async class definition
*/
// Created: BAX 2012-05-29
// =============================================================================
struct Async : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
    explicit Async( Pool_ABC& pool );
            ~Async();
    //@}

    //! @name Typedef helpers
    //@{
    typedef std::vector< Pool_ABC::Future > T_Futures;
    //@}

    //! @name Public Methods
    //@{
    void Post( const Pool_ABC::Task& task );
    void Join();
    Pool_ABC& GetPool() const;
    //@}

private:
    //! @name Private members
    //@{
    Pool_ABC& pool_;
    boost::mutex access_;
    T_Futures futures_;
    //@}
};

template< typename T >
void Post( Async& async, const T& functor )
{
    async.Post( [=]{ functor(); } );
}
}

#endif // ASYNC_H
