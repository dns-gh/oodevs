// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "NBCAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: NBCAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
NBCAttribute::NBCAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )    
{
    nbc_.nbc_agents.n = 0;
    nbc_.nbc_agents.elem = 0;
    nbc_.danger_level = 0;
    Update( asnMsg );    
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
NBCAttribute::~NBCAttribute()
{
    Clear();
}  

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void NBCAttribute::Update( const ASN1T_ObjectAttributes& asnMsg )
{
    if ( asnMsg.m.nbcPresent )
    {
        Clear();
        nbc_.nbc_agents.n = asnMsg.nbc.nbc_agents.n;
        nbc_.nbc_agents.elem = new ASN1T_OID[ nbc_.nbc_agents.n ];        
        memcpy( nbc_.nbc_agents.elem, asnMsg.nbc.nbc_agents.elem, sizeof( ASN1T_OID ) * nbc_.nbc_agents.n );
        nbc_.danger_level = asnMsg.nbc.danger_level;
    }
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void NBCAttribute::Send( ASN1T_ObjectAttributes& asnMsg ) const
{
    asnMsg.m.nbcPresent = 1;
    asnMsg.nbc.nbc_agents.n = nbc_.nbc_agents.n;
    asnMsg.nbc.nbc_agents.elem = nbc_.nbc_agents.elem;
    asnMsg.nbc.danger_level = nbc_.danger_level;
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Clear
// Created: JCR 2008-08-27
// -----------------------------------------------------------------------------
void NBCAttribute::Clear()
{
    if ( nbc_.nbc_agents.n > 0 )
    {
        delete [] nbc_.nbc_agents.elem;
        nbc_.nbc_agents.n = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void NBCAttribute::AsnDelete( ASN1T_ObjectAttributes& /*asnMsg*/ ) const
{
    // NOTHING
}
