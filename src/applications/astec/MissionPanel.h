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

#ifdef __GNUG__
#   pragma interface
#endif

#include "Param_ABC.h"
#include "Types.h"

class QPopupMenu;
class Agent;
class Population;
class PopulationConcentration;
class PopulationFlow;
class MissionInterface_ABC;
class RC;
class ShapeEditorMapEventFilter;
class Team;

// =============================================================================
// Created: APE 2004-03-19
// =============================================================================
class MissionPanel : public QDockWindow
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MissionPanel );
    
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MissionPanel( QWidget* pParent );
    ~MissionPanel();
    //@}

public slots:
    //! @name Operations
    //@{
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
    //@}

private:
    void FillDiplomacyPopupMenu( QPopupMenu& popumMenu, Team& team );
    void FillStandardPopupMenu( QPopupMenu& popupMenu, Agent& agent );
    void FillStandardPopupMenu( QPopupMenu& popupMenu, Population& population );
    void FillStandardPopupMenu( QPopupMenu& popupMenu, PopulationConcentration& concentration );
    void FillStandardPopupMenu( QPopupMenu& popupMenu, PopulationFlow& flow );
    void FillFragmentaryOrderPopup( QPopupMenu& popupMenu, RC& rc );

    void hideEvent( QHideEvent* pEvent );

private slots:
    void ToggleAutomate();
    void ToggleAggregation();
    void ActivateUnitMission( int nMissionId );
    void ActivatePopulationMission( int nMissionId );
    void ActivateAutomataMission( int nMissionId );
    void ActivateFragmentaryOrder( int nOrderId );
    void ActivateFragmentaryOrderPopulation( int nOrderId );

    void MagicMove();
    void MagicMoveDone();
    void MagicRestore( int nId );
    void MagicDestroyComponent();
    void MagicDestroyAll();
    void MagicRecompletion();
    void MagicSurrender();
    void MagicRecoverHumanTransporters();
    void MagicChangeDiplomatie();
    void MagicChangeLogisticLinks();
    void MagicPushFlux();
    void MagicChangeQuotas();
    void ChangeHumanFactors();

    //! @name Population magic actions
    //@{
    void PopulationMagicMove                ();
    void PopulationMagicMoveDone            ();
    void PopulationMagicChangeAttitudeGlobal( int nAttitude );
    void PopulationMagicChangeAttitude      ( int nAttitude, bool bGlobal = false );
    void PopulationMagicDestroyAll          ();
    //@}

private:
    Team*		             pPopupTeam_;
    Agent*		             pPopupAgent_;
	Population*              pPopupPopulation_;
    PopulationConcentration* pPopupPopulationConcentration_;
    PopulationFlow*          pPopupPopulationFlow_;

    MissionInterface_ABC* pMissionInterface_;

    ShapeEditorMapEventFilter* pAgentMagicMovePositionEditor_;
    ShapeEditorMapEventFilter* pPopulationMagicMovePositionEditor_;
};

#endif // __MissionPanel_h_
