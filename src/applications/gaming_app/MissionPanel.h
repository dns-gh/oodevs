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

class QPopupMenu;
class Decisions;
class AutomatDecisions;
class Agent_ABC;
class Population_ABC;
class MissionInterface_ABC;
class Mission;
class Controllers;
class ParametersLayer;
class CoordinateConverter_ABC;
class StaticModel;
class GlTools_ABC;
class AgentKnowledgeConverter_ABC;
class Publisher_ABC;

// =============================================================================
// Created: APE 2004-03-19
// =============================================================================
class MissionPanel : public QDockWindow
                   , public Observer_ABC
                   , public ContextMenuObserver_ABC< Agent_ABC >
                   , public ContextMenuObserver_ABC< Population_ABC >
        
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MissionPanel( QWidget* pParent, Controllers& controllers, const StaticModel& model, Publisher_ABC& publisher, ParametersLayer& layer, const GlTools_ABC& tools );
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
    virtual void NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu );
    virtual void NotifyContextMenu( const Population_ABC& agent, ContextMenu& menu );
    int AddMissions( Iterator< const Mission& > it, ContextMenu& menu, const QString& name, const char* slot );
    void AddAgentMissions( const Decisions& decisions, ContextMenu& menu ); 
    void AddAutomatMissions( const AutomatDecisions& decisions, ContextMenu& menu ); 
    template< typename D >
    int AddFragOrders( const D& decisions, ContextMenu& menu, const QString& name, const char* slot );
    //@}

private:
    //! @name Member Data
    //@{
    Controllers& controllers_;
    const StaticModel& static_;
    Publisher_ABC& publisher_;
    ParametersLayer& layer_;
    const CoordinateConverter_ABC& converter_;
    const GlTools_ABC& tools_;

    AgentKnowledgeConverter_ABC* knowledgeConverter_;
    MissionInterface_ABC* pMissionInterface_;
    const Agent_ABC* selected_;
    const Population_ABC* selectedPopulation_;
    //@}
};

#endif // __MissionPanel_h_
