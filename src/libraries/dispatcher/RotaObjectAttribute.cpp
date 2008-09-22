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
#include "ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: RotaObjectAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
RotaObjectAttribute::RotaObjectAttribute( const ASN1T_ObjectAttributesSpecific& asnMsg )
    : ObjectAttribute_ABC( asnMsg )
    , nDanger_           ( 0 )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_rota )
    {
        nDanger_ = asnMsg.u.rota->danger_level;
        nbcAgents_.clear();
        for( unsigned int i = 0; i < asnMsg.u.rota->nbc_agents.n; ++i )
            nbcAgents_.push_back( asnMsg.u.rota->nbc_agents.elem[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: RotaObjectAttribute destructorw
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
RotaObjectAttribute::~RotaObjectAttribute()
{
    // NOTHING
}  

// -----------------------------------------------------------------------------
// Name: RotaObjectAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void RotaObjectAttribute::Update( const ASN1T_ObjectAttributesSpecific& asnMsg )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_rota )
    {
        nDanger_ = asnMsg.u.rota->danger_level;
        nbcAgents_.clear();
        for( unsigned int i = 0; i < asnMsg.u.rota->nbc_agents.n; ++i )
            nbcAgents_.push_back( asnMsg.u.rota->nbc_agents.elem[ i ] );
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
    asnMsg.u.rota->danger_level = nDanger_;
    {
        asnMsg.u.rota->nbc_agents.n = nbcAgents_.size();
        asnMsg.u.rota->nbc_agents.elem = asnMsg.u.rota->nbc_agents.n > 0 ? new ASN1T_OID[ asnMsg.u.rota->nbc_agents.n ] : 0;
        unsigned int i = 0;
        for( std::vector< unsigned int >::const_iterator it = nbcAgents_.begin(); it != nbcAgents_.end(); ++it, ++i )
            asnMsg.u.rota->nbc_agents.elem[i] = *it;
    }
}

// -----------------------------------------------------------------------------
// Name: RotaObjectAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void RotaObjectAttribute::AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    if( asnMsg.u.rota->nbc_agents.n > 0 )
        delete [] asnMsg.u.rota->nbc_agents.elem;
    delete asnMsg.u.rota;
}
