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
    class MissionType;
}

namespace actions
{
    class ActionsModel;
}

class Publisher_ABC;
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
             AutomateMissionInterface( QWidget* parent, kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::ActionController& controller
                 , Publisher_ABC& publisher, MissionInterfaceBuilder& builder, actions::ActionsModel& model );
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
    actions::ActionsModel&     model_;
    Publisher_ABC&             publisher_;
    const kernel::MissionType& mission_;
    //@}
};

#endif // __AutomateMissionInterface_h_
