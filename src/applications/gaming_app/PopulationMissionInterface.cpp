// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PopulationMissionInterface.h"
#include "gaming/ASN_Messages.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_gui/Tools.h"

#include "ENT/ENT_Tr.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface constructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
PopulationMissionInterface::PopulationMissionInterface( QWidget* parent, Population_ABC& population, unsigned long nMissionId, ActionController& controller, ParametersLayer& layer, const CoordinateConverter_ABC& converter, AgentKnowledgeConverter_ABC& knowledgeConverter, const ObjectTypes& objectTypes, Publisher_ABC& publisher )
    : MissionInterface_ABC( parent, population, controller, layer, converter, knowledgeConverter, objectTypes )
    , publisher_( publisher )
    , nMissionId_( nMissionId )
{
    pASNMsgOrder_ = new ASN_MsgPopulationOrder();
    pASNMsgOrder_->GetAsnMsg().order_id = (uint)(&population);
    pASNMsgOrder_->GetAsnMsg().oid_unite_executante = population.GetId();

    QLabel* pLabel = new QLabel( ENT_Tr::ConvertFromPopulationMission( E_PopulationMission( nMissionId_ ) ).c_str(), this );
    pLabel->setFrameStyle( QFrame::Box | QFrame::Sunken );
    pLabel->setAlignment( Qt::AlignCenter );
    QFont font = pLabel->font();
    font.setBold( true );
    pLabel->setFont( font );

    CreateInterface();
}

// -----------------------------------------------------------------------------
// Name: PopulationMissionInterface::CreateDefaultParameters
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void PopulationMissionInterface::CreateDefaultParameters()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::OnOk
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void PopulationMissionInterface::OnOk()
{
    if( ! CheckValidity() )
        return;

    Commit();
    pASNMsgOrder_->Send( publisher_, 45 );
    parentWidget()->hide();
}

#include "PopulationMissionInterface_Gen.cpp"
