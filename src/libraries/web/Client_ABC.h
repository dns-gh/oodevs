// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CLIENT_ABC_H
#define CLIENT_ABC_H

#include <boost/noncopyable.hpp>
#include <map>
#include <string>

namespace boost
{
    template< typename T > class optional;
    template< typename T > class shared_ptr;
}

namespace web
{
// =============================================================================
/** @class  Response_ABC
    @brief  Response_ABC interface
*/
// Created: BAX 2012-06-18
// =============================================================================
struct Response_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Response_ABC() {}
    virtual ~Response_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual int GetStatus() const = 0;
    virtual std::string GetBody() const = 0;
    //@}
};

// =============================================================================
/** @class  Client_ABC
    @brief  Client_ABC interface
*/
// Created: BAX 2012-04-11
// =============================================================================
struct Client_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Client_ABC() {}
    virtual ~Client_ABC() {}
    //@}

    //! @name Typedef helpers
    //@{
    typedef std::map< std::string, std::string > T_Parameters;
    typedef boost::shared_ptr< Response_ABC > T_Response;
    //@}

    //! @name Methods
    //@{
    virtual T_Response Get( const std::string& host, int port, const std::string& path, const T_Parameters& parameters ) = 0;
    virtual T_Response Get( const std::string& scheme, const std::string& host, int port, const std::string& path, const T_Parameters& parameters ) = 0;
    //@}
};
}

#endif // CLIENT_ABC_H
