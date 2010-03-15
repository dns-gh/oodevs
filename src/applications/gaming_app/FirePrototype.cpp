// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FirePrototype.h"
#include "clients_kernel/FireClass.h"
#include "protocol/Protocol.h"
#include "tools/Iterator.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: FirePrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
FirePrototype::FirePrototype( QWidget* parent, const tools::Resolver_ABC< FireClass >& resolver, MsgsClientToSim::MsgMagicActionCreateObject& msg )
    : FirePrototype_ABC( parent, resolver )
    , msg_( msg )     
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: FirePrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
FirePrototype::~FirePrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void FirePrototype::Commit()
{
    if( const kernel::FireClass* fire = fireClass_->GetValue() )
    {
        msg_.mutable_attributes()->mutable_fire()->set_class_id( fire->GetId() );
        msg_.mutable_attributes()->mutable_fire()->set_heat( fire->GetDefaultHeat() );
    }
}

// -----------------------------------------------------------------------------
// Name: FirePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void FirePrototype::Clean()
{
    if( msg_.attributes().has_fire() )
        msg_.mutable_attributes()->clear_fire();
}
