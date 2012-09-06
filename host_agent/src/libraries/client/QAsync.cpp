// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "QAsync.h"

#include <algorithm>
#include <boost/bind.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: QAsync::Register
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void QAsync::Register( T_Task task )
{
    QMutexLocker lock( &access_ );
    futures_.erase( std::remove_if( futures_.begin(), futures_.end(), boost::bind( &T_Task::isFinished, _1 ) ), futures_.end() );
    futures_.push_back( task );
}

// -----------------------------------------------------------------------------
// Name: QAsync::Join
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void QAsync::Join()
{
    while( true )
    {
        T_Futures next;
        QMutexLocker lock( &access_ );
        if( futures_.empty() )
            return;
        next.swap( futures_ );
        lock.unlock();
        lock.unlock();
        std::for_each( next.begin(), next.end(), boost::bind( &T_Task::waitForFinished, _1 ) );
    }
}