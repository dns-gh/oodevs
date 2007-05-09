// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "TacticalLine_ABC.h"
#include "Model.h"
#include "Formation.h"
#include "Automat.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: SBO 2006-11-15
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( const Model& model, unsigned int id, const ASN1T_TacticalLine& asn )
    : model_    ( model )
    , id_       ( id )
    , strName_  ( asn.nom )
    , location_ ( asn.geometrie )
    , automat_  ( 0 )
    , formation_( 0 )
{
    UpdateDiffusion( asn.diffusion );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC destructor
// Created: SBO 2006-11-15
// -----------------------------------------------------------------------------
TacticalLine_ABC::~TacticalLine_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Update
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Update( const ASN1T_TacticalLine& asn )
{
    strName_ = asn.nom;
    location_.Update( asn.geometrie );
    UpdateDiffusion( asn.diffusion );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::UpdateDiffusion
// Created: SBO 2006-11-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::UpdateDiffusion( const ASN1T_TacticalLinesDiffusion& diffusion )
{
    if( diffusion.t == T_TacticalLinesDiffusion_automate )
        automat_ = &model_.GetAutomats().Get( diffusion.u.automate );
    else
        formation_ = &model_.GetFormations().Get( diffusion.u.formation );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Send
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Send( ASN1T_TacticalLine& asn ) const
{
    location_.Send( asn.geometrie );
    asn.nom = strName_.c_str();
    if( formation_ )
    {
        asn.diffusion.t           = T_TacticalLinesDiffusion_formation;
        asn.diffusion.u.formation = formation_->GetID();
    }
    else if( automat_ )
    {
        asn.diffusion.t          = T_TacticalLinesDiffusion_automate;
        asn.diffusion.u.automate = automat_->GetID();
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetID
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
unsigned int TacticalLine_ABC::GetID() const
{
    return id_;
}
