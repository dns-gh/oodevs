// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionPanel_h_
#define __MissionPanel_h_

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
        class MissionInterface_ABC;
    }
}

class QPopupMenu;
class Decisions_ABC;
class Decisions;
class AutomatDecisions;
class PopulationDecisions;
class StaticModel;
class Publisher_ABC;
class MissionInterfaceFactory;
class MissionInterfaceBuilder;
class Simulation;
class CommandPublisher;

// =============================================================================
// Created: APE 2004-03-19
// =============================================================================
class MissionPanel : public QDockWindow
                   , public tools::Observer_ABC
                   , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
                   , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    MissionPanel( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& model, Publisher_ABC& publisher, gui::ParametersLayer& layer, const kernel::GlTools_ABC& tools, const kernel::Profile_ABC& profile, actions::ActionsModel& actionsModel
                         , kernel::AgentKnowledgeConverter_ABC& knowledgeConverter, kernel::ObjectKnowledgeConverter_ABC& objectKnowledgeConverter, const Simulation& simulation );
    virtual ~MissionPanel();
    //@}

    //! @name Operations
    //@{
    void Draw( kernel::Viewport_ABC& viewport );
    void hideEvent( QHideEvent* pEvent );
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
    //@}

private:
    //! @name Copy / Assignment
    //@{
    MissionPanel( const MissionPanel& );
    MissionPanel& operator=( const MissionPanel& );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );

    int AddMissions( tools::Iterator< const kernel::Mission& > it, kernel::ContextMenu& menu, const QString& name, const char* slot );
    void AddMissions( const Decisions_ABC& decisions, kernel::ContextMenu& menu, const QString& name, const char* slot, const QPixmap& pixmap = QPixmap() );
    int AddFragOrders( const Decisions_ABC& decisions, kernel::ContextMenu& menu, const QString& name, const char* slot );
    void AddFragOrders( tools::Iterator< const kernel::FragOrder& > it, QPopupMenu& menu, const char* slot );
    void SetInterface( actions::gui::MissionInterface_ABC* missionInterface );
    void NotifyMission();

    template< typename T >
    void AddMissionGroup( QPopupMenu& menu, const QString& prefix, const T& list, const char* slot );
    //@}

private:
    //! @name Member Data
    //@{
    kernel::Controllers& controllers_;
    const StaticModel& static_;
    actions::ActionsModel& actionsModel_;
    std::auto_ptr< Publisher_ABC > publisher_;
    gui::ParametersLayer& layer_;
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::GlTools_ABC& tools_;
    const kernel::Profile_ABC& profile_;
    std::auto_ptr< CommandPublisher > commandPublisher_;

    actions::gui::MissionInterface_ABC* pMissionInterface_;
    MissionInterfaceBuilder* interfaceBuilder_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    //@}
};

#endif // __MissionPanel_h_
