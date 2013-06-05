// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __FragmentaryOrderInterface_h_
#define __FragmentaryOrderInterface_h_

#include "actions_gui/MissionInterface_ABC.h"

namespace kernel
{
    class FragOrderType;
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
/** @class  FragmentaryOrderInterface
    @brief  Fragmentary order interface builder
*/
// Created: SBO 2006-11-22
// =============================================================================
class FragmentaryOrderInterface : public actions::gui::MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FragmentaryOrderInterface( QWidget* parent, kernel::Entity_ABC& entity, const kernel::FragOrderType& fragOrder, kernel::Controllers& controllers,
                                        actions::gui::InterfaceBuilder_ABC& builder, actions::ActionsModel& model, const tools::ExerciseConfig& config, const kernel::Time_ABC& simulation );
    virtual ~FragmentaryOrderInterface();
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
    const kernel::FragOrderType& fragOrder_;
    //@}
};

#endif // __FragmentaryOrderInterface_h_
