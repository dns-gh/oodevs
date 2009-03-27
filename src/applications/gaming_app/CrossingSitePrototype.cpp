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
#include "game_asn/SimulationSenders.h"
#include "clients_kernel/Units.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype constructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype::CrossingSitePrototype( QWidget* parent, ASN1T_MagicActionCreateObject& msg )
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
    msg_.attributes.m.crossing_sitePresent = 1;
    msg_.attributes.crossing_site.width                 = width_->value();
    msg_.attributes.crossing_site.depth                 = depth_->value();
    msg_.attributes.crossing_site.flow_rate             = speed_->value();
    msg_.attributes.crossing_site.banks_require_fitting = needsConstruction_->isOn();    
}

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void CrossingSitePrototype::Clean()
{
    msg_.attributes.m.crossing_sitePresent = 0;
}
