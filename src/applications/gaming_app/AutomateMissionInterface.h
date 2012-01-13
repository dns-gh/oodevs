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

#include "actions_gui/MissionInterface_ABC.h"

namespace kernel
{
    class MissionType;
}

namespace actions
{
    class ActionsModel;
    namespace gui
    {
        class InterfaceBuilder_ABC;
    }
}

// =============================================================================
/** @class  AutomateMissionInterface
    @brief  Mission interface builder
*/
// Created: SBO 2006-11-22
// =============================================================================
class AutomateMissionInterface : public actions::gui::MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomateMissionInterface( QWidget* parent, kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::ActionController& controller,
                                       actions::gui::InterfaceBuilder_ABC& builder, actions::ActionsModel& model );
    virtual ~AutomateMissionInterface();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Publish();
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel&     model_;
    const kernel::MissionType& mission_;
    //@}
};

#endif // __AutomateMissionInterface_h_
