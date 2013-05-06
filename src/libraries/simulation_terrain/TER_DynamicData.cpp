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
#include "TER_PathFindManager.h"
#include "TER_World.h"
#include "TER_AnalyzerManager.h"
#include <boost/make_shared.hpp>

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
    : points_     ( points )
    , terrainData_( Convert( "" ))
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_DynamicData constructor
// Created: MCO 2012-11-29
// -----------------------------------------------------------------------------
TER_DynamicData::TER_DynamicData( const T_PointVector& points, const std::string& type )
    : points_     ( points )
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
    DynamicDataDeleter( TER_PathFindManager* m, const DynamicDataPtr& data )
        : manager_( m )
        , data_( data )
    {}

    void operator()( TER_DynamicData* p )
    {
        if( p && manager_ && data_ )
        {
            if( p != data_.get() )
                throw std::logic_error( "original and dynamic data to destroy mismatch" );
            manager_->RemoveDynamicData( data_ );
            data_.reset();
        }
    }

private:
    TER_PathFindManager* manager_;
    DynamicDataPtr data_;
};

}  // namespace

DynamicDataPtr CreateAndRegisterDynamicData( const T_PointVector& points,
        const std::string& type )
{
    // This is ugly but let us run tests creating TER_LimitData
    // without having to instanciante all the TER_World machinery.
    TER_World* w = &TER_World::GetWorld();
    TER_PathFindManager* m = w ? &w->GetPathFindManager() : 0;
    auto p = boost::make_shared< TER_DynamicData >( points, type );
    if( m )
        m->AddDynamicData( p );
    return DynamicDataPtr( p.get(), DynamicDataDeleter( m, p ));
}
