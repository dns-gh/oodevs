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
#include "MissionInterfaceFactory.h"
#include "MissionInterfaceBuilder.h"
#include "gaming/ASN_Messages.h"
#include "gaming/ActionsModel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/FragOrder.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface constructor
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
FragmentaryOrderInterface::FragmentaryOrderInterface( QWidget* parent, Entity_ABC& entity, const FragOrder& fragOrder, ActionController& controller
                                                    , Publisher_ABC& publisher, MissionInterfaceFactory& factory, MissionInterfaceBuilder& builder, ActionsModel& model )
    : MissionInterface_ABC( parent, entity, controller )
    , model_              ( model )
    , publisher_          ( publisher )
    , fragOrder_          ( fragOrder )
{
    CreateTitle( fragOrder.GetName() );
    builder.Begin( *this, entity );
    factory.CreateFragOrderInterface( builder, fragOrder.GetId() );
    builder.End();
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
    Action_ABC* action = model_.CreateAction( GetEntity(), fragOrder_ );
    CommitTo( *action );

    ASN_MsgFragOrder asn;
    ASN1T_MsgFragOrder& order = asn.GetAsnMsg();

    order.oid_unite_executante = GetEntity().GetId();
    order.frag_order = fragOrder_.GetId();
    CommitTo( order.parametres );
    asn.Send( publisher_ );
    Clean( order.parametres );
}
