// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MONGOOSE_SERVER_H
#define MONGOOSE_SERVER_H

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

struct mg_context;
struct mg_request_info;
struct mg_connection;

namespace web
{
    class Observer_ABC;

// =============================================================================
/** @class  WebServer_ABC
    @brief  Web server definition
*/
// Created: BAX 2012-02-28
// =============================================================================
class MongooseServer : public boost::noncopyable
{
public:
    //! @name Destructor
             MongooseServer( Observer_ABC& observer, int port );
    virtual ~MongooseServer();
    //@}

    //! @name Operations
    //@{
    void Notify( mg_connection* link, const mg_request_info* request );
    //@}

private:
    //! @name Member data
    //@{
    Observer_ABC& observer_;
    boost::shared_ptr< mg_context > context_;
    //@}
};

}

#endif // MONGOOSE_SERVER_H
