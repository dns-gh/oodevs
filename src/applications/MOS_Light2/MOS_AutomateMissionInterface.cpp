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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AutomateMissionInterface.cpp $
// $Author: Age $
// $Modtime: 31/03/05 16:49 $
// $Revision: 11 $
// $Workfile: MOS_AutomateMissionInterface.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_AutomateMissionInterface.h"
#include "moc_MOS_AutomateMissionInterface.cpp"

#include "MOS_MissionPanel.h"
#include "MOS_ASN_Messages.h"
#include "MOS_ASN_Types.h"
#include "MOS_ParamPoint.h"
#include "MOS_ParamAgent.h"
#include "MOS_ParamAgentList.h"
#include "MOS_ParamPath.h"
#include "MOS_ParamLimits.h"
#include "MOS_ParamLimaList.h"
#include "MOS_ParamRadioBtnGroup.h"
#include "MOS_ParamGDH.h"
#include "MOS_ParamComboBox.h"
#include "MOS_ParamBool.h"
#include "MOS_ParamDirection.h"
#include "MOS_Agent.h"

// -----------------------------------------------------------------------------
// Name: MOS_AutomateMissionInterface constructor
/** @param  agent 
    @param  mission 
    @param  parentPanel 
*/
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
MOS_AutomateMissionInterface::MOS_AutomateMissionInterface( MOS_Agent& agent, uint nMissionId, MOS_MissionPanel& parentPanel )
    : MOS_MissionInterface_ABC( agent, parentPanel )
    , nMissionId_             ( nMissionId )
{
    pASNMsgOrder_ = new MOS_ASN_MsgAutomateOrder();
    pASNMsgOrder_->GetAsnMsg().order_id = (uint)(&agent_);
    pASNMsgOrder_->GetAsnMsg().oid_unite_executante = agent_.GetAgentID();

    QLabel* pLabel = new QLabel( ENT_Tr::ConvertFromAutomataMission( E_AutomataMission( nMissionId_ ) ).c_str(), this );
    pLabel->setFrameStyle( QFrame::Box | QFrame::Sunken );
    pLabel->setAlignment( Qt::AlignCenter );
    QFont font = pLabel->font();
    font.setBold( true );
    pLabel->setFont( font );

    this->CreateInterface();
}


// -----------------------------------------------------------------------------
// Name: MOS_AutomateMissionInterface::CreateDefaultParameters
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
void MOS_AutomateMissionInterface::CreateDefaultParameters()
{

    ASN1T_MsgAutomateOrder& order = pASNMsgOrder_->GetAsnMsg();

    // Limits
    paramVector_.push_back( new MOS_ParamLimits( order.oid_limite_gauche,
                                                 order.oid_limite_droite,
                                                 "Limites" ,
                                                 "Fixer limite 1",
                                                 "Fixer limite 2",
                                                 this ) );

    // Limas
    paramVector_.push_back( new MOS_ParamLimaList( order.oid_limas,
                                                   "Limas",
                                                   "Ajouter aux limas",
                                                   this ) );

    // Formation
    MOS_ParamRadioBtnGroup<ASN1T_EnumAutomateOrderFormation>* pParam 
        = new MOS_ParamRadioBtnGroup<ASN1T_EnumAutomateOrderFormation>( order.formation,
                                                                 "Formation",
                                                                 2,
                                                                 Qt::Horizontal,
                                                                 this );
    pParam->AddButton( "Un échelon", EnumAutomateOrderFormation::un_echelon );
    pParam->AddButton( "Deux échelons", EnumAutomateOrderFormation::deux_echelons, true );
    paramVector_.push_back( pParam );

    // Dangerous direction
    paramVector_.push_back( new MOS_ParamDirection( order.direction_dangereuse, "Direction dangeureuse", this ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_AutomateMissionInterface::OnOk
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
void MOS_AutomateMissionInterface::OnOk()
{
    if( ! this->CheckValidity() )
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
// Name: MOS_AutomateMissionInterface::CreateMission_Test_MoveTo
// Created: APE 2004-05-27
// -----------------------------------------------------------------------------
//void MOS_AutomateMissionInterface::CreateMission_Test_MoveTo()
//{
//    ASN1T_Mission_Automate_Test_MoveTo& asnMission = *new ASN1T_Mission_Automate_Test_MoveTo();
//    pASNMsgOrder_->GetAsnMsg().mission.t                              = T_Mission_Automate_mission_automate_test_move_to;
//    pASNMsgOrder_->GetAsnMsg().mission.u.mission_automate_test_move_to = &asnMission;
//
//    //$$$$ ?
//}


#include "MOS_AutomateMissionInterface_Gen.cpp"