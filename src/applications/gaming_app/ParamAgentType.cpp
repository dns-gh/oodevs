// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAgentType.h"

// -----------------------------------------------------------------------------
// Name: ParamAgentType constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamAgentType::ParamAgentType( QWidget* parent, ASN1T_NatureAtlas*& asn, const QString& label )
    : QButtonGroup( 2, Qt::Horizontal, label, parent )
    , asn_        ( new ASN1T_NatureAtlas() )
{
    asn = asn_;
    pArmoredCheckBox_        = new QCheckBox( tr( "Armored" )         , this );
    pASSCheckBox_            = new QCheckBox( tr( "SSA" )             , this );
    pHQCheckBox_             = new QCheckBox( tr( "Command post" )    , this );
    pLogCheckBox_            = new QCheckBox( tr( "Logistic" )        , this );
    pMortarCheckBox_         = new QCheckBox( tr( "Mortar" )          , this );
    pRocketLauncherCheckBox_ = new QCheckBox( tr( "Rocket launcher" ) , this );
    pVehiculeCheckBox_       = new QCheckBox( tr( "Vehicle" )         , this );
    pFootTroopsCheckBox_     = new QCheckBox( tr( "Footman")          , this );
    pHelicopterCheckBox_     = new QCheckBox( tr( "Helicopter" )      , this );
    pUndefinedCheckBox_      = new QCheckBox( tr( "Undefined" )       , this );
    pNoneCheckBox_           = new QCheckBox( tr( "None" )            , this );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentType destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamAgentType::~ParamAgentType()
{
    delete asn_;
}

// -----------------------------------------------------------------------------
// Name: ParamAgentType::Commit
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ParamAgentType::Commit()
{
    asn_->numbits = 11;
    asn_->data[0] = 0;
    asn_->data[1] = 0;

    asn_->data[BytXblinde          ] |= ( pArmoredCheckBox_       ->isChecked() ? BitMblinde : 0           );
    asn_->data[BytXass             ] |= ( pASSCheckBox_           ->isChecked() ? BitMass : 0              );
    asn_->data[BytXpc              ] |= ( pHQCheckBox_            ->isChecked() ? BitMpc : 0               );
    asn_->data[BytXlog             ] |= ( pLogCheckBox_           ->isChecked() ? BitMlog : 0              );
    asn_->data[BytXmortier         ] |= ( pMortarCheckBox_        ->isChecked() ? BitMmortier : 0          );
    asn_->data[BytXlance_roquette  ] |= ( pRocketLauncherCheckBox_->isChecked() ? BitMlance_roquette : 0   );
    asn_->data[BytXvehicule        ] |= ( pVehiculeCheckBox_      ->isChecked() ? BitMvehicule : 0         );
    asn_->data[BytXpersonnel_a_pied] |= ( pFootTroopsCheckBox_    ->isChecked() ? BitMpersonnel_a_pied : 0 );
    asn_->data[BytXhelicoptere     ] |= ( pHelicopterCheckBox_    ->isChecked() ? BitMhelicoptere : 0      );
    asn_->data[BytXindefini        ] |= ( pUndefinedCheckBox_     ->isChecked() ? BitMindefini : 0         );
    asn_->data[BytXnone            ] |= ( pNoneCheckBox_          ->isChecked() ? BitMnone : 0             );
}
