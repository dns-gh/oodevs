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
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: RotaObjectAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
RotaObjectAttribute::RotaObjectAttribute( const Model& model, const ASN1T_ObjectAttributesSpecific& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , nDanger_           ( 0 )
    , nbcAgents_         ()
{
    if( asnMsg.t == T_ObjectAttributesSpecific_rota )
    {
        nDanger_ = asnMsg.u.rota->niveau_danger;
        nbcAgents_.clear();
        for( uint i = 0; i < asnMsg.u.rota->agents_nbc.n; ++i )
            nbcAgents_.push_back( asnMsg.u.rota->agents_nbc.elem[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: RotaObjectAttribute destructorw
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
RotaObjectAttribute::~RotaObjectAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: RotaObjectAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void RotaObjectAttribute::Update( const ASN1T_ObjectAttributesSpecific& asnMsg )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_rota )
    {
        nDanger_ = asnMsg.u.rota->niveau_danger;
        nbcAgents_.clear();
        for( uint i = 0; i < asnMsg.u.rota->agents_nbc.n; ++i )
            nbcAgents_.push_back( asnMsg.u.rota->agents_nbc.elem[ i ] );
    }

}

// -----------------------------------------------------------------------------
// Name: RotaObjectAttribute::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void RotaObjectAttribute::Send( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    asnMsg.t = nType_;
    asnMsg.u.rota = new ASN1T_ObjectAttributesRota();

    asnMsg.u.rota->niveau_danger = nDanger_;

    SendContainerValues< ASN1T_ListOID, ASN1T_OID, T_IDVector >( nbcAgents_, asnMsg.u.rota->agents_nbc );
}

// -----------------------------------------------------------------------------
// Name: RotaObjectAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void RotaObjectAttribute::AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    if( asnMsg.u.rota->agents_nbc.n > 0 )
        delete [] asnMsg.u.rota->agents_nbc.elem;
    delete asnMsg.u.rota;
}
