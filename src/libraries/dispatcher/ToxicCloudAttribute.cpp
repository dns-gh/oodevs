// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ToxicCloudAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ToxicCloudAttribute::ToxicCloudAttribute( const Common::ObjectAttributes& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ToxicCloudAttribute::~ToxicCloudAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::Update( const Common::ObjectAttributes& message )
{
    if( message.has_toxic_cloud() )
    {
        quantities_.Clear();
        for( int i = 0; i < message.toxic_cloud().quantities().elem_size(); ++i )
            *quantities_.add_elem() = message.toxic_cloud().quantities().elem( i );
    }
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::Send( Common::ObjectAttributes& message ) const
{
    for( int i = 0; i < quantities_.elem_size(); ++i )
        *message.mutable_toxic_cloud()->mutable_quantities()->add_elem() = quantities_.elem( i );
}
