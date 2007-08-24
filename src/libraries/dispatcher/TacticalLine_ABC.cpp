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
#include "tools/App6Symbol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: SBO 2006-11-15
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( const Model& model, unsigned int id, const ASN1T_TacticalLine& asn )
    : model_    ( model )
    , id_       ( id )
    , strName_  ( asn.name )
    , location_ ( asn.geometry )
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
    strName_ = asn.name;
    location_.Update( asn.geometry );
    UpdateDiffusion( asn.diffusion );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::UpdateDiffusion
// Created: SBO 2006-11-15
// -----------------------------------------------------------------------------
void TacticalLine_ABC::UpdateDiffusion( const ASN1T_TacticalLinesDiffusion& diffusion )
{
    if( diffusion.t == T_TacticalLinesDiffusion_automat )
        automat_ = &model_.GetAutomats().Get( diffusion.u.automat );
    else
        formation_ = &model_.GetFormations().Get( diffusion.u.formation );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Send
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Send( ASN1T_TacticalLine& asn ) const
{
    location_.Send( asn.geometry );
    asn.name = strName_.c_str();
    if( formation_ )
    {
        asn.diffusion.t           = T_TacticalLinesDiffusion_formation;
        asn.diffusion.u.formation = formation_->GetID();
    }
    else if( automat_ )
    {
        asn.diffusion.t          = T_TacticalLinesDiffusion_automat;
        asn.diffusion.u.automat = automat_->GetID();
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::Send
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
void TacticalLine_ABC::Send( ASN1T_Location& asn ) const
{
    location_.Send( asn );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetID
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
unsigned int TacticalLine_ABC::GetID() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::BuildSymbol
// Created: SBO 2007-08-22
// -----------------------------------------------------------------------------
std::string TacticalLine_ABC::BuildSymbol( bool up /*= true*/ ) const
{
    std::string symbol( "G*GPGL-----*--X" ); // $$$$ SBO 2007-08-23: could be built using tool::app6 operations or symbol.xml
    if( up )
    {
        if( automat_ )
            return tools::app6::MergeSymbol( symbol, automat_->BuildSymbol() );
        if( formation_ )
            return tools::app6::MergeSymbol( symbol, formation_->BuildSymbol() );
    }
    return symbol;
}
