// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "MineJamObjectAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MineJamObjectAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
MineJamObjectAttribute::MineJamObjectAttribute( const Model& model, const ASN1T_ObjectAttributesSpecific& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , nMinesActivityTime_( 0 )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_mine_jam )
        nMinesActivityTime_ = asnMsg.u.mine_jam->activity_time;
}

// -----------------------------------------------------------------------------
// Name: MineJamObjectAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
MineJamObjectAttribute::~MineJamObjectAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: MineJamObjectAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void MineJamObjectAttribute::Update( const ASN1T_ObjectAttributesSpecific& asnMsg )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_mine_jam )
        nMinesActivityTime_ = asnMsg.u.mine_jam->activity_time;
}

// -----------------------------------------------------------------------------
// Name: MineJamObjectAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void MineJamObjectAttribute::Send( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    asnMsg.t = nType_;
    asnMsg.u.mine_jam = new ASN1T_ObjectAttributesMineJam();
    asnMsg.u.mine_jam->activity_time = nMinesActivityTime_;
}

// -----------------------------------------------------------------------------
// Name: MineJamObjectAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void MineJamObjectAttribute::AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    delete asnMsg.u.mine_jam;
}
