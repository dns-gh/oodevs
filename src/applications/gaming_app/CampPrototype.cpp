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

#include "clients_kernel/Agent_ABC.h"
#include "gaming/ASN_Messages.h"

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
        attrPrisonners_ = new ASN1T_AttrObjectCampPrisonniers();
        attrPrisonners_->tc2 = tc2s_->GetValue()->GetId();
        msg_.m.attributs_specifiquesPresent    = 1;
        msg_.attributs_specifiques.t           = T_AttrObjectSpecific_camp_prisonniers;
        msg_.attributs_specifiques.u.camp_prisonniers= attrPrisonners_;
    }
    else if( msg_.type == EnumObjectType::camp_refugies )
    {
        attrRefugees_ = new ASN1T_AttrObjectCampRefugies();
        attrRefugees_->tc2 = tc2s_->GetValue()->GetId();
        msg_.m.attributs_specifiquesPresent    = 1;
        msg_.attributs_specifiques.t           = T_AttrObjectSpecific_camp_refugies;
        msg_.attributs_specifiques.u.camp_refugies = attrRefugees_;
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
