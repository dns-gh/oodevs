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

class QPopupMenu;
class MOS_Agent;
class MOS_MissionInterface_ABC;
class MOS_RC;
class MOS_ShapeEditorMapEventFilter;
class MOS_Team;

// =============================================================================
/** @class  MOS_MissionPanel
    @brief  MOS_MissionPanel
    @par    Using example
    @code
    MOS_MissionPanel;
    @endcode
*/
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
    void FillFragmentaryOrderPopup( QPopupMenu& popupMenu, MOS_RC& rc );

    void hideEvent( QHideEvent* pEvent );

private slots:
    void ToggleAutomate();
    void ActivateUnitMission( int nMissionId );
    void ActivateAutomataMission( int nMissionId );
    void ActivateFragmentaryOrder( int nOrderId );

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

private:
    MOS_Team*   pPopupTeam_;
    MOS_Agent*  pPopupAgent_;

    MOS_MissionInterface_ABC* pMissionInterface_;

    MOS_ShapeEditorMapEventFilter* pLineEditor_;
};

#   include "MOS_MissionPanel.inl"

#endif // __MOS_MissionPanel_h_
