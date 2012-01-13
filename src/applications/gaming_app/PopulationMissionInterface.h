// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationMissionInterface_h_
#define __PopulationMissionInterface_h_

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
/** @class  PopulationMissionInterface
    @brief  Population interface builder
*/
// Created: SBO 2006-11-22
// =============================================================================
class PopulationMissionInterface : public actions::gui::MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationMissionInterface( QWidget* parent, kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::ActionController& controller,
                                         actions::gui::InterfaceBuilder_ABC& builder, actions::ActionsModel& model );
    virtual ~PopulationMissionInterface();
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

#endif // __PopulationMissionInterface_h_
