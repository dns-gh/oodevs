// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AutomateMissionInterface.h"
#include "OptionalParamFunctor_ABC.h"
#include "gaming/ASN_Messages.h"
#include "gaming/ASN_Types.h"
#include "clients_kernel/Agent_ABC.h"

#include "ENT/ENT_Tr.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface constructor
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
AutomateMissionInterface::AutomateMissionInterface( QWidget* parent, Agent_ABC& agent, uint nMissionId, ActionController& controller, ParametersLayer& layer, const CoordinateConverter_ABC& converter, AgentKnowledgeConverter_ABC& knowledgeConverter, const ObjectTypes& objectTypes, Publisher_ABC& publisher )
    : MissionInterface_ABC( parent, agent, controller, layer, converter, knowledgeConverter, objectTypes )
    , publisher_( publisher )
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
    CreateLimits( order.oid_limite_gauche, order.oid_limite_droite, "Fixer limite 1", "Fixer limite 2" );
    CreateLimaList( order.oid_limas, "Ajouter aux limas" );
    ParamRadioBtnGroup<ASN1T_EnumAutomateOrderFormation>& buttonGroup
        = CreateRadioButtonGroup( order.formation, "Formation" );
    buttonGroup.AddButton( "Un échelon", EnumAutomateOrderFormation::un_echelon );
    buttonGroup.AddButton( "Deux échelons", EnumAutomateOrderFormation::deux_echelons, true );

    CreateDirection( order.direction_dangereuse, "Direction dangeureuse" );
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
    pASNMsgOrder_->Send( publisher_, 45 );

    agent_.Update( pASNMsgOrder_->GetAsnMsg() );
    parentWidget()->hide();
}

#include "AutomateMissionInterface_Gen.cpp"