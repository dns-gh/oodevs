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
    , attr_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype destructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype::~CrossingSitePrototype()
{
    Clean();
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CrossingSitePrototype::Commit()
{
    if( msg_.type != EnumObjectType::site_franchissement )
        return;

    attr_ = new ASN1T_ObjectAttributesCrossingSite();
    
    attr_->width                 = width_->value();
    attr_->depth                 = depth_->value();
    attr_->flow_rate             = speed_->value();
    attr_->banks_require_fitting = needsConstruction_->isOn();
    msg_.m.specific_attributesPresent    = 1;
    msg_.specific_attributes.t           = T_ObjectAttributesSpecific_crossing_site;
    msg_.specific_attributes.u.crossing_site = attr_;
}

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void CrossingSitePrototype::Clean()
{
    delete attr_; attr_ = 0;
}
