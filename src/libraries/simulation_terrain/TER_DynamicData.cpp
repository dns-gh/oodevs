// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_DynamicData.h"
#include "TER_PathFinderThread.h"
#include "TER_PathFindManager.h"
#include "TER_World.h"
#include "TER_AnalyzerManager.h"
#include "MT_Tools/MT_Logger.h"
#include <pathfind/TerrainRetractationHandle.h>
#include <memory>

namespace
{
    TerrainData Convert( const std::string& type )
    {
        if( type.empty() )
            return TER_AnalyzerManager::DefaultTerrainData();
        if( type == "highway" )
            return TerrainData::Motorway();
        if( type == "main road" )
            return TerrainData::LargeRoad();
        if( type == "secondary road" )
            return TerrainData::MediumRoad();
        if( type == "country road" )
            return TerrainData::SmallRoad();
        return TerrainData::Bridge();
    }
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData constructor
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
TER_DynamicData::TER_DynamicData( const T_PointVector& points )
    : nNbrRefs_   ( 0 )
    , points_     ( points )
    , terrainData_( Convert( "" ))
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData constructor
// Created: MCO 2012-11-29
// -----------------------------------------------------------------------------
TER_DynamicData::TER_DynamicData( const T_PointVector& points, const std::string& type )
    : nNbrRefs_   ( 0 )
    , points_     ( points )
    , terrainData_( Convert( type ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData destructor
// Created: AGE 2005-02-02
// -----------------------------------------------------------------------------
TER_DynamicData::~TER_DynamicData()
{
    assert( handles_.empty() );
    assert( nNbrRefs_ == 0 );
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData::AddForRegistration
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_DynamicData::AddForRegistration( TER_PathFinderThread& thread )
{
    boost::mutex::scoped_lock locker( mutex_ );

    ++ nNbrRefs_;
    thread.AddDynamicDataToRegister( *this );
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData::RegisterDynamicData
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_DynamicData::RegisterDynamicData( TER_PathFinderThread& thread,
        TerrainRetractationHandle& handle )
{
    boost::mutex::scoped_lock locker( mutex_ );

    if( ! handles_.insert( std::make_pair( &thread, &handle ) ).second )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
    assert( nNbrRefs_ > 0 );
    -- nNbrRefs_;
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData::AddForUnregistration
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_DynamicData::AddForUnregistration( TER_PathFinderThread& thread )
{
    boost::mutex::scoped_lock locker( mutex_ );

    ++ nNbrRefs_;
    thread.AddDynamicDataToUnregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData::UnregisterDynamicData
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_DynamicData::UnregisterDynamicData( TER_PathFinderThread& thread )
{
    bool bMustBeDeleted = false;
    {
        boost::mutex::scoped_lock locker( mutex_ );

        IT_HandleMap it = handles_.find( &thread );
        assert( it != handles_.end() );
        delete it->second;
        handles_.erase( it );

        assert( nNbrRefs_ > 0 );
        -- nNbrRefs_;

        bMustBeDeleted = ( nNbrRefs_ == 0 && handles_.empty() );
    }
    if( bMustBeDeleted )
        delete this;
}

const T_PointVector& TER_DynamicData::GetPoints() const
{
    return points_;
}

const TerrainData& TER_DynamicData::GetData() const
{
    return terrainData_;
}

namespace
{

struct DynamicDataDeleter
{
    DynamicDataDeleter( TER_PathFindManager* m ): manager_(m) {}

    void operator()( TER_DynamicData* p )
    {
        if( p && manager_ )
        {
            manager_->RemoveDynamicData( *p );
        }
    }

private:
    TER_PathFindManager* manager_;
};

}  // namespace

boost::shared_ptr< TER_DynamicData > CreateAndRegisterDynamicData(
        const T_PointVector& points, const std::string& type )
{
    // This is ugly but let us run tests creating TER_LimitData
    // without having to instanciante all the TER_World machinery.
    TER_World* w = &TER_World::GetWorld();
    TER_PathFindManager* m = w ? &w->GetPathFindManager() : 0;
    auto p = new TER_DynamicData( points, type );
    if( m )
        m->AddDynamicData( *p );
    return boost::shared_ptr< TER_DynamicData >( p, DynamicDataDeleter( m ));
}
