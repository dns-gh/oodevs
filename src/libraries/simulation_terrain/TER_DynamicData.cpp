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
#include <spatialcontainer/TerrainData.h>
#include <boost/make_shared.hpp>

namespace
{

TerrainData Convert( const std::string& type )
{
    if( type.empty() )
        return TerrainData();
    const TerrainData result( type );
    if( result.IsRoad() )
        return result;
    return TerrainData::Bridge();
}

class DynamicData : public TER_DynamicData
{
public:
    DynamicData( const T_PointVector& points, const std::string& type )
        : points_     ( points )
        , terrainData_( Convert( type ))
    {
    }

    virtual ~DynamicData()
    {
    }

    virtual const T_PointVector& GetPoints() const
    {
        return points_;
    }

    virtual const TerrainData& GetData() const
    {
        return terrainData_;
    }

private:
    const T_PointVector points_;
    const TerrainData terrainData_;
};

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
    auto w = TER_World::GetWorldPtr();
    TER_PathFindManager* m = w ? &w->GetPathFindManager() : 0;
    auto p = boost::make_shared< DynamicData >( points, type );
    if( !m )
        return p;
    m->AddDynamicData( p );
    return DynamicDataPtr( p.get(), DynamicDataDeleter( m, p ));
}
