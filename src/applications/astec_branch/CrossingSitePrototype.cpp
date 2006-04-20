// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "CrossingSitePrototype.h"
#include "ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype constructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype::CrossingSitePrototype( QWidget* parent )
    : ObjectPrototypeAttributes_ABC( parent, tr( "Site de franchissement" ) )
    , attr_( 0 )
{
    new QLabel( tr( "Largeur:" ), this );
    width_ = new QSpinBox( 1, 10000, 10, this );

    new QLabel( tr( "Profondeur:" ), this );
    depth_ = new QSpinBox( 0, 1000, 10, this );

    new QLabel( tr( "Vitesse courant:" ), this );
    speed_ = new QSpinBox( 0, 100, 1, this );

    new QLabel( tr( "Berges à aménager:" ), this );
    needsConstruction_ = new QCheckBox( this );
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
// Name: CrossingSitePrototype::CheckValidity
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
bool CrossingSitePrototype::CheckValidity() const
{
    return true;
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::Serialize
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CrossingSitePrototype::Serialize( ASN1T_MagicActionCreateObject& msg )
{
    if( msg.type != EnumObjectType::site_franchissement )
        return;

    attr_ = new ASN1T_AttrObjectSiteFranchissement();
    
    attr_->largeur           = width_->value();
    attr_->profondeur        = depth_->value();
    attr_->vitesse_courant   = speed_->value();
    attr_->berges_a_amenager = needsConstruction_->isOn();
    msg.m.attributs_specifiquesPresent    = 1;
    msg.attributs_specifiques.t           = T_AttrObjectSpecific_site_franchissement;
    msg.attributs_specifiques.u.site_franchissement = attr_;
}

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void CrossingSitePrototype::Clean()
{
    delete attr_;
}
