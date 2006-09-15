// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FragmentaryOrderInterface.h"

#include "MissionPanel.h"
#include "gaming/ASN_Messages.h"
#include "gaming/ASN_Types.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_gui/Tools.h"

#include "ENT/ENT_Tr.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface constructor
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
FragmentaryOrderInterface::FragmentaryOrderInterface( QWidget* parent, Agent_ABC& agent, uint nMissionId, ActionController& controller, ParametersLayer& layer, const CoordinateConverter_ABC& converter, AgentKnowledgeConverter_ABC& knowledgeConverter, ObjectKnowledgeConverter_ABC& objectKnowledgeConverter, const ObjectTypes& objectTypes, Publisher_ABC& publisher )
: MissionInterface_ABC( parent, agent, controller, layer, converter, knowledgeConverter, objectKnowledgeConverter, objectTypes )
    , publisher_      ( publisher )
    , nMissionId_     ( nMissionId )
{
    pASNMsgOrder_ = new ASN_MsgOrderConduite();
    pASNMsgOrder_->GetAsnMsg().unit_id = agent.GetId();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = nMissionId_ + 1; // $$$$ AGE 2006-04-21: décallage asn enum. corriger.

    QLabel* pLabel = new QLabel( ENT_Tr::ConvertFromFragOrder( ( E_FragOrder )nMissionId_ ).c_str(), this );
    pLabel->setFrameStyle( QFrame::Box | QFrame::Sunken );
    pLabel->setAlignment( Qt::AlignCenter );
    QFont font = pLabel->font();
    font.setBold( true );
    pLabel->setFont( font );

    CreateInterface();

    QWidget* pSpacer = new QWidget( this );
    setStretchFactor( pSpacer, 100 );
}

// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface::OnOk
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
void FragmentaryOrderInterface::OnOk()
{
    if( ! CheckValidity() )
        return;

    Commit();

    pASNMsgOrder_->GetAsnMsg().order_id = 43;
    pASNMsgOrder_->Send( publisher_, 36999 );
    
    parentWidget()->hide();
}

#include "FragmentaryOrderInterface_Gen.cpp"