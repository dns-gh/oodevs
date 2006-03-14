// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-19 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/MissionPanel.h $
// $Author: Age $
// $Modtime: 6/04/05 17:33 $
// $Revision: 13 $
// $Workfile: MissionPanel.h $
//
// *****************************************************************************

#ifndef __MissionPanel_h_
#define __MissionPanel_h_

#include "ContextMenuObserver_ABC.h"
#include "Observer_ABC.h"
#include "Iterator.h"

class QPopupMenu;
class ActionController;
class Decisions;
class AutomatDecisions;
class Agent;
class Population;
class MissionInterface_ABC;
class Mission;

// =============================================================================
// Created: APE 2004-03-19
// =============================================================================
class MissionPanel : public QDockWindow
                   , private Observer_ABC
                   , public ContextMenuObserver_ABC< Agent >
                   , public ContextMenuObserver_ABC< Population >
        
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MissionPanel( QWidget* pParent, ActionController& actions );
    virtual ~MissionPanel();
    //@}

    //! @name Operations
    //@{
    void hideEvent( QHideEvent* pEvent );
    //@}

private slots:
    //! @name Slots
    //@{
    void ActivateAgentMission( int );
    void ActivateAutomatMission( int );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    MissionPanel( const MissionPanel& );
    MissionPanel& operator=( const MissionPanel& );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const Agent& agent, QPopupMenu& menu );
    virtual void NotifyContextMenu( const Population& agent, QPopupMenu& menu );
    int AddMissions( Iterator< const Mission& > it, QPopupMenu& menu, const QString& name, const char* slot );
    void AddAgentMissions( const Decisions& decisions, QPopupMenu& menu ); 
    void AddAutomatMissions( const AutomatDecisions& decisions, QPopupMenu& menu ); 
    //@}

private:
    //! @name Member Data
    //@{
    MissionInterface_ABC* pMissionInterface_;
    const Agent* selected_;
    //@}
};

#endif // __MissionPanel_h_
