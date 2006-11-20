// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UnitMissionInterface.h"

#include "MissionPanel.h"
#include "gaming/ASN_Messages.h"
#include "gaming/ASN_Types.h"
#include "OptionalParamFunctor_ABC.h"
#include "clients_kernel/Entity_ABC.h"

#include "ENT/ENT_Tr.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface constructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
UnitMissionInterface::UnitMissionInterface( QWidget* parent, Entity_ABC& agent, uint nMissionId, ActionController& controller, ParametersLayer& layer, const CoordinateConverter_ABC& converter, AgentKnowledgeConverter_ABC& knowledgeConverter, ObjectKnowledgeConverter_ABC& objectKnowledgeConverter, const ObjectTypes& objectTypes, Publisher_ABC& publisher )
    : MissionInterface_ABC( parent, agent, controller, layer, converter, knowledgeConverter, objectKnowledgeConverter, objectTypes )
    , publisher_( publisher )
    , agent_( agent )
    , nMissionId_( nMissionId )
{
    pASNMsgOrder_ = new ASN_MsgPionOrder();
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
    CreateLimits( order.order_context.limite_gauche, order.order_context.limite_droite, "Fixer limite 1", "Fixer limite 2", BuildOptionalParamFunctor< OrderBlahBlah, ASN1T_MsgPionOrder >( order ) );
    CreateLimaList( order.order_context.limas, "Ajouter aux limas", BuildOptionalParamFunctor< OrderBlahBlah, ASN1T_MsgPionOrder >( order ) );
    CreateDirection( order.order_context.direction_dangereuse, "Direction dangeureuse" );
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
    // $$$$ SBO 2006-11-14: Use BuildOptionalParameter instead ?
    order.order_context.m.limite_gauchePresent = (order.order_context.limite_gauche.vecteur_point.n > 1) ? 1 : 0;
    order.order_context.m.limite_droitePresent = (order.order_context.limite_droite.vecteur_point.n > 1) ? 1 : 0;
    pASNMsgOrder_->Send( publisher_, 45 );
    parentWidget()->hide();
}

#include "UnitMissionInterface_Gen.cpp"