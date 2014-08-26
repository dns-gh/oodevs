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

namespace runtime
{
    struct Pool_ABC;
    struct Runtime_ABC;
}

namespace web
{
    struct Observer_ABC;

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
             Server( const runtime::Runtime_ABC& runtime,
                     cpplog::BaseLogger& logger,
                     runtime::Pool_ABC& pool,
                     Observer_ABC& observer,
                     int port );
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
    const std::unique_ptr< Private > private_;
    //@}
};
}

#endif // MONGOOSE_SERVER_H
