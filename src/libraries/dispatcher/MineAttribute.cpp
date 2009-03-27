// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "MineAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MineAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
MineAttribute::MineAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , dotation_ ( 0 )
    , nPercentageMining_     ( 0 )
    , nNbrDotationForMining_ ( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
MineAttribute::~MineAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: MineAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void MineAttribute::Update( const ASN1T_ObjectAttributes& asnMsg )
{
    if( asnMsg.m.minePresent )
    {
        if ( asnMsg.mine.m.dotation_typePresent )
            dotation_ = asnMsg.mine.dotation_type;
        if ( asnMsg.mine.m.dotation_nbrPresent )
            nNbrDotationForMining_ = asnMsg.mine.dotation_nbr;
        if ( asnMsg.mine.m.percentagePresent )
            nPercentageMining_ = asnMsg.mine.percentage;        
    }
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void MineAttribute::Send( ASN1T_ObjectAttributes& asnMsg ) const
{
    asnMsg.m.minePresent = 1;

    asnMsg.mine.m.dotation_typePresent = 1;
    asnMsg.mine.dotation_type = dotation_;

    asnMsg.mine.m.dotation_nbrPresent = 1;
    asnMsg.mine.dotation_nbr = nNbrDotationForMining_;

    asnMsg.mine.m.percentagePresent = 1;
    asnMsg.mine.percentage = nPercentageMining_;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void MineAttribute::AsnDelete( ASN1T_ObjectAttributes& /*asnMsg*/ ) const
{
//    delete asnMsg.u.mine_jam;
}
