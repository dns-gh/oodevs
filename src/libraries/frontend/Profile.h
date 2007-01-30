// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Profile_h_
#define __Profile_h_

#include "Messages.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Controller;
}

namespace frontend
{
    class Publisher_ABC;

// =============================================================================
/** @class  Profile
    @brief  Profile
*/
// Created: SBO 2007-01-29
// =============================================================================
class Profile : public kernel::Updatable_ABC< ASN1T_MsgAuthenticationResponse >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Profile( kernel::Controller& controller );
    virtual ~Profile();
    //@}

    //! @name Operations
    //@{
    bool IsLoggedIn() const;
    void Login( Publisher_ABC& publisher, const QString& login, const QString& password ) const;
    virtual void DoUpdate( const ASN1T_MsgAuthenticationResponse& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Profile( const Profile& );            //!< Copy constructor
    Profile& operator=( const Profile& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    QString login_;
    ASN1T_EnumProfileType type_;
    bool loggedIn_;
    //@}
};

}

#endif // __Profile_h_
