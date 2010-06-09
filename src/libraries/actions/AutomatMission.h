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

#include "Mission.h"

namespace actions
{

// =============================================================================
/** @class  AutomatMission
    @brief  AutomatMission
*/
// Created: SBO 2007-05-21
// =============================================================================
class AutomatMission : public Mission
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatMission( const kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::Controller& controller, bool registered = true );
             AutomatMission( xml::xistream& xis, kernel::Controller& controller, const tools::Resolver_ABC< kernel::MissionType >& missions, const kernel::Entity_ABC& entity );
    virtual ~AutomatMission();
    //@}

    //! @name Operations
    //@{
    virtual void Publish( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatMission( const AutomatMission& );            //!< Copy constructor
    AutomatMission& operator=( const AutomatMission& ); //!< Assignment operator
    //@}
};

}

#endif // __ActionAutomatMission_h_
