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

namespace sword
{
    class AdminToLauncher;
    class LauncherToAdmin;
}

namespace tools
{
    class MessageSender_ABC;
}

namespace frontend
{
// =============================================================================
/** @class  LauncherPublisher
    @brief  Launcher publisher
*/
// Created: SBO 2010-09-29
// =============================================================================
class LauncherPublisher : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LauncherPublisher( tools::MessageSender_ABC& sender );
    virtual ~LauncherPublisher();
    //@}

    //! @name Accessors
    //@{
    bool IsSame( const std::string& endpoint ) const;
    void SetHost( const std::string& endpoint );
    const std::string& Endpoint() const;
    //@}

    //! @name Operations
    //@{
    void Send( const sword::AdminToLauncher& message );
    //@}

private:
    //! @name Member data
    //@{
    tools::MessageSender_ABC& sender_;
    std::string endpoint_;
    //@}
};

}

#endif // __LauncherPublisher_h_
