// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "ConstructionAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , dotation_ ( 0 )
    , nNbrDotationForConstruction_  ( 0 )
    , nPercentageConstruction_      ( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ConstructionAttribute::~ConstructionAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void ConstructionAttribute::Update( const ASN1T_ObjectAttributes& asnMsg )
{    
    if( asnMsg.m.constructionPresent )
    {
        if ( asnMsg.construction.m.dotation_typePresent )
            dotation_ = asnMsg.construction.dotation_type;
        if ( asnMsg.construction.m.dotation_nbrPresent )
            nNbrDotationForConstruction_ = asnMsg.construction.dotation_nbr;
        if ( asnMsg.construction.m.percentagePresent )
            nPercentageConstruction_ = asnMsg.construction.percentage;    
    }
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ConstructionAttribute::Send( ASN1T_ObjectAttributes& asnMsg ) const
{
    asnMsg.m.constructionPresent = 1;
    asnMsg.construction.m.dotation_typePresent = 1;
    asnMsg.construction.dotation_type = dotation_;

    asnMsg.construction.m.dotation_nbrPresent = 1;
    asnMsg.construction.dotation_nbr = nNbrDotationForConstruction_;

    asnMsg.construction.m.percentagePresent = 1;
    asnMsg.construction.percentage = nPercentageConstruction_;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ConstructionAttribute::AsnDelete( ASN1T_ObjectAttributes& /*asnMsg*/ ) const
{
//    delete asnMsg.u.mine_jam;
}
