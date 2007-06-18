// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "NBCObjectAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: NBCObjectAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
NBCObjectAttribute::NBCObjectAttribute( const Model& model, const ASN1T_ObjectAttributesSpecific& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , nNBCAgentType_     ( std::numeric_limits< unsigned int >::max() )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_nuage_nbc )
        nNBCAgentType_ = asnMsg.u.nuage_nbc->agent_nbc;       
    else if( asnMsg.t == T_ObjectAttributesSpecific_zone_nbc )
        nNBCAgentType_ = asnMsg.u.zone_nbc->agent_nbc;       
}

// -----------------------------------------------------------------------------
// Name: NBCObjectAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
NBCObjectAttribute::~NBCObjectAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: NBCObjectAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void NBCObjectAttribute::Update( const ASN1T_ObjectAttributesSpecific& asnMsg )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_nuage_nbc )
        nNBCAgentType_ = asnMsg.u.nuage_nbc->agent_nbc;       
    else if( asnMsg.t == T_ObjectAttributesSpecific_zone_nbc )
        nNBCAgentType_ = asnMsg.u.zone_nbc->agent_nbc;   
}

// -----------------------------------------------------------------------------
// Name: NBCObjectAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void NBCObjectAttribute::Send( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    asnMsg.t = nType_;
    switch( nType_ )
    {
        case T_ObjectAttributesSpecific_nuage_nbc: 
            asnMsg.u.nuage_nbc = new ASN1T_ObjectAttributesNbcCloud();
            asnMsg.u.nuage_nbc->agent_nbc = nNBCAgentType_;
            break;
        case T_ObjectAttributesSpecific_zone_nbc: 
            asnMsg.u.zone_nbc = new ASN1T_ObjectAttributesNbcZone();
            asnMsg.u.zone_nbc->agent_nbc = nNBCAgentType_;
            break;
        default:
            throw std::runtime_error( "object specific attributes inconsistency" );
    }
}

// -----------------------------------------------------------------------------
// Name: NBCObjectAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void NBCObjectAttribute::AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    switch( nType_ )
    {
        case T_ObjectAttributesSpecific_nuage_nbc: 
            delete asnMsg.u.nuage_nbc;
            break;
        case T_ObjectAttributesSpecific_zone_nbc: 
            delete asnMsg.u.zone_nbc;
            break;
        default:
            throw std::runtime_error( "object specific attributes inconsistency" );
    } 
}
