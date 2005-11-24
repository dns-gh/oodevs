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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MissionPanel.h $
// $Author: Age $
// $Modtime: 6/04/05 17:33 $
// $Revision: 13 $
// $Workfile: MOS_MissionPanel.h $
//
// *****************************************************************************

#ifndef __MOS_MissionPanel_h_
#define __MOS_MissionPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Param_ABC.h"
#include "MOS_Types.h"

class QPopupMenu;
class MOS_Agent;
class MOS_Population;
class MOS_PopulationConcentration;
class MOS_PopulationFlow;
class MOS_MissionInterface_ABC;
class MOS_RC;
class MOS_ShapeEditorMapEventFilter;
class MOS_Team;

// =============================================================================
// Created: APE 2004-03-19
// =============================================================================
class MOS_MissionPanel : public QDockWindow
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_MissionPanel );
    
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_MissionPanel( QWidget* pParent );
    ~MOS_MissionPanel();
    //@}

public slots:
    //! @name Operations
    //@{
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    //@}

private:
    void FillDiplomacyPopupMenu( QPopupMenu& popumMenu, MOS_Team& team );
    void FillStandardPopupMenu( QPopupMenu& popupMenu, MOS_Agent& agent );
    void FillStandardPopupMenu( QPopupMenu& popupMenu, MOS_Population& population );
    void FillStandardPopupMenu( QPopupMenu& popupMenu, MOS_PopulationConcentration& concentration );
    void FillStandardPopupMenu( QPopupMenu& popupMenu, MOS_PopulationFlow& flow );
    void FillFragmentaryOrderPopup( QPopupMenu& popupMenu, MOS_RC& rc );

    void hideEvent( QHideEvent* pEvent );

private slots:
    void ToggleAutomate();
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
    MOS_Team*		             pPopupTeam_;
    MOS_Agent*		             pPopupAgent_;
	MOS_Population*              pPopupPopulation_;
    MOS_PopulationConcentration* pPopupPopulationConcentration_;
    MOS_PopulationFlow*          pPopupPopulationFlow_;

    MOS_MissionInterface_ABC* pMissionInterface_;

    MOS_ShapeEditorMapEventFilter* pAgentMagicMovePositionEditor_;
    MOS_ShapeEditorMapEventFilter* pPopulationMagicMovePositionEditor_;
};

#endif // __MOS_MissionPanel_h_
