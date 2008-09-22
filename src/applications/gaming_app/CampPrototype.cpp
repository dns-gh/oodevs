// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "CampPrototype.h"

#include "clients_kernel/Automat_ABC.h"
#include "game_asn/SimulationSenders.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: CampPrototype::CampPrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CampPrototype::CampPrototype( QWidget* parent, Controllers& controllers, ASN1T_MagicActionCreateObject& msg )
    : CampPrototype_ABC( parent, controllers )
    , msg_( msg )
    , attrPrisonners_( 0 )
    , attrRefugees_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: CampPrototype::~CampPrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CampPrototype::~CampPrototype()
{
    Clean();
}

// -----------------------------------------------------------------------------
// Name: CampPrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CampPrototype::Commit()
{
    if( msg_.type == EnumObjectType::camp_prisonniers )
    {
        attrPrisonners_ = new ASN1T_ObjectAttributesPrisonerCamp();
        attrPrisonners_->tc2 = tc2s_->GetValue()->GetId();
        msg_.m.specific_attributesPresent    = 1;
        msg_.specific_attributes.t           = T_ObjectAttributesSpecific_prisoner_camp;
        msg_.specific_attributes.u.prisoner_camp= attrPrisonners_;
    }
    else if( msg_.type == EnumObjectType::camp_refugies )
    {
        attrRefugees_ = new ASN1T_ObjectAttributesRefugeeCamp();
        attrRefugees_->tc2 = tc2s_->GetValue()->GetId();
        msg_.m.specific_attributesPresent    = 1;
        msg_.specific_attributes.t           = T_ObjectAttributesSpecific_refugee_camp;
        msg_.specific_attributes.u.refugee_camp = attrRefugees_;
    }
}

// -----------------------------------------------------------------------------
// Name: CampPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void CampPrototype::Clean()
{
    delete attrPrisonners_; attrPrisonners_ = 0;
    delete attrRefugees_; attrRefugees_ = 0;
}
