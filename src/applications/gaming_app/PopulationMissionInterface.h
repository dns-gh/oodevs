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
}

class Publisher_ABC;
class MissionInterfaceBuilder;

// =============================================================================
/** @class  MissionInterfaceBuilder
    @brief  Mission interface builder
*/
// Created: SBO 2006-11-22
// =============================================================================
class PopulationMissionInterface : public actions::gui::MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationMissionInterface( QWidget* parent, kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::ActionController& controller
                 , Publisher_ABC& publisher, MissionInterfaceBuilder& builder, actions::ActionsModel& model );
    virtual ~PopulationMissionInterface();
    //@}
   
private:
    //! @name Copy/Assignment
    //@{
    PopulationMissionInterface( const PopulationMissionInterface& );
    PopulationMissionInterface& operator=( const PopulationMissionInterface& );
    //@}

    //! @name Helpers
    //@{
    virtual void Publish();
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel& model_;
    Publisher_ABC& publisher_;
    const kernel::MissionType& mission_;
    //@}
};

#endif // __PopulationMissionInterface_h_
