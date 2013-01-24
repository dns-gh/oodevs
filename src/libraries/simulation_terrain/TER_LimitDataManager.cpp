// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_LimitDataManager.h"
#include "TER_LimitData.h"
#include <boost/bind.hpp>

boost::shared_ptr< TER_LimitData > TER_LimitDataManager::CreateLimit( const T_PointVector& points )
{
    boost::weak_ptr< TER_LimitData >& w = limits_[ points ];
    boost::shared_ptr< TER_LimitData > p = w.lock();
    if( ! p )
    {
        p.reset( new TER_LimitData( points ), boost::bind( &TER_LimitDataManager::DeleteLimit, this, _1 ) );
        w = p;
    }
    return p;
}

void TER_LimitDataManager::DeleteLimit( const TER_LimitData* p )
{
    limits_.erase( p->GetPoints() );
    delete p;
}

bool TER_LimitDataManager::HasLimit( const T_PointVector& points ) const
{
    return limits_.find( points ) != limits_.end();
}
