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

using namespace gui;

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

    attr_ = new ASN1T_ObjectAttributesLogisticRoute();
    attr_->flow_rate        = flow_->value();
    attr_->width            = width_->value();
    attr_->length           = length_->value();
    attr_->max_weight       = maxWeight_->value();
    attr_->equipped         = equipped_->isOn();
    msg_.m.specific_attributesPresent    = 1;
    msg_.specific_attributes.t           = T_ObjectAttributesSpecific_logistic_route;
    msg_.specific_attributes.u.logistic_route = attr_;
}

// -----------------------------------------------------------------------------
// Name: LogisticRoutePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void LogisticRoutePrototype::Clean()
{
    delete attr_; attr_ = 0;
}
