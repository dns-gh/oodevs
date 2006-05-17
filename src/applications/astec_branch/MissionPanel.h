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
class Decisions;
class AutomatDecisions;
class Agent;
class Population;
class MissionInterface_ABC;
class Mission;
class Controllers;
class ParametersLayer;
class CoordinateConverter_ABC;
class Model;
class GlTools_ABC;

// =============================================================================
// Created: APE 2004-03-19
// =============================================================================
class MissionPanel : public QDockWindow
                   , public Observer_ABC
                   , public ContextMenuObserver_ABC< Agent >
                   , public ContextMenuObserver_ABC< Population >
        
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MissionPanel( QWidget* pParent, Controllers& controllers, Model& model, ParametersLayer& layer, const GlTools_ABC& tools );
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
    template< typename D >
    int AddFragOrders( const D& decisions, QPopupMenu& menu, const QString& name, const char* slot );
    //@}

private:
    //! @name Member Data
    //@{
    Controllers& controllers_;
    ParametersLayer& layer_;
    const CoordinateConverter_ABC& converter_;
    const GlTools_ABC& tools_;

    MissionInterface_ABC* pMissionInterface_;
    const Agent* selected_;
    const Population* selectedPopulation_;
    //@}
};

#endif // __MissionPanel_h_
