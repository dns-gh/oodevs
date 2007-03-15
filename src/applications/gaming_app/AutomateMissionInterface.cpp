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
    , mission_            ( mission )
{
    CreateTitle( mission.GetName() );
    builder.Begin( *this, entity );
    factory.CreateAutomatMissionInterface( builder, mission.GetId() );
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
// Name: AutomateMissionInterface::Publish
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void AutomateMissionInterface::Publish()
{
    ASN_MsgAutomateOrder asn;
    ASN1T_MsgAutomateOrder& order = asn.GetAsnMsg();
    order.oid_unite_executante = GetEntity().GetId();
    order.mission = mission_.GetId();
    order.formation = (ASN1T_EnumAutomateOrderFormation)EnumAutomateOrderFormation::deux_echelons; // $$$$ SBO 2007-03-15: move to order.parameters
    CommitTo( order.order_context );
    CommitTo( order.parametres );
    asn.Send( publisher_ );
    Clean( order.order_context );
    Clean( order.parametres );
}
