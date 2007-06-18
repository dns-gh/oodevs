// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MinePrototype.h"
#include "gaming/ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: MinePrototype constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MinePrototype::MinePrototype( QWidget* parent, ASN1T_MagicActionCreateObject& msg )
    : MinePrototype_ABC( parent )
    , msg_( msg )
    , attrMineJam_( 0 )
    , attrLinearMinedArea_( 0 )
    , attrDispersedMinedArea_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MinePrototype destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MinePrototype::~MinePrototype()
{
    Clean();
}

// -----------------------------------------------------------------------------
// Name: MinePrototype::Commit
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MinePrototype::Commit()
{
    if( msg_.type == EnumObjectType::bouchon_mines )
    {
        attrMineJam_ = new ASN1T_ObjectAttributesMineJam();
        attrMineJam_->delai_activite_mines = activityTime_->value();
        msg_.m.attributs_specifiquesPresent = 1;
        msg_.attributs_specifiques.t        = T_ObjectAttributesSpecific_bouchon_mines;
        msg_.attributs_specifiques.u.bouchon_mines = attrMineJam_;
    }
    else if( msg_.type == EnumObjectType::zone_minee_lineaire )
    {
        attrLinearMinedArea_ = new ASN1T_ObjectAttributesLinearMineArea();
        attrLinearMinedArea_->delai_activite_mines = activityTime_->value();
        attrLinearMinedArea_->densite = density_->text().toFloat();
        msg_.m.attributs_specifiquesPresent = 1;
        msg_.attributs_specifiques.t        = T_ObjectAttributesSpecific_zone_minee_lineaire;
        msg_.attributs_specifiques.u.zone_minee_lineaire = attrLinearMinedArea_;
    }
    else if( msg_.type == EnumObjectType::zone_minee_par_dispersion )
    {
        attrDispersedMinedArea_ = new ASN1T_ObjectAttributesDispersedMineArea();
        attrDispersedMinedArea_->delai_activite_mines = activityTime_->value();
        attrDispersedMinedArea_->densite = density_->text().toFloat();
        msg_.m.attributs_specifiquesPresent = 1;
        msg_.attributs_specifiques.t        = T_ObjectAttributesSpecific_zone_minee_par_dispersion;
        msg_.attributs_specifiques.u.zone_minee_par_dispersion = attrDispersedMinedArea_;
    }
}

// -----------------------------------------------------------------------------
// Name: MinePrototype::Clean
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MinePrototype::Clean()
{
    delete attrMineJam_; attrMineJam_ = 0;
    delete attrLinearMinedArea_; attrLinearMinedArea_ = 0;
    delete attrDispersedMinedArea_; attrDispersedMinedArea_ = 0;
}
