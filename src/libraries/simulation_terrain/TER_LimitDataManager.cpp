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

namespace
{

struct LimitDestructor
{
    LimitDestructor( TER_LimitDataManager* m ): manager_( m ) {}

    void operator()( TER_LimitData* p )
    {
        manager_->DeleteLimit( p );
    }

private:
    TER_LimitDataManager* manager_;
};

}  // namespace

TER_LimitDataManager::TER_LimitDataManager()
{
}

TER_LimitDataManager::~TER_LimitDataManager()
{
}

TER_LimitDataManager::T_LimitDataPtr TER_LimitDataManager::CreateLimit(
        const T_PointVector& points )
{
    T_LimitDataPtr p;
    auto it = limits_.find( points );
    if( it == limits_.end() )
    {
        p = boost::shared_ptr< TER_LimitData >(
            new TER_LimitData( points ), LimitDestructor( this ));
        limits_[ points ] = T_LimitDataWeakPtr( p );
    }
    else
    {
        p = it->second.lock();
        if( !p )
            throw std::logic_error( "TER_LimitData has been deallocated "
                "without being unregistered" );
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
