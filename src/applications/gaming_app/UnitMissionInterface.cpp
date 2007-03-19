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
#include "gaming/ActionsModel.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/Entity_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface constructor
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
UnitMissionInterface::UnitMissionInterface( QWidget* parent, Entity_ABC& entity, const Mission& mission, ActionController& controller
                                          , Publisher_ABC& publisher, MissionInterfaceFactory& factory, MissionInterfaceBuilder& builder, ActionsModel& model )
    : MissionInterface_ABC( parent, entity, controller )
    , model_              ( model )
    , publisher_          ( publisher )
    , mission_            ( mission )
{
    CreateTitle( mission.GetName() );
    builder.Begin( *this, entity );
    factory.CreateUnitMissionInterface( builder, mission.GetId() );
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
// Name: UnitMissionInterface::Publish
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void UnitMissionInterface::Publish()
{
    Action_ABC* action = model_.CreateAction( GetEntity(), mission_ );
    CommitTo( *action );

    ASN_MsgPionOrder asn;
    ASN1T_MsgPionOrder& order = asn.GetAsnMsg();
    order.oid_unite_executante = GetEntity().GetId();
    order.mission = mission_.GetId();
    CommitTo( order.parametres );
    CommitTo( order.order_context );
    asn.Send( publisher_ );
    Clean( order.parametres );
    Clean( order.order_context );
}
