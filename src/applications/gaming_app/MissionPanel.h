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

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/Iterator.h"

namespace kernel
{
    class Agent_ABC;
    class Population_ABC;
    class Controllers;
    class CoordinateConverter_ABC;
    class GlTools_ABC;
    class Mission;
}

namespace gui
{
    class ParametersLayer;
}

class QPopupMenu;

class Decisions;
class AutomatDecisions;
class MissionInterface_ABC;
class StaticModel;
class AgentKnowledgeConverter_ABC;
class Publisher_ABC;

// =============================================================================
// Created: APE 2004-03-19
// =============================================================================
class MissionPanel : public QDockWindow
                   , public kernel::Observer_ABC
                   , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
        
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MissionPanel( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& model, Publisher_ABC& publisher, gui::ParametersLayer& layer, const kernel::GlTools_ABC& tools );
    virtual ~MissionPanel();
    //@}

    //! @name Operations
    //@{
    void Draw( const geometry::Rectangle2f& viewport );
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
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& agent, kernel::ContextMenu& menu );
    int AddMissions( kernel::Iterator< const kernel::Mission& > it, kernel::ContextMenu& menu, const QString& name, const char* slot );
    void AddAgentMissions( const Decisions& decisions, kernel::ContextMenu& menu ); 
    void AddAutomatMissions( const AutomatDecisions& decisions, kernel::ContextMenu& menu ); 
    template< typename D >
    int AddFragOrders( const D& decisions, kernel::ContextMenu& menu, const QString& name, const char* slot );
    //@}

private:
    //! @name Member Data
    //@{
    kernel::Controllers& controllers_;
    const StaticModel& static_;
    Publisher_ABC& publisher_;
    gui::ParametersLayer& layer_;
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::GlTools_ABC& tools_;

    AgentKnowledgeConverter_ABC* knowledgeConverter_;
    MissionInterface_ABC* pMissionInterface_;
    const kernel::Agent_ABC* selected_;
    const kernel::Population_ABC* selectedPopulation_;
    //@}
};

#endif // __MissionPanel_h_
