// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "FireAttribute.h"
#include "protocol/SimulationSenders.h"

////using namespace Common;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: FireAttribute constructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute( const Model& model, const Common::MsgObjectAttributes& asnMsg )
    : ObjectAttribute_ABC   ( model, asnMsg )
    , heat_                 ( asnMsg.fire().heat() )
    , nFireClass_           ( asnMsg.fire().class_id() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireAttribute destructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
FireAttribute::~FireAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: FireAttribute::Update
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
void FireAttribute::Update( const Common::MsgObjectAttributes& asnMsg )
{
    if ( asnMsg.has_fire() )
    {
        heat_       = asnMsg.fire().heat();
        nFireClass_ = asnMsg.fire().class_id();
    }  
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::Send
// Created: RFT 2006-09-27
// -----------------------------------------------------------------------------
void FireAttribute::Send( Common::MsgObjectAttributes& asnMsg ) const
{

    asnMsg.mutable_fire()->set_heat( heat_ );
    asnMsg.mutable_fire()->set_class_id( nFireClass_ );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::Delete
// Created: RFT 2006-09-28
// -----------------------------------------------------------------------------
void FireAttribute::Delete( Common::MsgObjectAttributes& /*asnMsg*/ ) const
{
    // NOTHING
}
