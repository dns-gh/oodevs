// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef TOOLS_IPC_HANDLER_ABC_H
#define TOOLS_IPC_HANDLER_ABC_H

#include <boost/noncopyable.hpp>

namespace tools
{
namespace ipc
{
// =============================================================================
/** @class  Handler_ABC interface
    @brief  Handler_ABC
*/
// =============================================================================
class Handler_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Handler_ABC() {}
    virtual ~Handler_ABC() {}
    //@}

    //! @name Public methods
    //@{
    virtual void Stop() = 0;
    //@}
};
}
}

#endif // TOOLS_IPC_HANDLER_ABC_H
