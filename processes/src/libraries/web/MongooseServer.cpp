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

#define COUNT_OF( X ) ( sizeof( X ) / sizeof*( X ) )

typedef std::map< std::string, std::string > T_Values;

// -----------------------------------------------------------------------------
// Name: ParseParameters
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
T_Values ParseParameters( const mg_request_info* request )
{
    if( !request->query_string )
        return T_Values();

    T_Values args;
    std::vector< std::string > tokens;
    boost::split( tokens, request->query_string, boost::is_any_of("&"), boost::token_compress_on );
    BOOST_FOREACH( const std::string& token, tokens )
    {
        std::vector< std::string > tuple;
        boost::split( tuple, token, boost::is_any_of("="), boost::token_compress_on );
        if( tuple.size() == 2 )
            args.insert( std::make_pair( tuple[0], tuple[1] ) );
    }
    return args;
}

// -----------------------------------------------------------------------------
// Name: ParseHeaders
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
T_Values ParseHeaders( const mg_request_info* request )
{
    T_Values args;
    int count = std::min< size_t >( request->num_headers, COUNT_OF( request->http_headers ) );
    for( int i = 0; i < count; ++i )
        args.insert( std::make_pair( request->http_headers[i].name, request->http_headers[i].value ) );
    return args;
}

// -----------------------------------------------------------------------------
// Name: FindInto
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
boost::optional< std::string > FindInto( const T_Values& data, const std::string& name )
{
    T_Values::const_iterator it = data.find( name );
    if( it == data.end() )
        return boost::optional< std::string >();
    return it->second;
}

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
    MongooseRequest( const mg_request_info* request )
        : method_    ( request->request_method )
        , uri_       ( request->uri )
        , parameters_( ParseParameters( request ) )
        , headers_   ( ParseHeaders( request ) )
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
        return FindInto( parameters_, name );
    }

    // -----------------------------------------------------------------------------
    // Name: MongooseRequest::GetHeader
    // Created: BAX 2012-03-08
    // -----------------------------------------------------------------------------
    boost::optional< std::string > GetHeader( const std::string& name ) const
    {
        return FindInto( headers_, name );
    }

private:
    //! @name Members
    //@{
    const std::string method_;
    const std::string uri_;
    const T_Values parameters_;
    const T_Values headers_;
    //@}
};

}

// -----------------------------------------------------------------------------
// Name: MongooseServer::Notify
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
void MongooseServer::Notify( mg_connection* link, const mg_request_info* mg_request )
{
    MongooseRequest request( mg_request );
    const std::string reply = observer_.Notify( request );
    mg_write( link, reply.data(), reply.size() );
}
