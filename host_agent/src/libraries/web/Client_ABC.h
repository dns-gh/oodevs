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

namespace web
{
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
    //@}

    //! @name Methods
    //@{
    virtual void Get( const std::string& host, int port, const std::string& path, const T_Parameters& parameters ) = 0;
    //@}
};

}

#endif // CLIENT_ABC_H
