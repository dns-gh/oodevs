// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Profile_ABC_h_
#define __Profile_ABC_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class ClientToSim;
    class ClientToAuthentication;
    class ChatTarget;
    class ClientToReplay;
    class ProfileUpdateRequest;
}

namespace dispatcher
{
// =============================================================================
/** @class  Profile_ABC
    @brief  Profile definition
*/
// Created: AGE 2007-08-24
// =============================================================================
class Profile_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Profile_ABC() {}
    virtual ~Profile_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool CheckRights( const sword::ClientToSim& msg ) const = 0;
    virtual bool CheckRights( const sword::ClientToAuthentication& msg ) const = 0;
    virtual bool CheckRights( const sword::ClientToReplay& msg ) const = 0;
    virtual bool CheckRights( const sword::ChatTarget& source, const sword::ChatTarget& target ) const = 0;
    virtual bool CheckRights( const sword::ProfileUpdateRequest& msg, bool currentTimeControl ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetName() const = 0;
    //@}
};

}

#endif // __Profile_ABC_h_
