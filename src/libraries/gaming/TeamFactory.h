// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamFactory_h_
#define __TeamFactory_h_

#include "TeamFactory_ABC.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

class Model;
class StaticModel;

// =============================================================================
/** @class  TeamFactory
    @brief  TeamFactory
*/
// Created: AGE 2006-02-15
// =============================================================================
class TeamFactory : public TeamFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TeamFactory( kernel::Controllers& controllers,
                          Model& model,
                          const StaticModel& staticModel,
                          const kernel::Profile_ABC& profile,
                          actions::ActionsModel& actionsModel );
    virtual ~TeamFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Team_ABC* CreateTeam( const sword::PartyCreation& message );
    virtual kernel::Team_ABC* CreateNoSideTeam();
    virtual kernel::Formation_ABC* CreateFormation( const sword::FormationCreation& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    const kernel::Profile_ABC& profile_;
    actions::ActionsModel& actionsModel_;
    //@}
};

#endif // __TeamFactory_h_
