// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "LogisticRoutePrototype.h"
#include "astec_gaming/ASN_Messages.h"
#include "astec_gaming/Units.h"

// -----------------------------------------------------------------------------
// Name: LogisticRoutePrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
LogisticRoutePrototype::LogisticRoutePrototype( QWidget* parent )
    : ObjectPrototypeAttributes_ABC( parent, tr( "Itinéraire logistique" ) )
    , attr_( 0 )
{
    new QLabel( tr( "Débit:" ), this );
    flow_ = new QSpinBox( 0, 10000, 1, this );
    flow_->setSuffix( Units::vehiclesPerHour );

    new QLabel( tr( "Largeur:" ), this );
    width_ = new QSpinBox( 0, 10000, 1, this );
    width_->setSuffix( Units::meters );

    new QLabel( tr( "Longueur:" ), this );
    length_ = new QSpinBox( 0, 10000, 1, this );
    length_->setSuffix( Units::meters );

    new QLabel( tr( "Poids maximum:" ), this );
    maxWeight_ = new QSpinBox( 0, 10000, 1, this );
    maxWeight_->setSuffix( Units::tons );

    equipped_ = new QCheckBox( tr( "Equipé:" ), this );
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
// Name: LogisticRoutePrototype::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool LogisticRoutePrototype::CheckValidity() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: LogisticRoutePrototype::Serialize
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void LogisticRoutePrototype::Serialize( ASN1T_MagicActionCreateObject& msg )
{
    if( msg.type != EnumObjectType::itineraire_logistique )
        return;

    attr_ = new ASN1T_AttrObjectItineraireLogistique();
    attr_->debit              = flow_->value();
    attr_->largeur            = width_->value();
    attr_->longueur           = length_->value();
    attr_->poids_max_supporte = maxWeight_->value();
    attr_->itineraire_equipe  = equipped_->isOn();
    msg.m.attributs_specifiquesPresent    = 1;
    msg.attributs_specifiques.t           = T_AttrObjectSpecific_itineraire_logistique;
    msg.attributs_specifiques.u.itineraire_logistique = attr_;
}

// -----------------------------------------------------------------------------
// Name: LogisticRoutePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void LogisticRoutePrototype::Clean()
{
    delete attr_; attr_ = 0;
}
