// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileFactory_h_
#define __UserProfileFactory_h_

#include "UserProfileFactory_ABC.h"

namespace kernel
{
    class Controllers;
}

class Model;
class Publisher_ABC;

// =============================================================================
/** @class  UserProfileFactory
    @brief  UserProfileFactory
*/
// Created: SBO 2007-01-19
// =============================================================================
class UserProfileFactory : public UserProfileFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileFactory( const Model& model, kernel::Controllers& controllers, Publisher_ABC& publisher );
    virtual ~UserProfileFactory();
    //@}

    //! @name Operations
    //@{
    virtual UserProfile* Create( const ASN1T_MsgProfileCreation& message ) const;
    virtual void Create();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UserProfileFactory( const UserProfileFactory& );            //!< Copy constructor
    UserProfileFactory& operator=( const UserProfileFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    QString GenerateUniqueLogin() const;
    //@}

private:
    //! @name Member data
    //@{
    const Model& model_;
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    //@}
};

#endif // __UserProfileFactory_h_
