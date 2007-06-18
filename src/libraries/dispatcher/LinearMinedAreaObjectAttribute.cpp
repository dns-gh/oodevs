// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "LinearMinedAreaObjectAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LinearMinedAreaObjectAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LinearMinedAreaObjectAttribute::LinearMinedAreaObjectAttribute( const Model& model, const ASN1T_ObjectAttributesSpecific& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , nMinesActivityTime_( 0 )
    , rMinesDensity_     ( 0. )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_zone_minee_lineaire )
    {
        nMinesActivityTime_ = asnMsg.u.zone_minee_lineaire->delai_activite_mines;
        rMinesDensity_      = asnMsg.u.zone_minee_lineaire->densite;
    }
}

// -----------------------------------------------------------------------------
// Name: LinearMinedAreaObjectAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LinearMinedAreaObjectAttribute::~LinearMinedAreaObjectAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: LinearMinedAreaObjectAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LinearMinedAreaObjectAttribute::Update( const ASN1T_ObjectAttributesSpecific& asnMsg )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_zone_minee_lineaire )
    {
        nMinesActivityTime_ = asnMsg.u.zone_minee_lineaire->delai_activite_mines;
        rMinesDensity_      = asnMsg.u.zone_minee_lineaire->densite;
    }
}

// -----------------------------------------------------------------------------
// Name: LinearMinedAreaObjectAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LinearMinedAreaObjectAttribute::Send( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    asnMsg.t = nType_;
    asnMsg.u.zone_minee_lineaire = new ASN1T_ObjectAttributesLinearMineArea();
    
    asnMsg.u.zone_minee_lineaire->delai_activite_mines = nMinesActivityTime_;
    asnMsg.u.zone_minee_lineaire->densite              = rMinesDensity_;
}

// -----------------------------------------------------------------------------
// Name: LinearMinedAreaObjectAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LinearMinedAreaObjectAttribute::AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    delete asnMsg.u.zone_minee_lineaire;
}
