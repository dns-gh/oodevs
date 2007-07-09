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
#include "MissionInterfaceBuilder.h"
#include "gaming/ASN_Messages.h"
#include "gaming/ActionsModel.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/Entity_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface constructor
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
UnitMissionInterface::UnitMissionInterface( QWidget* parent, Entity_ABC& entity, const MissionType& mission, ActionController& controller
                                          , Publisher_ABC& publisher, MissionInterfaceBuilder& builder, ActionsModel& model )
    : MissionInterface_ABC( parent, mission.GetName(), entity, controller )
    , model_              ( model )
    , publisher_          ( publisher )
    , mission_            ( mission )
{
    builder.Build( *this, entity, mission );
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
    if( model_.IsRecording() )
    {
        Action_ABC* action = model_.CreateAction( GetEntity(), mission_ );
        CommitTo( *action );
    }

    ASN_MsgUnitOrder asn;
    asn().oid = GetEntity().GetId();
    asn().mission = mission_.GetId();
    CommitTo( asn().parametres );
    CommitTo( asn().order_context );
    asn.Send( publisher_ );
    Clean( asn().parametres );
    Clean( asn().order_context );
}
