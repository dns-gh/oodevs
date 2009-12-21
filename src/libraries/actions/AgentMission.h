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

#include "Mission.h"

namespace actions
{

// =============================================================================
/** @class  AgentMission
    @brief  AgentMission
*/
// Created: SBO 2007-05-21
// =============================================================================
class AgentMission : public Mission
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentMission( const kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::Controller& controller, bool registered = true );
             AgentMission( xml::xistream& xis, kernel::Controller& controller, const tools::Resolver_ABC< kernel::MissionType >& missions, const kernel::Entity_ABC& entity );
    virtual ~AgentMission();
    //@}

    //! @name Operations
    //@{
    virtual void Publish( Publisher_ABC& publisher ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentMission( const AgentMission& );            //!< Copy constructor
    AgentMission& operator=( const AgentMission& ); //!< Assignment operator
    //@}
};

}

#endif // __ActionAgentMission_h_
