// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __UserProfileEditor_h_
#define __UserProfileEditor_h_

#include <boost/noncopyable.hpp>
#include "gaming/UserProfile.h"

// =============================================================================
/** @class  UserProfileEditor
    @brief  UserProfileEditor
*/
// Created: NPT 2013-05-24
// =============================================================================
class UserProfileEditor : public UserProfile
                        , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileEditor( const UserProfile& profile, kernel::Controller& controller );
    virtual ~UserProfileEditor();
    //@}

    //! @name Operations
    //@{
    //@}

    //! @name Setters
    //@{
    virtual void SetLogin( const QString& value );
    virtual void SetSupervisor( bool supervisor );
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const UserProfile& profile_;
    //@}
};

#endif // __UserProfileEditor_h_
