// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef TOOLS_IPC_QUEUE_H
#define TOOLS_IPC_QUEUE_H

#include <memory>
#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/noncopyable.hpp>

namespace tools
{
    class ThreadPool;
    class WaitEvent;
namespace ipc
{
    class Handler_ABC;
}
}

namespace tools
{
namespace ipc
{
// do not modify current values or suffer ABI breakage
enum Command
{
    IPC_COMMAND_STOP  = 1,
};

// =============================================================================
/** @class  Queue class
    @brief  Queue
*/
// =============================================================================
class Queue : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Queue( Handler_ABC& handler );
    virtual ~Queue();
    //@}

    //! @name Public methods
    //@{
    static bool Send( Command cmd, int pid );
    //@}

private:
    //! @name Private methods
    //@{
    void Run();
    void Update();
    void Process( Command cmd );
    //@}

private:
    //! @name Member data
    //@{
    Handler_ABC& handler_;
    std::auto_ptr< tools::ThreadPool > pool_;
    std::auto_ptr< tools::WaitEvent > quit_;
    std::auto_ptr< boost::interprocess::message_queue > queue_;
    //@}
};
}
}

#endif // TOOLS_IPC_QUEUE_H
