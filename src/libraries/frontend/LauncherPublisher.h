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

namespace MsgsAdminToLauncher { class MsgAdminToLauncher; }
namespace MsgsLauncherToAdmin { class MsgLauncherToAdmin; }

namespace tools
{
    class MessageSender_ABC;
}

namespace frontend
{

// =============================================================================
/** @class  LauncherPublisher
    @brief  LauncherPublisher
*/
// Created: SBO 2010-09-29
// =============================================================================
class LauncherPublisher
{

public:
    //! @name Constructors/Destructor
    //@{
             LauncherPublisher( tools::MessageSender_ABC& sender, const std::string& endpoint );
    virtual ~LauncherPublisher();
    //@}

    //! @name Operations
    //@{
    void Send( const MsgsAdminToLauncher::MsgAdminToLauncher& message );
    void Send( const MsgsLauncherToAdmin::MsgLauncherToAdmin& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LauncherPublisher( const LauncherPublisher& );            //!< Copy constructor
    LauncherPublisher& operator=( const LauncherPublisher& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
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
