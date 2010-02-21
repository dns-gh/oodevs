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
#include "ToxicCloudAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ToxicCloudAttribute::ToxicCloudAttribute( const Model& model, const Common::MsgObjectAttributes& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
{
    quantities_.clear_elem();
    Update( asnMsg );    
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ToxicCloudAttribute::~ToxicCloudAttribute()
{
    Clear();   
}  

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::Update( const Common::MsgObjectAttributes& asnMsg )
{
    if( asnMsg.has_toxic_cloud() )
    {         
        Clear();
        for( int i = 0; i < quantities_.elem_size(); ++i )
            *quantities_.add_elem() = asnMsg.toxic_cloud().quantities().elem( i );
    }
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::Clear
// Created: JCR 2008-06-13
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::Clear()
{
    if ( quantities_.elem_size() > 0 )
    {
        quantities_.Clear();
    }
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::Send( Common::MsgObjectAttributes& asnMsg ) const
{
    for( int i = 0; i < quantities_.elem_size(); ++i )
        *asnMsg.mutable_toxic_cloud()->mutable_quantities()->add_elem() = quantities_.elem( i );
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::Delete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::Delete( Common::MsgObjectAttributes& /*asnMsg*/ ) const
{
//    delete asnMsg().mine_jam;
}
