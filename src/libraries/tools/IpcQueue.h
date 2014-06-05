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
    IPC_COMMAND_EXIT = -1, ///< private command
    IPC_COMMAND_STOP =  1,
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
    bool Update();
    bool Process( Command cmd );
    static bool Send( boost::interprocess::message_queue& queue, Command cmd );
    //@}

    //! @name Private types
    //@{
    struct Counter;
    //@}

private:
    //! @name Member data
    //@{
    Handler_ABC& handler_;
    std::unique_ptr< tools::ThreadPool > pool_;
    std::unique_ptr< Counter > counter_;
    std::unique_ptr< boost::interprocess::message_queue > queue_;
    //@}
};
}
}

#endif // TOOLS_IPC_QUEUE_H
