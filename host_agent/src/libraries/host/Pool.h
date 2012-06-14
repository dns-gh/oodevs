// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef POOL_H
#define POOL_H

#include "Pool_ABC.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace host
{
// =============================================================================
/** @class  Pool
    @brief  Thread pool class definition
*/
// Created: BAX 2012-04-16
// =============================================================================
class Pool : public Pool_ABC
{
public:
    //! @name Constructors/Destructors
    //@{
             Pool( size_t numThreads );
    virtual ~Pool();
    //@}

    //! @name Methods
    //@{
    virtual Future Post( const Task& task );
    virtual Future Go( const Task& task );
    virtual void Stop();
    //@}

private:
    //! @name Private members
    //@{
    boost::thread_group threads_;
    boost::asio::io_service service_;
    boost::asio::io_service::work work_;
    //@}
};
}

#endif // POOL_H
