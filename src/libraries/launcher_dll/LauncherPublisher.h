// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LauncherPublisher_h_
#define __LauncherPublisher_h_

#include <boost/noncopyable.hpp>
#include <string>

namespace MsgsLauncherToAdmin { class MsgLauncherToAdmin; }

namespace tools
{
    class MessageSender_ABC;
}

namespace launcher
{

// =============================================================================
/** @class  LauncherPublisher
    @brief  LauncherPublisher
*/
// Created: SBO 2010-09-29
// =============================================================================
class LauncherPublisher : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             LauncherPublisher( tools::MessageSender_ABC& sender, const std::string& endpoint );
    virtual ~LauncherPublisher();
    //@}

    //! @name Operations
    //@{
    void Send( const MsgsLauncherToAdmin::MsgLauncherToAdmin& message );
    //@}

private:
    //! @name Member data
    //@{
    tools::MessageSender_ABC& sender_;
    const std::string endpoint_;
    //@}
};

}

#endif // __LauncherPublisher_h_
