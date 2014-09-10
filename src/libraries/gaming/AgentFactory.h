// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentFactory_h_
#define __AgentFactory_h_

#include "AgentFactory_ABC.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Profile_ABC;
    class Workers;
}

class Model;
class Publisher_ABC;
class StaticModel;

// =============================================================================
/** @class  AgentFactory
    @brief  Agent factory
*/
// Created: AGE 2006-02-13
// =============================================================================
class AgentFactory : public AgentFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentFactory( kernel::Controllers& controllers,
                           Model& model,
                           const StaticModel& staticModel,
                           Publisher_ABC& publisher,
                           kernel::Workers& workers,
                           const kernel::Profile_ABC& profile,
                           actions::ActionsModel& actionsModel );
    virtual ~AgentFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Automat_ABC* Create( const sword::AutomatCreation& message );
    virtual kernel::Agent_ABC* Create( const sword::UnitCreation& message );
    virtual kernel::Population_ABC* Create( const sword::CrowdCreation& message );
    virtual kernel::Inhabitant_ABC* Create( const sword::PopulationCreation& message );
    //@}

private:
    //! @name Helpers
    //@{
    void AttachExtensions( kernel::Entity_ABC& agent );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    Publisher_ABC& publisher_;
    kernel::Workers& workers_;
    const kernel::Profile_ABC& profile_;
    actions::ActionsModel& actionsModel_;
    //@}
};

#endif // __AgentFactory_h_
