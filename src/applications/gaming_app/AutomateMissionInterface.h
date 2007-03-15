// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomateMissionInterface_h_
#define __AutomateMissionInterface_h_

#include "MissionInterface_ABC.h"

namespace kernel
{
    class Mission;
}

class Publisher_ABC;
class MissionInterfaceFactory;
class MissionInterfaceBuilder;

// =============================================================================
/** @class  MissionInterfaceBuilder
    @brief  Mission interface builder
*/
// Created: SBO 2006-11-22
// =============================================================================
class AutomateMissionInterface : public MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomateMissionInterface( QWidget* parent, kernel::Entity_ABC& entity, const kernel::Mission& mission, kernel::ActionController& controller
                                     , Publisher_ABC& publisher, MissionInterfaceFactory& factory, MissionInterfaceBuilder& builder );
    virtual ~AutomateMissionInterface();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomateMissionInterface( const AutomateMissionInterface& );
    AutomateMissionInterface& operator=( const AutomateMissionInterface& );
    //@}

    //! @name Helpers
    //@{
    virtual void Publish();
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC&         publisher_;
    const kernel::Mission& mission_;
    //@}
};

#endif // __AutomateMissionInterface_h_
