// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ProfilesChecker_h_
#define __ProfilesChecker_h_

#include "ProfilesChecker_ABC.h"

namespace kernel
{
    class Controllers;
}

class Model;

// =============================================================================
/** @class  ProfilesChecker
    @brief  Profiles checker
*/
// Created: LGY 2011-12-07
// =============================================================================
class ProfilesChecker : public ProfilesChecker_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ProfilesChecker();
    virtual ~ProfilesChecker();
    //@}

    //! @name Operations
    //@{t;
    virtual void Display( const T_ProfileEditors& editors );
    virtual bool Exists( const QString& oldLogin, const QString& newLogin ) const;
    virtual bool Exists( const QString& login ) const;
    //@}

private:
    //! @name Member data
    //@{
    T_ProfileEditors editors_;
    //@}
};

#endif // __ProfilesChecker_h_