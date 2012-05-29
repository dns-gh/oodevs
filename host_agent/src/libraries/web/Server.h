// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SERVER_H
#define SERVER_H

#include <boost/noncopyable.hpp>
#include <memory>

namespace cpplog
{
    class BaseLogger;
}

namespace host
{
    class Pool_ABC;
}

namespace web
{
    class Observer_ABC;

// =============================================================================
/** @class  Server
    @brief  Web server definition
*/
// Created: BAX 2012-02-28
// =============================================================================
class Server : public boost::noncopyable
{
public:
    //! @name Destructor
             Server( cpplog::BaseLogger& logger, host::Pool_ABC& pool, Observer_ABC& observer, int port );
    virtual ~Server();
    //@}

    //! @name Public Functions
    //@{
    void Listen();
    //@}

private:
    //! @name Member data
    //@{
    struct Private;
    const std::auto_ptr< Private > private_;
    //@}
};

}

#endif // MONGOOSE_SERVER_H
