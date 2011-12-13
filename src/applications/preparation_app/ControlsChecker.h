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
#include "preparation/ProfilesModel.h"

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
    virtual std::set< std::string > Find( const kernel::Entity_ABC& entity ) const;
    virtual bool Exists( const QString& oldLogin, const QString& newLogin ) const;
    virtual bool Exists( const QString& login ) const;
    virtual bool IsControlled( const kernel::Entity_ABC& entity ) const;
    virtual void Clean();
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateProfile( UserProfile& profile, const kernel::Entity_ABC& entity, bool control );
    void Find( const kernel::Entity_ABC& entity, const ProfilesModel::T_Units& units, std::set< std::string >& results,
               const std::set< std::string >& editors ) const;
    bool IsWriteable( const kernel::Entity_ABC& entity, const UserProfile& profile ) const;
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
