// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DefaultProfile_h_
#define __DefaultProfile_h_

#include "Profile_ABC.h"

namespace dispatcher
{
// =============================================================================
/** @class  DefaultProfile
    @brief  DefaultProfile
*/
// Created: AGE 2007-08-24
// =============================================================================
class DefaultProfile : public Profile_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DefaultProfile();
    virtual ~DefaultProfile();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckRights( const sword::ClientToSim& msg ) const;
    virtual bool CheckRights( const sword::ClientToAuthentication& msg ) const;
    virtual bool CheckRights( const sword::ClientToReplay& msg ) const;
    virtual bool CheckRights( const sword::ChatTarget& source, const sword::ChatTarget& target ) const;
    virtual bool CheckRights( const sword::ProfileUpdateRequest& msg, bool currentTimeControl ) const;
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetName() const;
    //@}
};

}

#endif // __DefaultProfile_h_
