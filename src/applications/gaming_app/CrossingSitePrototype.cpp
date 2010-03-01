// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "CrossingSitePrototype.h"
#include "clients_kernel/Units.h"
#include "protocol/Protocol.h"       

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype constructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype::CrossingSitePrototype( QWidget* parent, MsgsClientToSim::MsgMagicActionCreateObject& msg )
    : CrossingSitePrototype_ABC( parent )
    , msg_( msg )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype destructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype::~CrossingSitePrototype()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CrossingSitePrototype::Commit()
{
    msg_.mutable_attributes()->mutable_crossing_site()->set_width                 ( width_->value() );
    msg_.mutable_attributes()->mutable_crossing_site()->set_depth                 ( depth_->value() );
    msg_.mutable_attributes()->mutable_crossing_site()->set_flow_rate             ( speed_->value() );
    msg_.mutable_attributes()->mutable_crossing_site()->set_banks_require_fitting ( needsConstruction_->isOn() );    
}

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void CrossingSitePrototype::Clean()
{
    if( msg_.attributes().has_crossing_site() )
        msg_.mutable_attributes()->clear_crossing_site();
}
