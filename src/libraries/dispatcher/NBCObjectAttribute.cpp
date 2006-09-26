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
NBCObjectAttribute::NBCObjectAttribute( const Model& model, const ASN1T_MsgObjectCreation& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , nNBCAgentType_     ( std::numeric_limits< unsigned int >::max() )
{
    if( !asnMsg.m.attributs_specifiquesPresent )
        return;

    if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_nuage_nbc )
        nNBCAgentType_ = asnMsg.attributs_specifiques.u.nuage_nbc->agent_nbc;       
    else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_zone_nbc )
        nNBCAgentType_ = asnMsg.attributs_specifiques.u.zone_nbc->agent_nbc;       
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
void NBCObjectAttribute::Update( const ASN1T_MsgObjectUpdate& asnMsg )
{
    if( !asnMsg.m.attributs_specifiquesPresent )
        return;

    if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_nuage_nbc )
        nNBCAgentType_ = asnMsg.attributs_specifiques.u.nuage_nbc->agent_nbc;       
    else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_zone_nbc )
        nNBCAgentType_ = asnMsg.attributs_specifiques.u.zone_nbc->agent_nbc;   
}
