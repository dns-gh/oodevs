// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionMission_h_
#define __ActionMission_h_

#include "Action_ABC.h"

namespace kernel
{
    class MissionType;
    class Controller;
    class Agent_ABC;
    class Automat_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ActionMission
    @brief  ActionMission
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionMission : public Action_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionMission( const kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::Controller& controller, bool registered = true );
             ActionMission( xml::xistream& xis, kernel::Controller& controller, const kernel::Resolver_ABC< kernel::MissionType >& missions
                          , const kernel::Resolver_ABC< kernel::Agent_ABC >& agents, const kernel::Resolver_ABC< kernel::Automat_ABC >& automats );
    virtual ~ActionMission();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionMission( const ActionMission& );            //!< Copy constructor
    ActionMission& operator=( const ActionMission& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    bool registered_;
    //@}
};

#endif // __ActionMission_h_
