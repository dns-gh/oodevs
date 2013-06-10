// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventOrderWidget_h_
#define __EventOrderWidget_h_

#include "EventWidget_ABC.h"
#include "clients_gui/ValuedComboBox.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace actions
{
    class Action_ABC;

    namespace gui
    {
        class InterfaceBuilder_ABC;
        class MissionInterface;
    }
}

namespace gui
{
    template< typename T > class RichWarnWidget;
    class RichComboBox;
    class RichGroupBox;
    class RichLabel;
}

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class OrderType;
    class Population_ABC;
    class Profile_ABC;
}

namespace tools
{
    class ExerciseConfig;
    template< typename T > class Iterator;
}

class Decisions_ABC;
class EventAction;
class Model;

// =============================================================================
/** @class  EventOrderWidget
    @brief  EventOrderWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventOrderWidget : public EventWidget_ABC
                       , public tools::Observer_ABC
                       , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
                       , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventOrderWidget( kernel::Controllers& controllers, Model& model, const tools::ExerciseConfig& config, actions::gui::InterfaceBuilder_ABC& interfaceBuilder, const kernel::Profile_ABC& profile );
    virtual ~EventOrderWidget();
    //@}

private:
    //! @name EventWidget_ABC implementation
    //@{
    virtual void Purge();
    virtual void Fill( const Event& event );
    virtual void Commit( timeline::Event& event ) const;
    virtual void Trigger() const;
    virtual bool IsValid() const;
    //@}

    //! @name Obersvers implementation
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    //@}

    //! @name Helpers
    //@{
    void AddTargetToMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu, E_MissionType allowedType );
    const Decisions_ABC* GetTargetDecision() const;
    void FillMission();
    void SetTarget( const kernel::Entity_ABC* entity );
    void WarnIfTargetAndMissionAreNotCompatible() const;
    void BuildMissionInterface();
    void FillMissionInterface( const EventAction& event );
    void Publish( timeline::Event* event = 0 ) const;

    template< typename T >
    void AddSingleOrder( const T& mission );

    template< typename T >
    void AddAllOrders();

    template< typename T >
    void AddCompatibleOrders( tools::Iterator< const T& > it );

    void AddCompatibleFragOrders( const Decisions_ABC& decisions );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnMissionTypeChanged( int index );
    void OnMissionChanged( int id );
    void OnTargetSelected();
    void OnPlannedMission( const actions::Action_ABC& action, timeline::Event* event ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    actions::gui::InterfaceBuilder_ABC& interfaceBuilder_;
    const kernel::Profile_ABC& profile_;

    gui::RichWarnWidget< QComboBox >* missionTypeCombo_;
    QVBoxLayout* missionComboLayout_;
    gui::RichWarnWidget< QComboBox >* missionCombo_;
    QStandardItemModel missionModel_;
    gui::RichGroupBox* targetGroupBox_;
    gui::RichLabel* targetLabel_;
    actions::gui::MissionInterface* missionInterface_;

    E_MissionType currentType_;
    bool missionChoosed_;

    kernel::SafePointer< kernel::Entity_ABC > contextMenuEntity_;
    kernel::SafePointer< kernel::Entity_ABC > target_;
    //@}
};

#endif // __EventOrderWidget_h_
