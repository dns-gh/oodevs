// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ProfilesModel_ABC_h_
#define __ProfilesModel_ABC_h_

namespace kernel
{
class ProfileEditor;
class UserProfile_ABC;

// =============================================================================
/** @class  ProfilesModel_ABC
    @brief  ProfilesModel_ABC declaration
*/
// Created: JSR 2014-06-03
// =============================================================================
class ProfilesModel_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ProfilesModel_ABC() {}
    virtual ~ProfilesModel_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void CommitFromEditor( kernel::ProfileEditor& editor ) = 0;
    virtual kernel::ProfileEditor* CreateProfileEditor() const = 0;
    virtual kernel::ProfileEditor* CreateProfileEditor( kernel::UserProfile_ABC& profile ) const = 0;
    virtual void Apply( boost::function< void( kernel::UserProfile_ABC& ) > functor ) = 0;
    //@}
};

}

#endif // __ProfilesModel_ABC_h_
