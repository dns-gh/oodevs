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
#include "MissionInterfaceFactory.h"
#include "MissionInterfaceBuilder.h"
#include "gaming/ASN_Messages.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Mission.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface constructor
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
AutomateMissionInterface::AutomateMissionInterface( QWidget* parent, Entity_ABC& entity, const Mission& mission, ActionController& controller
                                                  , Publisher_ABC& publisher, MissionInterfaceFactory& factory, MissionInterfaceBuilder& builder )
    : MissionInterface_ABC( parent, entity, controller )
    , publisher_          ( publisher )
    , order_              ( new ASN_MsgAutomateOrder() )
{
    order_->GetAsnMsg().oid_unite_executante = entity.GetId();
    order_->GetAsnMsg().mission = mission.GetId();

    QLabel* pLabel = new QLabel( mission.GetName(), this );
    pLabel->setFrameStyle( QFrame::Box | QFrame::Sunken );
    pLabel->setAlignment( Qt::AlignCenter );
    QFont font = pLabel->font();
    font.setBold( true );
    pLabel->setFont( font );

    builder.Begin( *this, entity );
    factory.CreateMissionInterface( builder, mission.GetId(), order_->GetAsnMsg() );
    builder.End();
    CreateOkCancelButtons();
}

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
AutomateMissionInterface::~AutomateMissionInterface()
{
    // NOTHING
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
    ASN1T_MsgAutomateOrder& order = order_->GetAsnMsg();
    order.order_context.m.limite_gauchePresent = 1; // $$$$ SBO 2006-11-23: move somehow in ParamLimits
    order.order_context.m.limite_droitePresent = 1; // $$$$ SBO 2006-11-23: 
    order_->Send( publisher_, 45 );
    parentWidget()->hide();
}
