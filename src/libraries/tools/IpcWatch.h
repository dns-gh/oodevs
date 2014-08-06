// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef TOOLS_IPC_WATCH_H
#define TOOLS_IPC_WATCH_H

#include "IpcHandler_ABC.h"
#include "IpcQueue.h"
#include <tools/WaitEvent.h>

namespace tools
{
namespace ipc
{
class Watch : public Handler_ABC
{
public:
    explicit Watch( tools::WaitEvent& event )
        : event_( event )
    {
        queue_.reset( new Queue( *this ) );
    }

    virtual ~Watch()
    {
        // NOTHING
    }

    virtual void Stop()
    {
        event_.Signal();
    }

private:
    WaitEvent& event_;
    std::unique_ptr< Queue > queue_;
};
}
}

#endif // TOOLS_IPC_WATCH_H
