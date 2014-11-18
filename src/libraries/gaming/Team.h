// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Team_h_
#define __Team_h_

#include "clients_gui/Team.h"
#include "clients_kernel/Karma.h"

namespace kernel
{
    class Controllers;
    class OptionVariant;
    class Profile_ABC;
}

namespace sword
{
    class PartyCreation;
    class PartyUpdate;
}

// =============================================================================
/** @class  Team
    @brief  Team
*/
// Created: AGN 2003-12-22
// =============================================================================
class Team : public gui::Team
           , public kernel::Extension_ABC
           , public tools::Observer_ABC
           , public kernel::Updatable_ABC< sword::PartyUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             Team( const sword::PartyCreation& message,
                   kernel::Controllers& controllers,
                   actions::ActionsModel& actionsModel,
                   const kernel::Profile_ABC& profile );
             Team( kernel::Controllers& controllers,
                   unsigned long id,
                   const QString& name,
                   actions::ActionsModel& actionsModel,
                   const kernel::Profile_ABC& profile );
    virtual ~Team();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Karma& GetKarma() const;
    void DoUpdate( const sword::PartyUpdate& message );
    virtual bool CanBeRenamed() const;
    //@}

private:
    //! @name Attributes
    //@{
    kernel::Karma karma_;
    const kernel::Profile_ABC& profile_;
    //@}

};

#endif // __Team_h_
