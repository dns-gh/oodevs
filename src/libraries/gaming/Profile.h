// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Profile_h_
#define __Profile_h_

#include "ASN_Types.h"

namespace kernel
{
    class Controller;
}

class Publisher_ABC;

// =============================================================================
/** @class  Profile
    @brief  Profile
*/
// Created: AGE 2006-10-11
// =============================================================================
class Profile
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Profile( kernel::Controller& controller );
    virtual ~Profile();
    //@}

    //! @name Operations
    //@{
    void Login( Publisher_ABC& publisher ) const;
    void Login( Publisher_ABC& publisher, const std::string& login, const std::string& password ) const;
    void Update( const ASN1T_MsgAuthLoginAck& asnMsg );
    //@}

    //! @name Accessors
    //@{
    bool IsLoggedIn() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Profile( const Profile& );            //!< Copy constructor
    Profile& operator=( const Profile& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    mutable std::string login_;
    mutable std::string password_;
    bool loggedIn_;
    //@}
};

#endif // __Profile_h_
