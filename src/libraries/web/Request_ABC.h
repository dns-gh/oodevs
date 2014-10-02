// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************
#ifndef REQUEST_ABC_H
#define REQUEST_ABC_H

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <string>

namespace boost
{
    template< typename T > class optional;
}

namespace io
{
    struct Reader_ABC;
}

namespace web
{
    typedef boost::property_tree::ptree Tree;
}

namespace web
{
// =============================================================================
/** @class  Request_ABC
    @brief  Request definition
*/
// Created: BAX 2012-02-28
// =============================================================================
struct Request_ABC : public boost::noncopyable
{
    //! @name Constructor/Destructor
             Request_ABC() {}
    virtual ~Request_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string GetUri() const = 0;
    virtual boost::optional< std::string > GetParameter( const std::string& name ) const = 0;
    virtual boost::optional< std::string > GetHeader( const std::string& name ) const = 0;
    virtual std::string GetSid() const = 0;
    //@}

    //! @name Mime methods
    //@{
    typedef std::function< void( io::Reader_ABC& ) > MimeHandler;
    virtual void RegisterMime( const std::string& name, const MimeHandler& handler ) = 0;
    virtual void ParseBodyAsMime() = 0;
    virtual Tree ParseBodyAsJson() = 0;
    //@}
};
}

#endif // REQUEST_ABC_H
