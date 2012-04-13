// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PROXY_ABC_H
#define PROXY_ABC_H

#include <boost/noncopyable.hpp>
#include <string>

namespace host
{

// =============================================================================
/** @class  Proxy_ABC
    @brief  Proxy_ABC interface
*/
// Created: BAX 2012-04-11
// =============================================================================
class Proxy_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Proxy_ABC() {}
    virtual ~Proxy_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual void Register( const std::string& prefix, const std::string& host, int port ) const = 0;
    virtual void Unregister( const std::string& prefix ) const = 0;
    //@}
};

}

#endif // PROXY_ABC_H
