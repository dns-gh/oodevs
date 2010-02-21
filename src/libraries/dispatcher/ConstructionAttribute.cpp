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
#include "protocol/SimulationSenders.h"

////using namespace Common;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( const Model& model, const Common::MsgObjectAttributes& asnMsg )
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
void ConstructionAttribute::Update( const Common::MsgObjectAttributes& asnMsg )
{    
    if( asnMsg.has_construction()  )
    {
        if ( asnMsg.construction().has_dotation_type() )
            dotation_ = asnMsg.construction().dotation_type();
        if ( asnMsg.construction().has_dotation_nbr() )
            nNbrDotationForConstruction_ = asnMsg.construction().dotation_nbr();
        if ( asnMsg.construction().has_percentage() )
            nPercentageConstruction_ = asnMsg.construction().percentage();    
    }
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ConstructionAttribute::Send( Common::MsgObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_construction()->set_dotation_type( dotation_ );
    asnMsg.mutable_construction()->set_dotation_nbr( nNbrDotationForConstruction_ );
    asnMsg.mutable_construction()->set_percentage( nPercentageConstruction_ );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Delete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ConstructionAttribute::Delete( Common::MsgObjectAttributes& /*asnMsg*/ ) const
{
//    delete asnMsg().mine_jam;
}
