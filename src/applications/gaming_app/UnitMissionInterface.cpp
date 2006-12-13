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
#include "MissionInterfaceFactory.h"
#include "MissionInterfaceBuilder.h"
#include "gaming/ASN_Messages.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/Entity_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface constructor
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
UnitMissionInterface::UnitMissionInterface( QWidget* parent, Entity_ABC& entity, const Mission& mission, ActionController& controller
                                          , Publisher_ABC& publisher, MissionInterfaceFactory& factory, MissionInterfaceBuilder& builder )
    : MissionInterface_ABC( parent, entity, controller )
    , publisher_          ( publisher )
    , order_              ( new ASN_MsgPionOrder() )
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
// Name: UnitMissionInterface destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
UnitMissionInterface::~UnitMissionInterface()
{
    // NOTHING
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
    ASN1T_MsgPionOrder& order = order_->GetAsnMsg();
    // $$$$ SBO 2006-11-14: Use BuildOptionalParameter instead ?
    order.order_context.m.limite_gauchePresent = (order.order_context.limite_gauche.vecteur_point.n > 1) ? 1 : 0;
    order.order_context.m.limite_droitePresent = (order.order_context.limite_droite.vecteur_point.n > 1) ? 1 : 0;
    order_->Send( publisher_, 45 );
    parentWidget()->hide();
}
