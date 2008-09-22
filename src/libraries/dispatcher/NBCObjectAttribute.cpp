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
NBCObjectAttribute::NBCObjectAttribute( const ASN1T_ObjectAttributesSpecific& asnMsg )
    : ObjectAttribute_ABC( asnMsg )
    , nNBCAgentType_     ( std::numeric_limits< unsigned int >::max() )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_nbc_cloud )
        nNBCAgentType_ = asnMsg.u.nbc_cloud->nbc_agent;       
    else if( asnMsg.t == T_ObjectAttributesSpecific_nbc_zone)
        nNBCAgentType_ = asnMsg.u.nbc_zone->nbc_agent;       
}

// -----------------------------------------------------------------------------
// Name: NBCObjectAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
NBCObjectAttribute::~NBCObjectAttribute()
{
    // NOTHING
}  

// -----------------------------------------------------------------------------
// Name: NBCObjectAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void NBCObjectAttribute::Update( const ASN1T_ObjectAttributesSpecific& asnMsg )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_nbc_cloud )
        nNBCAgentType_ = asnMsg.u.nbc_cloud->nbc_agent;       
    else if( asnMsg.t == T_ObjectAttributesSpecific_nbc_zone )
        nNBCAgentType_ = asnMsg.u.nbc_zone->nbc_agent;   
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
        case T_ObjectAttributesSpecific_nbc_cloud: 
            asnMsg.u.nbc_cloud = new ASN1T_ObjectAttributesNbcCloud();
            asnMsg.u.nbc_cloud->nbc_agent = nNBCAgentType_;
            break;
        case T_ObjectAttributesSpecific_nbc_zone: 
            asnMsg.u.nbc_zone = new ASN1T_ObjectAttributesNbcZone();
            asnMsg.u.nbc_zone->nbc_agent = nNBCAgentType_;
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
        case T_ObjectAttributesSpecific_nbc_cloud: 
            delete asnMsg.u.nbc_cloud;
            break;
        case T_ObjectAttributesSpecific_nbc_zone: 
            delete asnMsg.u.nbc_zone;
            break;
        default:
            throw std::runtime_error( "object specific attributes inconsistency" );
    } 
}
