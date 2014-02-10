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
#include "ENT/ENT_Enums.h"
#include <boost/scoped_ptr.hpp>
#pragma warning( push )
#pragma warning( disable : 4251 )
#include <QtCore/qobject.h>
#pragma warning( pop )

namespace kernel
{
    class AgentTypes;
    class Controllers;
    class Entity_ABC;
    class OrderType;
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
    struct EventOrderViewState;
    template< typename T > class EventView_ABC;
    class TimelinePublisher;

// =============================================================================
/** @class  EventOrderPresenter
    @brief  Event manager
*/
// Created: LGY 2013-10-03
// =============================================================================
class EventOrderPresenter : public QObject
                          , public EventSubPresenter_ABC< EventOrderViewState >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventOrderPresenter( EventView_ABC< EventOrderViewState >& view,
                                  const kernel::AgentTypes& agentTypes,
                                  actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                                  actions::gui::MissionInterface_ABC& missionInterface,
                                  actions::ActionsModel& actionsModel,
                                  actions::ActionFactory_ABC& actionFactory,
                                  TimelinePublisher& timelinePublisher,
                                  kernel::Controllers& controllers );
    virtual ~EventOrderPresenter();
    //@}

    //! @name Operations
    //@{
    virtual void FillFrom( const Event& event );
    int GetLastContext() const;
    void OnTargetChanged( const kernel::Entity_ABC* entity );
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

    virtual void Trigger( const gui::Event& event );
    virtual void CommitTo( timeline::Event& event ) const;
    virtual void Purge();
    virtual void Clear();
    //@}

private:
    //! @name Helpers
    //@{
    void Select( E_MissionType type = eMissionType_Pawn,
                 std::string mission = "",
                 const actions::Action_ABC* action = 0 );
    void SelectWithoutTarget( E_MissionType type,
                              const std::string& mission,
                              const actions::Action_ABC* action );
    void SelectWithTarget( const kernel::Entity_ABC& entity,
                           E_MissionType type,
                           const std::string& mission,
                           const actions::Action_ABC* action );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::AgentTypes& agentTypes_;
    actions::gui::InterfaceBuilder_ABC& interfaceBuilder_;
    actions::gui::MissionInterface_ABC& missionInterface_;
    actions::ActionsModel& actionsModel_;
    actions::ActionFactory_ABC& actionFactory_;
    TimelinePublisher& timelinePublisher_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    const kernel::OrderType* order_;
    //@}
};

} //! namespace gui

#endif // __EventOrderPresenter_h_
