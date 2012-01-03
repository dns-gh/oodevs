// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProfileEditor_h_
#define __ProfileEditor_h_

#include "preparation/UserProfile.h"

// =============================================================================
/** @class  ProfileEditor
    @brief  Profile editor
*/
// Created: SBO 2007-11-07
// =============================================================================
class ProfileEditor : public UserProfile
{
public:
    //! @name Constructors/Destructor
    //@{
             ProfileEditor( const UserProfile& profile, kernel::Controller& controller );
    virtual ~ProfileEditor();
    //@}

    //! @name Setters
    //@{
    virtual void SetLogin( const QString& value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ProfileEditor( const ProfileEditor& );            //!< Copy constructor
    ProfileEditor& operator=( const ProfileEditor& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const UserProfile& profile_;
    //@}
};

#endif // __ProfileEditor_h_
