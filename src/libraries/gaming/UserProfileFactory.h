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

#include "clients_kernel/ProfileFactory_ABC.h"

namespace kernel
{
    class Controller;
}

class Publisher_ABC;

// =============================================================================
/** @class  UserProfileFactory
    @brief  UserProfileFactory
*/
// Created: SBO 2007-01-19
// =============================================================================
class UserProfileFactory : public kernel::ProfileFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UserProfileFactory( kernel::Controller& controller, Publisher_ABC& publisher );
    virtual ~UserProfileFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::UserProfile_ABC* Create( xml::xistream& xis ) const;
    virtual kernel::UserProfile_ABC* Create( const QString& name ) const;
    virtual kernel::UserProfile_ABC* Create( const sword::ProfileCreation& message ) const;
    virtual kernel::UserProfile_ABC* Create( kernel::UserProfile_ABC& profile ) const;
    virtual kernel::UserProfile_ABC* Create() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Publisher_ABC& publisher_;
    //@}
};

#endif // __UserProfileFactory_h_
