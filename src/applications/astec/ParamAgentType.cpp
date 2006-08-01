// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-07 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamAgentType.cpp $
// $Author: Ape $
// $Modtime: 7/05/04 12:05 $
// $Revision: 1 $
// $Workfile: ParamAgentType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamAgentType.h"

// -----------------------------------------------------------------------------
// Name: ParamAgentType constructor
// Created: APE 2004-05-07
// -----------------------------------------------------------------------------
ParamAgentType::ParamAgentType( ASN1T_NatureAtlas& asnNatureAtlas, const std::string& strLabel, QWidget* pParent, OptionalParamFunctor_ABC* pOptional )
    : QButtonGroup    ( 2, Qt::Horizontal, strLabel.c_str(), pParent )
    , Param_ABC   ( pOptional )
    , asnNatureAtlas_ ( asnNatureAtlas )
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
// Created: APE 2004-05-07
// -----------------------------------------------------------------------------
ParamAgentType::~ParamAgentType()
{
}


// -----------------------------------------------------------------------------
// Name: ParamAgentType::WriteMsg
/** @param  strMsg 
*/
// Created: APE 2004-05-07
// -----------------------------------------------------------------------------
void ParamAgentType::WriteMsg( std::stringstream& /*strMsg*/ )
{
    if( pOptional_ )
        pOptional_->SetOptionalPresent();

    asnNatureAtlas_.numbits = 11;
    asnNatureAtlas_.data[0] = 0;
    asnNatureAtlas_.data[1] = 0;

    asnNatureAtlas_.data[BytXblinde          ] |= ( pArmoredCheckBox_       ->isChecked() ? BitMblinde : 0             );
    asnNatureAtlas_.data[BytXass             ] |= ( pASSCheckBox_           ->isChecked() ? BitMass : 0                );
    asnNatureAtlas_.data[BytXpc              ] |= ( pHQCheckBox_            ->isChecked() ? BitMpc : 0                 );
    asnNatureAtlas_.data[BytXlog             ] |= ( pLogCheckBox_           ->isChecked() ? BitMlog : 0                );
    asnNatureAtlas_.data[BytXmortier         ] |= ( pMortarCheckBox_        ->isChecked() ? BitMmortier : 0            );
    asnNatureAtlas_.data[BytXlance_roquette  ] |= ( pRocketLauncherCheckBox_->isChecked() ? BitMlance_roquette : 0     );
    asnNatureAtlas_.data[BytXvehicule        ] |= ( pVehiculeCheckBox_      ->isChecked() ? BitMvehicule : 0           );
    asnNatureAtlas_.data[BytXpersonnel_a_pied] |= ( pFootTroopsCheckBox_    ->isChecked() ? BitMpersonnel_a_pied : 0   );
    asnNatureAtlas_.data[BytXhelicoptere     ] |= ( pHelicopterCheckBox_    ->isChecked() ? BitMhelicoptere : 0        );
    asnNatureAtlas_.data[BytXindefini        ] |= ( pUndefinedCheckBox_     ->isChecked() ? BitMindefini : 0           );
    asnNatureAtlas_.data[BytXnone            ] |= ( pNoneCheckBox_          ->isChecked() ? BitMnone : 0               );
}
