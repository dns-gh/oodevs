// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef QASYNC_H_
#define QASYNC_H_

#include <QMutex>
#include <QFuture>
#include <vector>

namespace gui
{
struct QAsync
{
    typedef QFuture< void >         T_Future;
    typedef std::vector< T_Future > T_Futures;

    void Register( T_Future future );
    void Join();

private:
    T_Futures futures_;
    QMutex access_;
};
}

#endif // QASYNC_H_