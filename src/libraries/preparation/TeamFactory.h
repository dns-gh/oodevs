// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TeamFactory_h_
#define __TeamFactory_h_

#include "TeamFactory_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace tools
{
    class IdManager;
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
             TeamFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel, tools::IdManager& idManager );
    virtual ~TeamFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Team_ABC* CreateTeam();
    virtual kernel::Team_ABC* CreateTeam( xml::xistream& xis );
    virtual kernel::Team_ABC* CreateNoSideTeam();
    virtual void ConfigureNoSideTeam( kernel::Team_ABC& team, xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& staticModel_;
    tools::IdManager& idManager_;
    //@}
};

#endif // __TeamFactory_h_
