// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ParamAgentType.h"

// -----------------------------------------------------------------------------
// Name: ParamAgentType constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamAgentType::ParamAgentType( QWidget* parent, ASN1T_NatureAtlas& asn, const std::string& label )
    : QButtonGroup( 2, Qt::Horizontal, label.c_str(), parent )
    , asn_        ( asn )
{
    pArmoredCheckBox_        = new QCheckBox( tr( "Blindé" )         , this );
    pASSCheckBox_            = new QCheckBox( tr( "ASS" )            , this );
    pHQCheckBox_             = new QCheckBox( tr( "PC" )             , this );
    pLogCheckBox_            = new QCheckBox( tr( "Log" )            , this );
    pMortarCheckBox_         = new QCheckBox( tr( "Mortier" )        , this );
    pRocketLauncherCheckBox_ = new QCheckBox( tr( "Lance roquette" ) , this );
    pVehiculeCheckBox_       = new QCheckBox( tr( "Vehicule" )       , this );
    pFootTroopsCheckBox_     = new QCheckBox( tr( "Personnal à pied"), this );
    pHelicopterCheckBox_     = new QCheckBox( tr( "Helicoptère" )    , this );
    pUndefinedCheckBox_      = new QCheckBox( tr( "Indéfini" )       , this );
    pNoneCheckBox_           = new QCheckBox( tr( "Aucun" )          , this );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentType destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamAgentType::~ParamAgentType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgentType::Commit
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ParamAgentType::Commit()
{
    asn_.numbits = 11;
    asn_.data[0] = 0;
    asn_.data[1] = 0;

    asn_.data[BytXblinde          ] |= ( pArmoredCheckBox_       ->isChecked() ? BitMblinde : 0           );
    asn_.data[BytXass             ] |= ( pASSCheckBox_           ->isChecked() ? BitMass : 0              );
    asn_.data[BytXpc              ] |= ( pHQCheckBox_            ->isChecked() ? BitMpc : 0               );
    asn_.data[BytXlog             ] |= ( pLogCheckBox_           ->isChecked() ? BitMlog : 0              );
    asn_.data[BytXmortier         ] |= ( pMortarCheckBox_        ->isChecked() ? BitMmortier : 0          );
    asn_.data[BytXlance_roquette  ] |= ( pRocketLauncherCheckBox_->isChecked() ? BitMlance_roquette : 0   );
    asn_.data[BytXvehicule        ] |= ( pVehiculeCheckBox_      ->isChecked() ? BitMvehicule : 0         );
    asn_.data[BytXpersonnel_a_pied] |= ( pFootTroopsCheckBox_    ->isChecked() ? BitMpersonnel_a_pied : 0 );
    asn_.data[BytXhelicoptere     ] |= ( pHelicopterCheckBox_    ->isChecked() ? BitMhelicoptere : 0      );
    asn_.data[BytXindefini        ] |= ( pUndefinedCheckBox_     ->isChecked() ? BitMindefini : 0         );
    asn_.data[BytXnone            ] |= ( pNoneCheckBox_          ->isChecked() ? BitMnone : 0             );
}
