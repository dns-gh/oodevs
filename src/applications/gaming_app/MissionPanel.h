// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionPanel_h_
#define __MissionPanel_h_

#include "clients_gui/RichDockWidget.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/Iterator.h"

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Population_ABC;
    class Entity_ABC;
    class Controllers;
    class Mission;
    class Profile_ABC;
    class Time_ABC;
}

namespace gui
{
    class GlTools_ABC;
    class RichLabel;
    class RichCheckBox;
    class RichDateTimeEdit;
    class RichPushButton;
    class Viewport_ABC;
}

namespace actions
{
    class Action_ABC;
    class ActionsModel;
    namespace gui
    {
        class InterfaceBuilder_ABC;
        class MissionInterface;
    }
}

namespace timeline
{
    struct Event;
}

namespace tools
{
    class ExerciseConfig;
}

class kernel::ContextMenu;
class Decisions_ABC;
class StaticModel;
class Publisher_ABC;
class CommandPublisher;
class TimelinePublisher;

// =============================================================================
// Created: APE 2004-03-19
// =============================================================================
class MissionPanel : public gui::RichDockWidget
                   , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
                   , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    MissionPanel( QWidget* pParent, kernel::Controllers& controllers, const ::StaticModel& model, Publisher_ABC& publisher,
                  gui::GlTools_ABC& tools, const kernel::Profile_ABC& profile, actions::ActionsModel& actionsModel,
                  const kernel::Time_ABC& simulation, actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                  const tools::ExerciseConfig& config, TimelinePublisher& timelinePublisher );
    virtual ~MissionPanel();
    //@}

    //! @name Operations
    //@{
    void Draw( gui::Viewport_ABC& viewport );
    //@}

private slots:
    //! @name Slots
    //@{
    void ActivateAgentMission( int );
    void ActivateAutomatMission( int );
    void ActivateFragOrder( int );
    void ActivatePopulationMission( int );
    void Engage();
    void Disengage();
    void Validate();
    void Close();
    virtual void closeEvent( QCloseEvent* pEvent );
    void OnPlanningChecked( int state );
    void OnPlannedMission( const actions::Action_ABC& action );
    //@}

signals:
    //! @name Signals
    //@{
    void CreateEvent( const timeline::Event& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    virtual void NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode );

    void SetVisible( bool visible );
    void FillInterface( int id ) const;
    QAction* AddMissions( tools::Iterator< const kernel::Mission& > it, kernel::ContextMenu& menu, const QString& name, const char* slot, int current );
    void AddMissions( const Decisions_ABC& decisions, kernel::ContextMenu& menu, const QString& name, const char* slot, const QPixmap& pixmap = QPixmap() );
    QAction* AddFragOrders( const Decisions_ABC& decisions, kernel::ContextMenu& menu, const QString& name, const char* slot );
    void NotifyMission();

    template< typename E, typename T >
    void AddMissionGroup( kernel::ContextMenu& menu, const QString& prefix, const T& list, const char* slot, int current );

    template< typename T >
    void FillInterface( int id );
    //@}

private:
    //! @name Member Data
    //@{
    gui::RichLabel* titleLabel_;
    gui::RichLabel* pixmapLabel_;
    gui::RichCheckBox* planningCheckBox_;
    gui::RichDateTimeEdit* planningDateTimeEdit_;
    gui::RichPushButton* okButton_;
    gui::RichPushButton* cancelButton_;
    actions::gui::MissionInterface* pMissionInterface_;
    E_MissionType currentType_;

    const StaticModel& static_;
    actions::ActionsModel& actionsModel_;
    std::auto_ptr< Publisher_ABC > publisher_;
    gui::GlTools_ABC& tools_;
    const kernel::Profile_ABC& profile_;
    std::auto_ptr< CommandPublisher > commandPublisher_;
    const kernel::Time_ABC& simulation_;
    
    actions::gui::InterfaceBuilder_ABC& interfaceBuilder_;
    const tools::ExerciseConfig& config_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    TimelinePublisher& timelinePublisher_;
    //@}
};

#endif // __MissionPanel_h_
