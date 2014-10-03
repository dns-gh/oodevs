// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MOCK_POOL_H
#define MOCK_POOL_H

#include "runtime/Pool_ABC.h"

#include <boost/thread/future.hpp>

namespace mocks
{
    MOCK_BASE_CLASS( MockPool, runtime::Pool_ABC )
    {
        MockPool()
        {
            MOCK_EXPECT( Post ).calls( boost::bind( &MockPool::Call, this, _1 ) );
        }
        MOCK_METHOD( Post, 1 );
        MOCK_METHOD( Stop, 0 );
    private:
        typedef boost::promise< void > LazyPromise;
        static void LazyCall( LazyPromise& promise, Task task )
        {
            task();
            promise.set_value();
        }
        Future Call( Task task )
        {
            boost::shared_ptr< LazyPromise > ptr( new LazyPromise() );
            ptr->set_wait_callback( boost::bind( &LazyCall, _1, task ) );
            promises.push_back( ptr );
            return Future( ptr->get_future() );
        }
        std::vector< boost::shared_ptr< LazyPromise > > promises;
    };
};

#endif // MOCK_POOL_H
