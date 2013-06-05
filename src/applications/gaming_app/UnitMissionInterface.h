// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UnitMissionInterface_h_
#define __UnitMissionInterface_h_

#include "actions_gui/MissionInterface_ABC.h"

namespace kernel
{
    class MissionType;
    class Time_ABC;
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
/** @class  UnitMissionInterface
    @brief  Unit interface builder
*/
// Created: SBO 2006-11-22
// =============================================================================
class UnitMissionInterface : public actions::gui::MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
            UnitMissionInterface( QWidget* parent, kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::Controllers& controllers,
                                  actions::gui::InterfaceBuilder_ABC& builder, actions::ActionsModel& model, const tools::ExerciseConfig& config, const kernel::Time_ABC& simulation );
    virtual ~UnitMissionInterface();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Publish();
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel& model_;
    const kernel::MissionType& mission_;
    //@}
};

#endif // __UnitMissionInterface_h_
