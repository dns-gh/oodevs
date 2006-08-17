// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticRoutePrototype.h"
#include "gaming/ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: LogisticRoutePrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
LogisticRoutePrototype::LogisticRoutePrototype( QWidget* parent, ASN1T_MagicActionCreateObject& msg )
    : LogisticRoutePrototype_ABC( parent )
    , msg_( msg )
    , attr_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticRoutePrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
LogisticRoutePrototype::~LogisticRoutePrototype()
{
    Clean();
}

// -----------------------------------------------------------------------------
// Name: LogisticRoutePrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void LogisticRoutePrototype::Commit()
{
    if( msg_.type != EnumObjectType::itineraire_logistique )
        return;

    attr_ = new ASN1T_AttrObjectItineraireLogistique();
    attr_->debit              = flow_->value();
    attr_->largeur            = width_->value();
    attr_->longueur           = length_->value();
    attr_->poids_max_supporte = maxWeight_->value();
    attr_->itineraire_equipe  = equipped_->isOn();
    msg_.m.attributs_specifiquesPresent    = 1;
    msg_.attributs_specifiques.t           = T_AttrObjectSpecific_itineraire_logistique;
    msg_.attributs_specifiques.u.itineraire_logistique = attr_;
}

// -----------------------------------------------------------------------------
// Name: LogisticRoutePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void LogisticRoutePrototype::Clean()
{
    delete attr_; attr_ = 0;
}
