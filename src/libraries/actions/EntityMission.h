// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EntityMission_h_
#define __EntityMission_h_

#include "Mission.h"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/SimulationSenders.h"

namespace actions
{

// =============================================================================
/** @class  EntityMission
    @brief  EntityMission
*/
// Created: ABR 2013-06-14
// =============================================================================
template< typename T >
class EntityMission : public Mission
{

public:
    //! @name Constructors/Destructor
    //@{
             EntityMission( const kernel::MissionType* mission, kernel::Controller& controller, bool registered = true );
    virtual ~EntityMission();
    //@}

    //! @name Operations
    //@{
    virtual void Publish( Publisher_ABC& publisher, int context ) const;
    //@}
};

#include "EntityMission.inl"

typedef EntityMission< simulation::UnitOrder >    AgentMission;
typedef EntityMission< simulation::AutomatOrder > AutomatMission;
typedef EntityMission< simulation::CrowdOrder >   PopulationMission;

} //! namespace actions

#endif // __EntityMission_h_
