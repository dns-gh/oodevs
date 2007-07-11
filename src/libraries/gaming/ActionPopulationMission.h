// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionPopulationMission_h_
#define __ActionPopulationMission_h_

#include "ActionMission.h"

// =============================================================================
/** @class  ActionPopulationMission
    @brief  Action Population Mission
*/
// Created: AGE 2007-07-11
// =============================================================================
class ActionPopulationMission : public ActionMission
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionPopulationMission( const kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::Controller& controller, bool registered = true );
             ActionPopulationMission( xml::xistream& xis, kernel::Controller& controller, const kernel::Resolver_ABC< kernel::MissionType >& missions, const kernel::Entity_ABC& entity );
    virtual ~ActionPopulationMission();
    //@}

    //! @name Operations
    //@{
    virtual void Publish( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionPopulationMission( const ActionPopulationMission& );            //!< Copy constructor
    ActionPopulationMission& operator=( const ActionPopulationMission& ); //!< Assignment operator
    //@}
};

#endif // __ActionPopulationMission_h_
