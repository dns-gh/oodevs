// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifdef _MSC_VER
#   define _SCL_SECURE_NO_WARNINGS
#endif

#include "MongooseServer.h"
#include "Observer_ABC.h"
#include "Request_ABC.h"
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <mongoose/mongoose.h>

using namespace web;

namespace
{
    // -----------------------------------------------------------------------------
    // Name: web_callback
    // Created: BAX 2012-02-28
    // -----------------------------------------------------------------------------
    void* web_callback( mg_event event, mg_connection* link, const mg_request_info* request )
    {
        if( event != MG_NEW_REQUEST )
            return 0;
        MongooseServer* ptr = reinterpret_cast< MongooseServer* >( request->user_data );
        ptr->Notify( link, request );
        return link;
    }
}

// -----------------------------------------------------------------------------
// Name: MongooseServer::MongooseServer
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
MongooseServer::MongooseServer( Observer_ABC& observer )
    : observer_( observer )
{
    const char* options[] = {
        "enable_directory_listing", "false",
        NULL
    };
    mg_context* next = mg_start( web_callback, this, options );
    if( !next )
        throw std::runtime_error( "unable to start mongoose server" );

    context_.reset( next, mg_stop );
}

// -----------------------------------------------------------------------------
// Name: MongooseServer::~MongooseServer
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
MongooseServer::~MongooseServer()
{
    // NOTHING
}

namespace
{
// =============================================================================
/** @class  MongooseRequest
    @brief  MongooseRequest definition
*/
// Created: BAX 2012-03-08
// =============================================================================
class MongooseRequest : public Request_ABC
{
    public:
    // -----------------------------------------------------------------------------
    // Name: MongooseRequest::MongooseRequest
    // Created: BAX 2012-03-08
    // -----------------------------------------------------------------------------
    MongooseRequest( const mg_connection* link, const mg_request_info* request )
        : link_   ( link )
        , method_ ( request->request_method )
        , uri_    ( request->uri )
        , query_  ( request->query_string ? request->query_string : "" )
    {
        // NOTHING
    }

    // -----------------------------------------------------------------------------
    // Name: MongooseRequest::~MongooseRequest
    // Created: BAX 2012-03-08
    // -----------------------------------------------------------------------------
    ~MongooseRequest()
    {
        // NOTHING
    }

    // -----------------------------------------------------------------------------
    // Name: MongooseRequest::GetMethod
    // Created: BAX 2012-03-08
    // -----------------------------------------------------------------------------
    const std::string& GetMethod() const
    {
        return method_;
    }

    // -----------------------------------------------------------------------------
    // Name: MongooseRequest::GetUri
    // Created: BAX 2012-03-08
    // -----------------------------------------------------------------------------
    const std::string& GetUri() const
    {
        return uri_;
    }

    // -----------------------------------------------------------------------------
    // Name: MongooseRequest::GetParameter
    // Created: BAX 2012-03-08
    // -----------------------------------------------------------------------------
    boost::optional< std::string > GetParameter( const std::string& name ) const
    {
        std::vector< char > buffer( query_.size() );
        int len = mg_get_var( query_.data(), query_.size(), name.c_str(), &buffer[0], buffer.size() );
        if( len == -1 )
            return boost::none;
        return std::string( &buffer[0], len );
    }

    // -----------------------------------------------------------------------------
    // Name: MongooseRequest::GetHeader
    // Created: BAX 2012-03-08
    // -----------------------------------------------------------------------------
    boost::optional< std::string > GetHeader( const std::string& name ) const
    {
        const char* reply = mg_get_header( link_, name.data() );
        if( !reply )
            return boost::none;
        return reply;
    }

private:
    //! @name Members
    //@{
    const mg_connection* link_;
    const std::string method_;
    const std::string uri_;
    const std::string query_;
    //@}
};

}

// -----------------------------------------------------------------------------
// Name: MongooseServer::Notify
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
void MongooseServer::Notify( mg_connection* link, const mg_request_info* mg_request )
{
    MongooseRequest request( link, mg_request );
    const std::string reply = observer_.Notify( request );
    mg_write( link, reply.data(), reply.size() );
}
