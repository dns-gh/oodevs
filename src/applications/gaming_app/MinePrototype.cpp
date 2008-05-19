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
#include "gaming/SimulationMessages.h"

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
        attrMineJam_->activity_time = activityTime_->value();
        msg_.m.specific_attributesPresent = 1;
        msg_.specific_attributes.t        = T_ObjectAttributesSpecific_mine_jam;
        msg_.specific_attributes.u.mine_jam = attrMineJam_;
    }
    else if( msg_.type == EnumObjectType::zone_minee_lineaire )
    {
        attrLinearMinedArea_ = new ASN1T_ObjectAttributesLinearMineArea();
        attrLinearMinedArea_->activity_time = activityTime_->value();
        attrLinearMinedArea_->density = density_->text().toFloat();
        msg_.m.specific_attributesPresent = 1;
        msg_.specific_attributes.t        = T_ObjectAttributesSpecific_linear_mine_area;
        msg_.specific_attributes.u.linear_mine_area = attrLinearMinedArea_;
    }
    else if( msg_.type == EnumObjectType::zone_minee_par_dispersion )
    {
        attrDispersedMinedArea_ = new ASN1T_ObjectAttributesDispersedMineArea();
        attrDispersedMinedArea_->activity_time = activityTime_->value();
        attrDispersedMinedArea_->density = density_->text().toFloat();
        msg_.m.specific_attributesPresent = 1;
        msg_.specific_attributes.t        = T_ObjectAttributesSpecific_dispersed_mine_area;
        msg_.specific_attributes.u.dispersed_mine_area = attrDispersedMinedArea_;
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
