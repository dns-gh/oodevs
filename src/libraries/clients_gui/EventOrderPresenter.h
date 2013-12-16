// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EventOrderPresenter_h_
#define __EventOrderPresenter_h_

#include "EventSubPresenter_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "ENT/ENT_Enums_Gen.h"
#include <boost/scoped_ptr.hpp>
#pragma warning( push )
#pragma warning( disable : 4251 )
#include <QtCore/qobject.h>
#pragma warning( pop )

namespace kernel
{
    class AgentTypes;
    class Controllers;
    class Decisions_ABC;
    class Entity_ABC;
    class OrderType;
    class TimelinePublisher;
}

namespace actions
{
    namespace gui
    {
        class MissionInterface_ABC;
        class InterfaceBuilder_ABC;
    }
    class ActionsModel;
    class ActionFactory_ABC;
    class Action_ABC;
}

namespace timeline
{
    struct Event;
}

namespace gui
{
    class EventOrderView_ABC;
    struct EventOrderViewState;

// =============================================================================
/** @class  EventOrderPresenter
    @brief  Event manager
*/
// Created: LGY 2013-10-03
// =============================================================================
class EventOrderPresenter : public QObject
                          , public EventSubPresenter_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventOrderPresenter( EventOrderView_ABC& view,
                                  const kernel::AgentTypes& agentTypes,
                                  actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                                  actions::gui::MissionInterface_ABC& missionInterface,
                                  actions::ActionsModel& actionsModel,
                                  actions::ActionFactory_ABC& actionFactory,
                                  kernel::TimelinePublisher& timelinePublisher,
                                  kernel::Controllers& controllers );
    virtual ~EventOrderPresenter();
    //@}

    //! @name Operations
    //@{
    int GetLastContext() const;
    void FillFromAction( const actions::Action_ABC& action,
                         E_MissionType type,
                         const kernel::Entity_ABC* entity,
                         const kernel::Decisions_ABC* decisions );
    void OnTargetChanged( const kernel::Entity_ABC* entity, const kernel::Decisions_ABC* decisions );
    void OnMissionTypeChanged( E_MissionType missionType );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnMissionChanged( const QString& mission );
    //@}

private:
    //! @name EventSubPresenter_ABC implementation
    //@{
    virtual bool ShouldEnableTrigger() const;
    virtual bool ShouldEnableClear() const;

    virtual void Trigger();
    virtual void CommitTo( timeline::Event& event ) const;
    virtual void Purge();
    virtual void Clear();
    //@}

private:
    //! @name Operations helpers
    //@{
    void SetTarget( const kernel::Entity_ABC* entity,
                    const kernel::Decisions_ABC* decisions );
    void Select( E_MissionType type = eMissionType_Pawn,
                 std::string mission = "",
                 const actions::Action_ABC* action = 0 );
    void SelectWithoutTarget( E_MissionType type,
                              const std::string& mission,
                              const actions::Action_ABC* action );
    void SelectWithTarget( const kernel::Decisions_ABC& decisions,
                           E_MissionType type,
                           E_MissionType entityType,
                           const std::string& mission,
                           const actions::Action_ABC* action );
    //@}

private:
    //! @name Member data
    //@{
    EventOrderView_ABC& view_;
    const kernel::AgentTypes& agentTypes_;
    actions::gui::InterfaceBuilder_ABC& interfaceBuilder_;
    actions::gui::MissionInterface_ABC& missionInterface_;
    actions::ActionsModel& actionsModel_;
    actions::ActionFactory_ABC& actionFactory_;
    kernel::TimelinePublisher& timelinePublisher_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    const kernel::Decisions_ABC* decisions_;
    const kernel::OrderType* order_;

    boost::scoped_ptr< EventOrderViewState > state_;
    int context_;
    //@}
};

} //! namespace gui

#endif // __EventOrderPresenter_h_
