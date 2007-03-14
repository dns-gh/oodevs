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
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: ParamAgentType constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamAgentType::ParamAgentType( ASN1T_NatureAtlas*& asn, const QString& name )
    : Param_ABC( name )
    , asn_( new ASN1T_NatureAtlas() )
    , pArmoredCheckBox_( 0 )
{
    asn = asn_;
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
// Name: ParamAgentType::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamAgentType::BuildInterface( QWidget* parent )
{
    QButtonGroup* group      = new QButtonGroup( 2, Qt::Horizontal, GetName(), parent );
    pArmoredCheckBox_        = new QCheckBox( group->tr( "Armored" )         , group );
    pASSCheckBox_            = new QCheckBox( group->tr( "SSA" )             , group );
    pHQCheckBox_             = new QCheckBox( group->tr( "Command post" )    , group );
    pLogCheckBox_            = new QCheckBox( group->tr( "Logistic" )        , group );
    pMortarCheckBox_         = new QCheckBox( group->tr( "Mortar" )          , group );
    pRocketLauncherCheckBox_ = new QCheckBox( group->tr( "Rocket launcher" ) , group );
    pVehiculeCheckBox_       = new QCheckBox( group->tr( "Vehicle" )         , group );
    pFootTroopsCheckBox_     = new QCheckBox( group->tr( "Footman")          , group );
    pHelicopterCheckBox_     = new QCheckBox( group->tr( "Helicopter" )      , group );
    pUndefinedCheckBox_      = new QCheckBox( group->tr( "Undefined" )       , group );
    pNoneCheckBox_           = new QCheckBox( group->tr( "None" )            , group );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentType::Commit
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ParamAgentType::Commit()
{
    if( !pArmoredCheckBox_ )
        InterfaceNotInitialized();

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
