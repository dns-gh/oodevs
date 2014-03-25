// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef dispatcher_AuthenticatedLinkResolver_ABC_h
#define dispatcher_AuthenticatedLinkResolver_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace dispatcher
{
    class Profile_ABC;

// =============================================================================
/** @class  AuthenticatedLinkResolver_ABC
    @brief  Authenticated link resolver declaration
*/
// Created: MCO 2013-08-23
// =============================================================================
class AuthenticatedLinkResolver_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AuthenticatedLinkResolver_ABC() {}
    virtual ~AuthenticatedLinkResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual Profile_ABC& GetProfile( const std::string& link ) const = 0;
    //@}
};

}

#endif // dispatcher_AuthenticatedLinkResolver_ABC_h
