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
#include "tools/Iterator.h"
#include "game_asn/SimulationSenders.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: FirePrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
FirePrototype::FirePrototype( QWidget* parent, const tools::Resolver_ABC< FireClass >& resolver, ASN1T_MagicActionCreateObject& msg )
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
    msg_.attributes.m.firePresent = 1;
    msg_.attributes.fire.class_id = fireClass_->GetValue()->GetId();
//    msg_.attributes.fire.heat = fireClass_->heat;
}

// -----------------------------------------------------------------------------
// Name: FirePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void FirePrototype::Clean()
{
    fireClass_ = 0;
}
