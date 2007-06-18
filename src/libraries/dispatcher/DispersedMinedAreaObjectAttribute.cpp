// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "DispersedMinedAreaObjectAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: DispersedMinedAreaObjectAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
DispersedMinedAreaObjectAttribute::DispersedMinedAreaObjectAttribute( const Model& model, const ASN1T_ObjectAttributesSpecific& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , nMinesActivityTime_( 0 )
    , rMinesDensity_     ( 0. )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_zone_minee_par_dispersion )
    {
        nMinesActivityTime_ = asnMsg.u.zone_minee_par_dispersion->delai_activite_mines;
        rMinesDensity_      = asnMsg.u.zone_minee_par_dispersion->densite;
    }
}

// -----------------------------------------------------------------------------
// Name: DispersedMinedAreaObjectAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
DispersedMinedAreaObjectAttribute::~DispersedMinedAreaObjectAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: DispersedMinedAreaObjectAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void DispersedMinedAreaObjectAttribute::Update( const ASN1T_ObjectAttributesSpecific& asnMsg )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_zone_minee_par_dispersion )
    {
        nMinesActivityTime_ = asnMsg.u.zone_minee_par_dispersion->delai_activite_mines;
        rMinesDensity_      = asnMsg.u.zone_minee_par_dispersion->densite;
    }
}

// -----------------------------------------------------------------------------
// Name: DispersedMinedAreaObjectAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void DispersedMinedAreaObjectAttribute::Send( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    asnMsg.t = nType_;
    asnMsg.u.zone_minee_par_dispersion = new ASN1T_ObjectAttributesDispersedMineArea();
    
    asnMsg.u.zone_minee_par_dispersion->delai_activite_mines = nMinesActivityTime_;
    asnMsg.u.zone_minee_par_dispersion->densite              = rMinesDensity_;
}

// -----------------------------------------------------------------------------
// Name: DispersedMinedAreaObjectAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void DispersedMinedAreaObjectAttribute::AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    delete asnMsg.u.zone_minee_par_dispersion;
}
