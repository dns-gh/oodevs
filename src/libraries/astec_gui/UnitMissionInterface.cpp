// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "UnitMissionInterface.h"

#include "MissionPanel.h"
#include "astec_gaming/ASN_Messages.h"
#include "astec_gaming/ASN_Types.h"
#include "astec_gaming/Agent_ABC.h"
#include "OptionalParamFunctor_ABC.h"

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface constructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
UnitMissionInterface::UnitMissionInterface( QWidget* parent, Agent_ABC& agent, uint nMissionId, ActionController& controller, ParametersLayer& layer, const CoordinateConverter_ABC& converter, AgentKnowledgeConverter_ABC& knowledgeConverter, const ObjectTypes& objectTypes, Publisher_ABC& publisher )
    : MissionInterface_ABC( parent, agent, controller, layer, converter, knowledgeConverter, objectTypes )
    , publisher_( publisher )
    , agent_( agent )
    , nMissionId_( nMissionId )
{
    pASNMsgOrder_ = new ASN_MsgPionOrder();
    pASNMsgOrder_->GetAsnMsg().order_id = (uint)(&agent);
    pASNMsgOrder_->GetAsnMsg().oid_unite_executante = agent.GetId();

    QLabel* pLabel = new QLabel( ENT_Tr::ConvertFromUnitMission( E_UnitMission( nMissionId_ ) ).c_str(), this );
    pLabel->setFrameStyle( QFrame::Box | QFrame::Sunken );
    pLabel->setAlignment( Qt::AlignCenter );
    QFont font = pLabel->font();
    font.setBold( true );
    pLabel->setFont( font );

    CreateInterface();
}

namespace
{
    struct OrderBlahBlah : public OptionalParamFunctor_ABC
    {
        OrderBlahBlah( ASN1T_MsgPionOrder& ) {};
        virtual void SetOptionalPresent() {
            // NOTHING
        }
    };
};

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::CreateDefaultParameters
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
void UnitMissionInterface::CreateDefaultParameters()
{
    ASN1T_MsgPionOrder& order = pASNMsgOrder_->GetAsnMsg();
    CreateLimits( order.oid_limite_gauche, order.oid_limite_droite, "Fixer limite 1", "Fixer limite 2", BuildOptionalParamFunctor< OrderBlahBlah, ASN1T_MsgPionOrder >( order ) );
    CreateLimaList( order.oid_limas, "Ajouter aux limas" );
    CreateDirection( order.direction_dangereuse, "Direction dangeureuse" );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::OnOk
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void UnitMissionInterface::OnOk()
{
    if( ! CheckValidity() )
        return;

    Commit();
    ASN1T_MsgPionOrder& order = pASNMsgOrder_->GetAsnMsg();
    order.m.oid_limite_gauchePresent = (order.oid_limite_gauche != MIL_NULL_LINE_ID) ? 1 : 0;
    order.m.oid_limite_droitePresent = (order.oid_limite_droite != MIL_NULL_LINE_ID) ? 1 : 0;
    pASNMsgOrder_->Send( publisher_, 45 );

    agent_.Update( order );
    parentWidget()->hide();
}

#include "UnitMissionInterface_Gen.cpp"