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
#include "game_asn/Asn.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: ParamAgentType constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamAgentType::ParamAgentType( const QString& name )
    : Param_ABC( name )
    , pArmoredCheckBox_( 0 )
{
    // NOTHING
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
// Name: ParamAgentType::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamAgentType::BuildInterface( QWidget* parent )
{
    QButtonGroup* group      = new QButtonGroup( 2, Qt::Horizontal, GetName(), parent );
    pArmoredCheckBox_        = new QCheckBox( tools::translate( "AtlasNature", "Armored" )         , group );
    pASSCheckBox_            = new QCheckBox( tools::translate( "AtlasNature", "SSA" )             , group );
    pHQCheckBox_             = new QCheckBox( tools::translate( "AtlasNature", "Command post" )    , group );
    pLogCheckBox_            = new QCheckBox( tools::translate( "AtlasNature", "Logistic" )        , group );
    pMortarCheckBox_         = new QCheckBox( tools::translate( "AtlasNature", "Mortar" )          , group );
    pRocketLauncherCheckBox_ = new QCheckBox( tools::translate( "AtlasNature", "Rocket launcher" ) , group );
    pVehiculeCheckBox_       = new QCheckBox( tools::translate( "AtlasNature", "Vehicle" )         , group );
    pFootTroopsCheckBox_     = new QCheckBox( tools::translate( "AtlasNature", "Footman")          , group );
    pHelicopterCheckBox_     = new QCheckBox( tools::translate( "AtlasNature", "Helicopter" )      , group );
    pUndefinedCheckBox_      = new QCheckBox( tools::translate( "AtlasNature", "Undefined" )       , group );
    pNoneCheckBox_           = new QCheckBox( tools::translate( "AtlasNature", "None" )            , group );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentType::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAgentType::CommitTo( ASN1T_MissionParameter& asn ) const
{
    if( !pArmoredCheckBox_ )
        InterfaceNotInitialized();
    asn.null_value = 0;
    asn.value.t = T_MissionParameter_value_natureAtlas;
    ASN1T_NatureAtlas*& bits = asn.value.u.natureAtlas = new ASN1T_NatureAtlas();
    bits->numbits = 11;
    bits->data[0] = 0;
    bits->data[1] = 0;

    bits->data[BytXblinde          ] |= ( pArmoredCheckBox_       ->isChecked() ? BitMblinde : 0           );
    bits->data[BytXass             ] |= ( pASSCheckBox_           ->isChecked() ? BitMass : 0              );
    bits->data[BytXpc              ] |= ( pHQCheckBox_            ->isChecked() ? BitMpc : 0               );
    bits->data[BytXlog             ] |= ( pLogCheckBox_           ->isChecked() ? BitMlog : 0              );
    bits->data[BytXmortier         ] |= ( pMortarCheckBox_        ->isChecked() ? BitMmortier : 0          );
    bits->data[BytXlance_roquette  ] |= ( pRocketLauncherCheckBox_->isChecked() ? BitMlance_roquette : 0   );
    bits->data[BytXvehicule        ] |= ( pVehiculeCheckBox_      ->isChecked() ? BitMvehicule : 0         );
    bits->data[BytXpersonnel_a_pied] |= ( pFootTroopsCheckBox_    ->isChecked() ? BitMpersonnel_a_pied : 0 );
    bits->data[BytXhelicoptere     ] |= ( pHelicopterCheckBox_    ->isChecked() ? BitMhelicoptere : 0      );
    bits->data[BytXindefini        ] |= ( pUndefinedCheckBox_     ->isChecked() ? BitMindefini : 0         );
    bits->data[BytXnone            ] |= ( pNoneCheckBox_          ->isChecked() ? BitMnone : 0             );
}

// -----------------------------------------------------------------------------
// Name: ParamAgentType::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAgentType::Clean( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.natureAtlas;
}
