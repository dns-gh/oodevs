// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileFactory_ABC_h_
#define __UserProfileFactory_ABC_h_

#include "game_asn/Asn.h"

class UserProfile;

// =============================================================================
/** @class  UserProfileFactory_ABC
    @brief  UserProfileFactory_ABC
*/
// Created: SBO 2007-01-19
// =============================================================================
class UserProfileFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileFactory_ABC() {}
    virtual ~UserProfileFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual UserProfile* Create( const ASN1T_MsgProfileCreation& message ) const = 0;
    virtual void Create() = 0;
    //@}
};

#endif // __UserProfileFactory_ABC_h_
