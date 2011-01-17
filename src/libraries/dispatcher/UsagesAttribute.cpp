// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "UsagesAttribute.h"
#include "protocol/Simulation.h"
#include <boost/foreach.hpp>
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: UsagesAttribute constructor
// Created: LGY 2011-01-17
// -----------------------------------------------------------------------------
UsagesAttribute::UsagesAttribute( const sword::UrbanAttributes& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: UsagesAttribute destructor
// Created: LGY 2011-01-17
// -----------------------------------------------------------------------------
UsagesAttribute::~UsagesAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UsagesAttribute::Update
// Created: LGY 2011-01-17
// -----------------------------------------------------------------------------
void UsagesAttribute::Update( const sword::UrbanAttributes& message )
{
    for( int i = 0; i < message.usages_size(); ++i )
    {
        const sword::UrbanUsage& usage = message.usages( i );
        motivations_[ usage.role() ] = usage.percentage();
    }
}

// -----------------------------------------------------------------------------
// Name: UsagesAttribute::Send
// Created: LGY 2011-01-17
// -----------------------------------------------------------------------------
void UsagesAttribute::Send( sword::UrbanAttributes& message ) const
{
    BOOST_FOREACH( const T_Motivations::value_type& motivation, motivations_ )
    {
        sword::UrbanUsage& usage = *message.add_usages();
        usage.set_role( motivation.first );
        usage.set_percentage( motivation.second );
    }
}
