// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ProfilesChecker_ABC_h_
#define __ProfilesChecker_ABC_h_

namespace kernel
{
    class UserProfile_ABC;
}

// =============================================================================
/** @class  ProfilesChecker_ABC
    @brief  Profiles checker declaration
*/
// Created: LGY 2011-12-07
// =============================================================================
class ProfilesChecker_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ProfilesChecker_ABC() {}
    virtual ~ProfilesChecker_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Exists( const QString& oldLogin, const QString& newLogin ) const = 0;
    virtual bool Exists( const QString& login ) const = 0;
    virtual void NotifyNameChanged( const kernel::UserProfile_ABC* profile ) const = 0;
    //@}
};

#endif // __ProfilesChecker_ABC_h_
