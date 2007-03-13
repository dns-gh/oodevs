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
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/FragOrder.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface constructor
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
FragmentaryOrderInterface::FragmentaryOrderInterface( QWidget* parent, Entity_ABC& entity, const FragOrder& fragOrder, ActionController& controller
                                                    , Publisher_ABC& publisher, MissionInterfaceFactory& factory, MissionInterfaceBuilder& builder )
    : MissionInterface_ABC( parent, entity, controller )
    , publisher_          ( publisher )
    , order_              ( new ASN_MsgFragOrder() )
{
    order_->GetAsnMsg().oid_unite_executante = entity.GetId();
    order_->GetAsnMsg().frag_order = fragOrder.GetId();

    QLabel* pLabel = new QLabel( fragOrder.GetName(), this );
    pLabel->setFrameStyle( QFrame::Box | QFrame::Sunken );
    pLabel->setAlignment( Qt::AlignCenter );
    QFont font = pLabel->font();
    font.setBold( true );
    pLabel->setFont( font );

    builder.Begin( *this, entity );
    factory.CreateMissionInterface( builder, fragOrder.GetId(), order_->GetAsnMsg() );
    builder.End();
    CreateOkCancelButtons();

    QWidget* pSpacer = new QWidget( this ); // $$$$ SBO 2006-11-23: ?
    setStretchFactor( pSpacer, 100 );       // $$$$ SBO 2006-11-23: ?
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
    order_->Send( publisher_, 36999 );
}
