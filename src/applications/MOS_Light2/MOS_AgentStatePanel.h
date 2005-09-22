// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentStatePanel.h $
// $Author: Nld $
// $Modtime: 8/07/05 15:25 $
// $Revision: 14 $
// $Workfile: MOS_AgentStatePanel.h $
//
// *****************************************************************************

#ifndef __MOS_AgentStatePanel_h_
#define __MOS_AgentStatePanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Attr_Def.h"
#include "MOS_InfoPanel_ABC.h"

class MOS_Agent;
class MOS_ReportListView;

class QGroupBox;

// =============================================================================
/** @class  MOS_AgentStatePanel
    @brief  MOS_AgentStatePanel
*/
// Created: APE 2004-03-10
// =============================================================================
class MOS_AgentStatePanel : public MOS_InfoPanel_ABC
{
    MT_COPYNOTALLOWED( MOS_AgentStatePanel );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    explicit MOS_AgentStatePanel( QWidget* pParent );
    virtual ~MOS_AgentStatePanel();
    //@}

private:
    //! @name Operations
    //@{
    virtual void OnUpdate        ();
    virtual void OnClearSelection();
    virtual void OnAgentUpdated  ( MOS_Agent& agent );
    //@}

private:
    //! @name Member data
    //@{
    QLabel*    pNameLabel_;
    QLabel*    pRawOpStateLabel_;
    QLabel*    pDeadLabel_;
    QLabel*    pNeutralizedLabel_;
    QLabel*    pSpeedLabel_;
    QLabel*    pDirectionLabel_;
    QLabel*    pAltitudeLabel_;
    
    QLabel*    pOldStanceLabel_;
    QLabel*    pStanceLabel_;

    QLabel*    pROELabel_;
    QLabel*    pCloseCombatLabel_;
    QLabel*    pFightRateLabel_;
    QLabel*    pOpStateLabel_;

    QGroupBox* pReinforcementsGroupBox_;
    QLabel*    pReinforcementsLabel_;
    QLabel*    pReinforcedLabel_;

    QLabel*    pBoardingStateLabel_;
    QLabel*    pHumanTransportersReadyLabel_;
    QLabel*    pNBCSuitLabel_;
    QLabel*    pNBCAgentsLabel_;
    QLabel*    pContaminationLabel_;

    QLabel*    pCommJammedLabel_;
    QLabel*    pRadioSilenceLabel_;

    QLabel*    pExperienceLabel_;
    QLabel*    pMoraleLabel_;
    QLabel*    pTirednessLabel_;
     
    QGroupBox* pTransportedGroupBox_;
    QLabel*    pTransportedLabel_;

    QLabel*    pPrisonnerLabel_;
    QLabel*    pSurrenderedLabel_;
    QLabel*    pRefugeeLabel_;

    QGroupBox* pLogLinksGroupBox_;
    QLabel*    pTC2_;
    QLabel*    pLogMaintenanceSuperior_;
    QLabel*    pLogMedicalSuperior_;
    QLabel*    pLogSupplySuperior_;
    //@}
};

#endif // __MOS_AgentStatePanel_h_
