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
#include "MissionInterfaceBuilder.h"
#include "gaming/ASN_Messages.h"
#include "gaming/ActionsModel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/FragOrderType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface constructor
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
FragmentaryOrderInterface::FragmentaryOrderInterface( QWidget* parent, Entity_ABC& entity, const FragOrderType& fragOrder, ActionController& controller
                                                    , Publisher_ABC& publisher, MissionInterfaceBuilder& builder, ActionsModel& model )
    : MissionInterface_ABC( parent, entity, controller )
    , model_              ( model )
    , publisher_          ( publisher )
    , fragOrder_          ( fragOrder )
{
    CreateTitle( fragOrder.GetName() );
    builder.Build( *this, entity, fragOrder );
    CreateOkCancelButtons();
}

// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
FragmentaryOrderInterface::~FragmentaryOrderInterface()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface::Publish
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void FragmentaryOrderInterface::Publish()
{
    if( model_.IsRecording() )
    {
        Action_ABC* action = model_.CreateAction( GetEntity(), fragOrder_ );
        CommitTo( *action );
    }

    ASN_MsgFragOrder asn;
    asn().oid_unite_executante = GetEntity().GetId();
    asn().frag_order = fragOrder_.GetId();
    CommitTo( asn().parametres );
    asn.Send( publisher_ );
    Clean( asn().parametres );
}
