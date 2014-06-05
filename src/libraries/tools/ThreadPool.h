// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef THREAD_POOL_H__
#define THREAD_POOL_H__

#include <boost/noncopyable.hpp>
#include <memory>

namespace boost
{
    template< typename T > class function;
    template< typename T > class shared_future;
}

namespace tools
{
// =============================================================================
/** @class  Pool
    @brief  Thread pool class definition
*/
// Created: BAX 2012-04-16
// =============================================================================
class ThreadPool : public boost::noncopyable
{
public:
    //! @name Constructors/Destructors
    //@{
    explicit ThreadPool( size_t numThreads );
    virtual ~ThreadPool();
    //@}

    //! @name Typedefs
    //@{
    typedef boost::function< void() >    Task;
    typedef boost::shared_future< void > Future;
    //@}

    //! @name Methods
    //@{
    virtual void   Post( const Task& task );
    virtual Future Promise( const Task& task );
    enum JoinType { SkipPending, ProcessPending };
    virtual void   Join( JoinType type );
    //@}

private:
    //! @name Private members
    //@{
    struct Private;
    std::unique_ptr< Private > private_;
    //@}
};
}

#endif // THREAD_POOL_H__
