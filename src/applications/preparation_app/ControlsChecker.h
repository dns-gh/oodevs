// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ControlsChecker_h_
#define __ControlsChecker_h_

#include "ControlsChecker_ABC.h"

namespace kernel
{
    class Controllers;
}

class Model;

// =============================================================================
/** @class  ControlsChecker
    @brief  Controls checker
*/
// Created: LGY 2011-09-15
// =============================================================================
class ControlsChecker : public ControlsChecker_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ControlsChecker( kernel::Controllers& controllers, Model& model );
    virtual ~ControlsChecker();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const T_ProfileEditors& editors );
    virtual void Update( const UserProfile& profile, const kernel::Entity_ABC& entity );
    virtual QString GetProfileControl( const UserProfile& profile, const kernel::Entity_ABC& entity ) const;
    virtual bool Exists( const QString& oldLogin, const QString& newLogin ) const;
    virtual bool Exists( const QString& login ) const;
    virtual QString Validate();
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateProfile( UserProfile& profile, const kernel::Entity_ABC& entity, bool control );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    T_ProfileEditors editors_;
    //@}
};

#endif // __ControlsChecker_h_
