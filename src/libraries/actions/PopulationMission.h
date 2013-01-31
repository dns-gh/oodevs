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

#include "Mission.h"
#include <boost/noncopyable.hpp>

namespace actions
{

// =============================================================================
/** @class  PopulationMission
    @brief  Action Population Mission
*/
// Created: AGE 2007-07-11
// =============================================================================
class PopulationMission : public Mission
                        , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationMission( const kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::Controller& controller, bool registered = true );
             PopulationMission( xml::xistream& xis, kernel::Controller& controller, const tools::Resolver_ABC< kernel::MissionType >& missions, const kernel::Entity_ABC& entity, bool stub );
    virtual ~PopulationMission();
    //@}

    //! @name Operations
    //@{
    virtual void Publish( Publisher_ABC& publisher, int context ) const;
    //@}
};

}

#endif // __ActionPopulationMission_h_
