// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EventManager_h_
#define __EventManager_h_

#include "ENT/ENT_Tr.h"

namespace kernel
{
    class AgentTypes;
    class Entity_ABC;
}

namespace actions
{
    namespace gui
    {
        class MissionInterface_ABC;
        class InterfaceBuilder_ABC;
    }
    class ActionsModel;
    class Action_ABC;
}

namespace timeline
{
    struct Event;
}

class Decisions_ABC;

namespace gui
{
    class EventBuilder_ABC;
// =============================================================================
/** @class  EventOrderPresenter
    @brief  Event manager
*/
// Created: LGY 2013-10-03
// =============================================================================
class EventOrderPresenter : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             EventOrderPresenter( EventBuilder_ABC& builder, const kernel::AgentTypes& agentTypes,
                                  actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                                  actions::gui::MissionInterface_ABC& missionInterface );
            ~EventOrderPresenter();
    //@}

    //! @name Operations
    //@{
    void Select();
    void Select( E_MissionType type );
    void Select( E_MissionType type, const std::string& mission );
    void Select( E_MissionType type, const std::string& mission, const actions::Action_ABC* action );
    void Select( const Decisions_ABC& decisions );
    void Select( const Decisions_ABC& decisions, E_MissionType type );
    void Select( const Decisions_ABC& decisions, E_MissionType type, const std::string& mission );
    void Select( const Decisions_ABC& decisions, E_MissionType type, const std::string& mission,
                 const actions::Action_ABC* action );

    void Publish( actions::ActionsModel& model, timeline::Event* event, bool planned, int context ) const;
    void SetPlanningMode( bool value );
    //@}

private:
    //! @name Operations
    //@{
    void Select( const Decisions_ABC& decisions, const kernel::Entity_ABC& entity, E_MissionType type,
                 E_MissionType entityType, const std::string& mission, const actions::Action_ABC* action );
    void Build( const Decisions_ABC& decisions, E_MissionType type, const std::string& mission );
    //@}

private:
    //! @name Member data
    //@{
    EventBuilder_ABC& builder_;
    const kernel::AgentTypes& agentTypes_;
    E_MissionType currentMissionType_;
    std::string currentMission_;
    actions::gui::InterfaceBuilder_ABC& interfaceBuilder_;
    bool planningMode_;
    actions::gui::MissionInterface_ABC& missionInterface_;
    //@}
};

}

#endif // __EventManager_h_
