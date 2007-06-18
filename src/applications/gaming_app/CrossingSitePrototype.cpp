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
#include "gaming/ASN_Messages.h"
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
    
    attr_->largeur           = width_->value();
    attr_->profondeur        = depth_->value();
    attr_->vitesse_courant   = speed_->value();
    attr_->berges_a_amenager = needsConstruction_->isOn();
    msg_.m.attributs_specifiquesPresent    = 1;
    msg_.attributs_specifiques.t           = T_ObjectAttributesSpecific_site_franchissement;
    msg_.attributs_specifiques.u.site_franchissement = attr_;
}

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void CrossingSitePrototype::Clean()
{
    delete attr_; attr_ = 0;
}
