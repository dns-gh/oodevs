// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionAgentMission_h_
#define __ActionAgentMission_h_

#include "ActionMission.h"

// =============================================================================
/** @class  ActionAgentMission
    @brief  ActionAgentMission
*/
// Created: SBO 2007-05-21
// =============================================================================
class ActionAgentMission : public ActionMission
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionAgentMission( const kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::Controller& controller, const Simulation& simulation, bool registered = true );
             ActionAgentMission( xml::xistream& xis, kernel::Controller& controller, const kernel::Resolver_ABC< kernel::MissionType >& missions, const kernel::Entity_ABC& entity, const Simulation& simulation );
    virtual ~ActionAgentMission();
    //@}

    //! @name Operations
    //@{
    virtual void Publish( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionAgentMission( const ActionAgentMission& );            //!< Copy constructor
    ActionAgentMission& operator=( const ActionAgentMission& ); //!< Assignment operator
    //@}
};

#endif // __ActionAgentMission_h_
