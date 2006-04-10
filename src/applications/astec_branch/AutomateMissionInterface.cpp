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

#include "astec_pch.h"
#include "AutomateMissionInterface.h"
#include "ASN_Messages.h"
#include "ASN_Types.h"
#include "Agent.h"

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface constructor
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
AutomateMissionInterface::AutomateMissionInterface( QWidget* parent, Agent& agent, uint nMissionId, ActionController& controller, ParametersLayer& layer, const CoordinateConverter& converter )
    : MissionInterface_ABC( parent, agent, controller, layer, converter )
    , agent_( agent )
    , nMissionId_( nMissionId )
{
    pASNMsgOrder_ = new ASN_MsgAutomateOrder();
    pASNMsgOrder_->GetAsnMsg().order_id = (uint)(&agent_);
    pASNMsgOrder_->GetAsnMsg().oid_unite_executante = agent_.GetId();

    QLabel* pLabel = new QLabel( ENT_Tr::ConvertFromAutomataMission( E_AutomataMission( nMissionId_ ) ).c_str(), this );
    pLabel->setFrameStyle( QFrame::Box | QFrame::Sunken );
    pLabel->setAlignment( Qt::AlignCenter );
    QFont font = pLabel->font();
    font.setBold( true );
    pLabel->setFont( font );

    CreateInterface();
}


// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::CreateDefaultParameters
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
void AutomateMissionInterface::CreateDefaultParameters()
{
    ASN1T_MsgAutomateOrder& order = pASNMsgOrder_->GetAsnMsg();
    CreateLimits( order.oid_limite_gauche, order.oid_limite_droite, "Fixer limite 1", "Fixer limite 2", false );
    CreateLimaList( order.oid_limas, "Ajouter aux limas", true );
    ParamRadioBtnGroup<ASN1T_EnumAutomateOrderFormation>& buttonGroup
        = CreateRadioButtonGroup( order.formation, "Formation", false );
    buttonGroup.AddButton( "Un échelon", EnumAutomateOrderFormation::un_echelon );
    buttonGroup.AddButton( "Deux échelons", EnumAutomateOrderFormation::deux_echelons, true );

    CreateDirection( order.direction_dangereuse, "Direction dangeureuse", true );
}


// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface::OnOk
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
void AutomateMissionInterface::OnOk()
{
    if( ! CheckValidity() )
        return;

    Commit();
    pASNMsgOrder_->Send( 45 );

    agent_.Update( pASNMsgOrder_->GetAsnMsg() );
    parentWidget()->hide();
}

#include "AutomateMissionInterface_Gen.cpp"