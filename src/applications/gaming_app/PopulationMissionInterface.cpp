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
#include "MissionInterfaceBuilder.h"
#include "gaming/ASN_Messages.h"
#include "gaming/ActionsModel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/MissionType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationMissionInterface constructor
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
PopulationMissionInterface::PopulationMissionInterface( QWidget* parent, Entity_ABC& entity, const MissionType& mission, ActionController& controller
                                                      , Publisher_ABC& publisher, MissionInterfaceBuilder& builder, ActionsModel& model )
    : MissionInterface_ABC( parent, entity, controller )
    , model_              ( model )
    , publisher_          ( publisher )
    , mission_            ( mission )
{
    CreateTitle( mission.GetName() );
    builder.Build( *this, entity, mission );
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
    if( model_.IsRecording() )
    {
        Action_ABC* action = model_.CreateAction( GetEntity(), mission_ );
        CommitTo( *action );
    }

    ASN_MsgPopulationOrder asn;
    asn().oid_unite_executante = GetEntity().GetId();
    asn().mission = mission_.GetId();
    CommitTo( asn().parametres );
    asn.Send( publisher_ );
    Clean( asn().parametres );
}
