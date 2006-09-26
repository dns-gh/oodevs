// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "RotaObjectAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: RotaObjectAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
RotaObjectAttribute::RotaObjectAttribute( const Model& model, const ASN1T_MsgObjectCreation& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , nDanger_           ( 0 )
    , nbcAgents_         ()
{
    if( !asnMsg.m.attributs_specifiquesPresent )
        return;

    if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_rota )
    {
        nDanger_ = asnMsg.attributs_specifiques.u.rota->niveau_danger;
        for( uint i = 0; i < asnMsg.attributs_specifiques.u.rota->agents_nbc.n; ++i )
            nbcAgents_.push_back( asnMsg.attributs_specifiques.u.rota->agents_nbc.elem[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: RotaObjectAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
RotaObjectAttribute::~RotaObjectAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: RotaObjectAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void RotaObjectAttribute::Update( const ASN1T_MsgObjectUpdate& asnMsg )
{
    if( !asnMsg.m.attributs_specifiquesPresent )
        return;

    if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_rota )
    {
        nDanger_ = asnMsg.attributs_specifiques.u.rota->niveau_danger;
        nbcAgents_.clear();
        for( uint i = 0; i < asnMsg.attributs_specifiques.u.rota->agents_nbc.n; ++i )
            nbcAgents_.push_back( asnMsg.attributs_specifiques.u.rota->agents_nbc.elem[ i ] );
    }
}
