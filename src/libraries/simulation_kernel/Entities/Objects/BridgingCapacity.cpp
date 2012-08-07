// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "BridgingCapacity.h"
#include "Object.h"
#include "TimeLimitedAttribute.h"
#include "simulation_terrain/TER_DynamicData.h"
#include "simulation_terrain/TER_Localisation.h"
#include "simulation_terrain/TER_PathFindManager.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( BridgingCapacity )

// -----------------------------------------------------------------------------
// Name: BridgingCapacity constructor
// Created: JCR 2008-08-21
// -----------------------------------------------------------------------------
BridgingCapacity::BridgingCapacity( xml::xistream& xis )
    : type_( xis.attribute< std::string >( "type" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacityconstructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BridgingCapacity::BridgingCapacity()
    : type_( "bridge" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacityconstructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BridgingCapacity::BridgingCapacity( const BridgingCapacity& from )
    : type_( from.type_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacitydestructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BridgingCapacity::~BridgingCapacity()
{
    // NOTHING
}

namespace
{
    bool IsBridge( const std::string& type ) { return type == "bridge"; }

    TerrainData Convert( const std::string& type )
    {
        if( type == "highway" )             return TerrainData::Motorway();
        else if( type == "main road" )      return TerrainData::LargeRoad();
        else if( type == "secondary road" ) return TerrainData::MediumRoad();
        else if( type == "country road" )   return TerrainData::SmallRoad();
        else if( IsBridge( type ) )         return TerrainData::Bridge();
        return TerrainData::Bridge();
    }
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacity::load
// Created: JSR 2012-04-16
// -----------------------------------------------------------------------------
void BridgingCapacity::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    bool isPathData = true;
    file >> boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         >> type_
         >> bridge_
         >> isPathData;
    if( isPathData )
        CreatePathData();
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacity::save
// Created: JSR 2012-04-16
// -----------------------------------------------------------------------------
void BridgingCapacity::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    bool isPathData = IsPathData();
    file << boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         << type_
         << bridge_
         << isPathData;
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void BridgingCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void BridgingCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new BridgingCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacity::Finalize
// Created: JCR 2008-08-19
// -----------------------------------------------------------------------------
void BridgingCapacity::Finalize( MIL_Object_ABC& object )
{
    CreateBridgeGeometry( object.GetLocalisation().GetPoints() );
    if( type_ != "" && !IsBridge( type_ ) )
        CreatePathData();
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacity::CreatePathData
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void BridgingCapacity::CreatePathData()
{
    handler_.Reset( new TER_DynamicData( bridge_, Convert( type_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacity::IsBridgeType
// Created: MMC 2012-08-01
// -----------------------------------------------------------------------------
bool BridgingCapacity::IsBridgeType() const
{
    return IsBridge( type_ );
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacity::IsPathData
// Created: MMC 2012-08-01
// -----------------------------------------------------------------------------
bool BridgingCapacity::IsPathData() const
{
    return handler_.IsPathfindData();
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacity::CreateBridgeGeometry
// Created: JCR 2008-08-19
// -----------------------------------------------------------------------------
void BridgingCapacity::CreateBridgeGeometry( const T_PointVector& points )
{
    assert( !points.empty() );
    bridge_.reserve( points.size() * 2 );
    if( points.size() == 1 )
    {
        bridge_.push_back( points.front() );
        bridge_.push_back( points.front() );
    }
    else
    {
        CIT_PointVector itPoint = points.begin();
        const MT_Vector2D* pLastPoint = &*itPoint;
        for( ++itPoint; itPoint != points.end(); ++itPoint )
        {
            const MT_Vector2D* pCurPoint = &*itPoint;
            MT_Vector2D direction( *pCurPoint - *pLastPoint );
            direction.Normalize() *= 150;
            bridge_.push_back( *pLastPoint - direction );
            bridge_.push_back( *pCurPoint  + direction );
            pLastPoint = pCurPoint;
        }
    }
}
