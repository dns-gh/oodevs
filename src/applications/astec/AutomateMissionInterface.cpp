// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-06 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AutomateMissionInterface.cpp $
// $Author: Age $
// $Modtime: 31/03/05 16:49 $
// $Revision: 11 $
// $Workfile: AutomateMissionInterface.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "AutomateMissionInterface.h"
#include "moc_AutomateMissionInterface.cpp"

#include "MissionPanel.h"
#include "ASN_Messages.h"
#include "ASN_Types.h"
#include "ParamPoint.h"
#include "ParamAgent.h"
#include "ParamAgentList.h"
#include "ParamPath.h"
#include "ParamLimits.h"
#include "ParamLimaList.h"
#include "ParamRadioBtnGroup.h"
#include "ParamGDH.h"
#include "ParamComboBox.h"
#include "ParamBool.h"
#include "ParamDirection.h"
#include "Agent.h"

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface constructor
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
AutomateMissionInterface::AutomateMissionInterface( Agent& agent, uint nMissionId, MissionPanel& parentPanel )
    : MissionInterface_ABC( agent, parentPanel )
    , nMissionId_             ( nMissionId )
    , pParamLimits_           ( 0 )
{
    pASNMsgOrder_ = new ASN_MsgAutomateOrder();
    pASNMsgOrder_->GetAsnMsg().order_id = (uint)(&agent_);
    pASNMsgOrder_->GetAsnMsg().oid_unite_executante = agent_.GetID();

    QLabel* pLabel = new QLabel( ENT_Tr::ConvertFromAutomataMission( E_AutomataMission( nMissionId_ ) ).c_str(), this );
    pLabel->setFrameStyle( QFrame::Box | QFrame::Sunken );
    pLabel->setAlignment( Qt::AlignCenter );
    QFont font = pLabel->font();
    font.setBold( true );
    pLabel->setFont( font );

    this->CreateInterface();
}


// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateDefaultParameters
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateDefaultParameters()
{

    ASN1T_MsgAutomateOrder& order = pASNMsgOrder_->GetAsnMsg();

    // Limits
    // saved so we can check validity later...
    pParamLimits_ = new ParamLimits( order.oid_limite_gauche,
                                        order.oid_limite_droite,
                                        "Limites" ,
                                        "Fixer limite 1",
                                        "Fixer limite 2",
                                        this );
    paramVector_.push_back( pParamLimits_ );

    // Limas
    paramVector_.push_back( new ParamLimaList( order.oid_limas,
                                                   "Limas",
                                                   "Ajouter aux limas",
                                                   this ) );

    // Formation
    ParamRadioBtnGroup<ASN1T_EnumAutomateOrderFormation>* pParam 
        = new ParamRadioBtnGroup<ASN1T_EnumAutomateOrderFormation>( order.formation,
                                                                 "Formation",
                                                                 2,
                                                                 Qt::Horizontal,
                                                                 this );
    pParam->AddButton( "Un échelon", EnumAutomateOrderFormation::un_echelon );
    pParam->AddButton( "Deux échelons", EnumAutomateOrderFormation::deux_echelons, true );
    paramVector_.push_back( pParam );

    // Dangerous direction
    paramVector_.push_back( new ParamDirection( order.direction_dangereuse, "Direction dangeureuse", this, false ) );
}


// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::OnOk
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
void AutomateMissionInterface::OnOk()
{
    if( ! this->CheckValidity() || ! pParamLimits_->CheckValidityWhenRequired() )
        return;

    std::stringstream strMsg;
    for( IT_ParamVector it = paramVector_.begin() ; it != paramVector_.end() ; ++it )
    {
        if( it != paramVector_.begin() )
            strMsg << "\n";
        (*it)->WriteMsg( strMsg );
    }

    std::stringstream strMsgTitle;
    strMsgTitle << "Mission automate " << ENT_Tr::ConvertFromAutomataMission( E_AutomataMission( nMissionId_ ) ) << " pour agent " << agent_.GetName();
    MT_LOG_INFO( strMsgTitle.str().c_str(), eSent, strMsg.str().c_str() );

    pASNMsgOrder_->Send( 45 );

    parentPanel_.hide();
}


// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateMission_Test_MoveTo
// Created: APE 2004-05-27
// -----------------------------------------------------------------------------
//void AutomateMissionInterface::CreateMission_Test_MoveTo()
//{
//    ASN1T_Mission_Automate_Test_MoveTo& asnMission = *new ASN1T_Mission_Automate_Test_MoveTo();
//    pASNMsgOrder_->GetAsnMsg().mission.t                              = T_Mission_Automate_mission_automate_test_move_to;
//    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_test_move_to = &asnMission;
//
//    //$$$$ ?
//}


#include "AutomateMissionInterface_Gen.cpp"