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
#include "MissionInterfaceFactory.h"
#include "MissionInterfaceBuilder.h"
#include "gaming/ASN_Messages.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Mission.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationMissionInterface constructor
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
PopulationMissionInterface::PopulationMissionInterface( QWidget* parent, Entity_ABC& entity, const Mission& mission, ActionController& controller
                                                      , Publisher_ABC& publisher, MissionInterfaceFactory& factory, MissionInterfaceBuilder& builder )
    : MissionInterface_ABC( parent, entity, controller )
    , publisher_          ( publisher )
    , mission_            ( mission )
{
    CreateTitle( mission.GetName() );
    builder.Begin( *this, entity );
    factory.CreatePopulationMissionInterface( builder, mission.GetId() );
    builder.End();
    CreateOkCancelButtons();
}

// -----------------------------------------------------------------------------
// Name: PopulationMissionInterface destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
PopulationMissionInterface::~PopulationMissionInterface()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationMissionInterface::Publish
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void PopulationMissionInterface::Publish()
{
    ASN_MsgPopulationOrder asn;
    ASN1T_MsgPopulationOrder& order = asn.GetAsnMsg();
    order.oid_unite_executante = GetEntity().GetId();
    order.mission = mission_.GetId();
    CommitTo( order.parametres );
    asn.Send( publisher_ );
    Clean( order.parametres );
}
