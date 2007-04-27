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
#include "MissionInterfaceBuilder.h"
#include "gaming/ASN_Messages.h"
#include "gaming/ActionsModel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/MissionType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AutomateMissionInterface constructor
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
AutomateMissionInterface::AutomateMissionInterface( QWidget* parent, Entity_ABC& entity, const MissionType& mission, ActionController& controller
                                                  , Publisher_ABC& publisher, MissionInterfaceBuilder& builder, ActionsModel& model )
    : MissionInterface_ABC( parent, mission.GetName(), entity, controller )
    , model_              ( model )
    , publisher_          ( publisher )
    , mission_            ( mission )
{
    builder.Build( *this, entity, mission );
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
    if( model_.IsRecording() )
    {
        Action_ABC* action = model_.CreateAction( GetEntity(), mission_ );
        CommitTo( *action );
    }

    ASN_MsgAutomateOrder asn;
    asn().oid_unite_executante = GetEntity().GetId();
    asn().mission = mission_.GetId();
    asn().formation = (ASN1T_EnumAutomateOrderFormation)EnumAutomateOrderFormation::deux_echelons; // $$$$ SBO 2007-03-15: move to order.parameters
    CommitTo( asn().order_context );
    CommitTo( asn().parametres );
    asn.Send( publisher_ );
    Clean( asn().order_context );
    Clean( asn().parametres );
}
