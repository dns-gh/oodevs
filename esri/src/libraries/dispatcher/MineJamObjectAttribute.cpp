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
MineJamObjectAttribute::MineJamObjectAttribute( const Model& model, const ASN1T_AttrObjectSpecific& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , nMinesActivityTime_( 0 )
{
    if( asnMsg.t == T_AttrObjectSpecific_bouchon_mines )
        nMinesActivityTime_ = asnMsg.u.bouchon_mines->delai_activite_mines;
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
void MineJamObjectAttribute::Update( const ASN1T_AttrObjectSpecific& asnMsg )
{
    if( asnMsg.t == T_AttrObjectSpecific_bouchon_mines )
        nMinesActivityTime_ = asnMsg.u.bouchon_mines->delai_activite_mines;
}

// -----------------------------------------------------------------------------
// Name: MineJamObjectAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void MineJamObjectAttribute::Send( ASN1T_AttrObjectSpecific& asnMsg ) const
{
    asnMsg.t = nType_;
    asnMsg.u.bouchon_mines = new ASN1T_AttrObjectBouchonMines();
    
    asnMsg.u.bouchon_mines->delai_activite_mines = nMinesActivityTime_;
}

// -----------------------------------------------------------------------------
// Name: MineJamObjectAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void MineJamObjectAttribute::AsnDelete( ASN1T_AttrObjectSpecific& asnMsg ) const
{
    delete asnMsg.u.bouchon_mines;
}
