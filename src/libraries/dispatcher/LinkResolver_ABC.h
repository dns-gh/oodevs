// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LinkResolver_ABC_h_
#define __LinkResolver_ABC_h_

#include <string>

namespace dispatcher
{
    class Profile_ABC;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  LinkResolver_ABC
    @brief  Link resolver declaration
*/
// Created: AGE 2007-08-24
// =============================================================================
class LinkResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LinkResolver_ABC() {}
    virtual ~LinkResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual Profile_ABC&         GetProfile  ( const std::string& link ) = 0;
    virtual ClientPublisher_ABC& GetPublisher( const std::string& link ) = 0;
    virtual unsigned int         GetClientID ( const std::string& link ) const = 0;
    //@}
};

}

#endif // __LinkResolver_ABC_h_
