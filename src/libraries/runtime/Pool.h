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

namespace runtime
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
             Pool( uint32_t cache, uint32_t max );
    virtual ~Pool();
    //@}

    //! @name Methods
    //@{
    virtual Future Post( const Task& task );
    virtual void Stop();
    virtual void Acquire();
    //@}

    //! @name Typedef helpers
    //@{
    typedef boost::shared_ptr< boost::thread > T_Thread;
    typedef std::vector< T_Thread > T_Threads;
    //@}

private:
    //! @name Private members
    //@{
    void AddThread();
    void RunThread();
    //@}

private:
    //! @name Private members
    //@{
    const uint32_t cache_; ///< max number of cached threads
    const uint32_t max_;   ///< max number of threads
    uint32_t load_;        ///< number of current tasks
    uint32_t idle_;        ///< number of idle threads
    boost::mutex access_;
    T_Threads threads_;
    boost::asio::io_service service_;
    boost::asio::io_service::work work_;
    //@}
};
}

#endif // POOL_H
