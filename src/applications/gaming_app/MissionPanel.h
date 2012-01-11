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

#include <boost/noncopyable.hpp>
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "tools/Observer_ABC.h"
#include "tools/Iterator.h"
#include "clients_kernel/SafePointer.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Population_ABC;
    class Entity_ABC;
    class Controllers;
    class CoordinateConverter_ABC;
    class GlTools_ABC;
    class Viewport_ABC;
    class Mission;
    class FragOrder;
    class Profile_ABC;
    class AgentKnowledgeConverter_ABC;
    class ObjectKnowledgeConverter_ABC;
    class Time_ABC;
}

namespace gui
{
    class ParametersLayer;
}

namespace actions
{
    class ActionsModel;
    namespace gui
    {
        class InterfaceBuilder_ABC;
        class MissionInterface_ABC;
    }
}

class kernel::ContextMenu;
class Decisions_ABC;
class Decisions;
class AutomatDecisions;
class PopulationDecisions;
class StaticModel;
class Publisher_ABC;
class MissionInterfaceFactory;
class MissionInterfaceBuilder;
class CommandPublisher;

// =============================================================================
// Created: APE 2004-03-19
// =============================================================================
class MissionPanel : public QDockWidget
                   , public tools::Observer_ABC
                   , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
                   , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                   , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    MissionPanel( QWidget* pParent, kernel::Controllers& controllers, const ::StaticModel& model, Publisher_ABC& publisher,
                  const kernel::GlTools_ABC& tools, const kernel::Profile_ABC& profile, actions::ActionsModel& actionsModel,
                  const kernel::Time_ABC& simulation, actions::gui::InterfaceBuilder_ABC& interfaceBuilder );
    virtual ~MissionPanel();
    //@}

    //! @name Operations
    //@{
    void Draw( kernel::Viewport_ABC& viewport );
    //@}

public slots:
    //! @name slots
    //@{
    void ActivatePlanification();
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
    void Close();
    void closeEvent( QCloseEvent* pEvent );
    virtual void OnCancel();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );

    QAction* AddMissions( tools::Iterator< const kernel::Mission& > it, kernel::ContextMenu& menu, const QString& name, const char* slot, int current );
    void AddMissions( const Decisions_ABC& decisions, kernel::ContextMenu& menu, const QString& name, const char* slot, const QPixmap& pixmap = QPixmap() );
    QAction* AddFragOrders( const Decisions_ABC& decisions, kernel::ContextMenu& menu, const QString& name, const char* slot );
    void SetInterface( actions::gui::MissionInterface_ABC* missionInterface );
    void NotifyMission();

    template< typename E, typename T >
    void AddMissionGroup( kernel::ContextMenu& menu, const QString& prefix, const T& list, const char* slot, int current );
    //@}

private:
    //! @name Member Data
    //@{
    kernel::Controllers& controllers_;
    const StaticModel& static_;
    actions::ActionsModel& actionsModel_;
    std::auto_ptr< Publisher_ABC > publisher_;
    const kernel::GlTools_ABC& tools_;
    const kernel::Profile_ABC& profile_;
    std::auto_ptr< CommandPublisher > commandPublisher_;
    const kernel::Time_ABC& simulation_;
    actions::gui::MissionInterface_ABC* pMissionInterface_;
    actions::gui::InterfaceBuilder_ABC& interfaceBuilder_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    bool isPlanifMode_;
    //@}
};

#endif // __MissionPanel_h_
