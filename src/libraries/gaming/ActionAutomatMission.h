// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionAutomatMission_h_
#define __ActionAutomatMission_h_

#include "ActionMission.h"

// =============================================================================
/** @class  ActionAutomatMission
    @brief  ActionAutomatMission
*/
// Created: SBO 2007-05-21
// =============================================================================
class ActionAutomatMission : public ActionMission
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionAutomatMission( const kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::Controller& controller, bool registered = true );
             ActionAutomatMission( xml::xistream& xis, kernel::Controller& controller, const kernel::Resolver_ABC< kernel::MissionType >& missions, const kernel::Entity_ABC& entity );
    virtual ~ActionAutomatMission();
    //@}

    //! @name Operations
    //@{
    virtual void Publish( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionAutomatMission( const ActionAutomatMission& );            //!< Copy constructor
    ActionAutomatMission& operator=( const ActionAutomatMission& ); //!< Assignment operator
    //@}
};

#endif // __ActionAutomatMission_h_
