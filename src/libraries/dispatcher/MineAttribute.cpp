// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "protocol/SimulationSenders.h"
#include "MineAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MineAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
MineAttribute::MineAttribute( const Model& model, const Common::MsgObjectAttributes& asnMsg )
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
void MineAttribute::Update( const Common::MsgObjectAttributes& asnMsg )
{
    if( asnMsg.has_mine()  )
    {
        if ( asnMsg.mine().has_dotation_type()  )
            dotation_ = asnMsg.mine().dotation_type();
        if ( asnMsg.mine().has_dotation_nbr()  )
            nNbrDotationForMining_ = asnMsg.mine().dotation_nbr();
        if ( asnMsg.mine().has_percentage()  )
            nPercentageMining_ = asnMsg.mine().percentage();        
    }
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void MineAttribute::Send( Common::MsgObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_mine()->set_dotation_type( dotation_ );
    asnMsg.mutable_mine()->set_dotation_nbr( nNbrDotationForMining_ );
    asnMsg.mutable_mine()->set_percentage( nPercentageMining_ );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Delete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void MineAttribute::Delete( Common::MsgObjectAttributes& /*asnMsg*/ ) const
{

}
