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

BOOST_CLASS_EXPORT_GUID( BridgingCapacity, "BridgingCapacity" )

// -----------------------------------------------------------------------------
// Name: BridgingCapacity constructor
// Created: JCR 2008-08-21
// -----------------------------------------------------------------------------
BridgingCapacity::BridgingCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacityconstructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BridgingCapacity::BridgingCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacityconstructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BridgingCapacity::BridgingCapacity( const BridgingCapacity& /*from*/ )
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

// -----------------------------------------------------------------------------
// Name: BridgingCapacity::load
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
template< typename Archive >
void BridgingCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void BridgingCapacity::Register( Object& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void BridgingCapacity::Instanciate( Object& object ) const
{
    object.AddCapacity( new BridgingCapacity( *this ) );    
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacity::Finalize
// Created: JCR 2008-08-19
// -----------------------------------------------------------------------------
void BridgingCapacity::Finalize( Object& object )
{
    T_PointVector bridge; 
    CreateBridgeGeometry( object.GetLocalisation().GetPoints(), bridge );
    handler_.Reset( new TER_DynamicData( bridge, TerrainData::Bridge() ) );
}

// -----------------------------------------------------------------------------
// Name: BridgingCapacity::CreateBridgeGeometry
// Created: JCR 2008-08-19
// -----------------------------------------------------------------------------
void BridgingCapacity::CreateBridgeGeometry( const T_PointVector& points, T_PointVector& output ) const
{
    assert( !points.empty() );
    output.reserve( points.size() * 2 );
    if( points.size() == 1 )
    {
        output.push_back( points.front() );
        output.push_back( points.front() );
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
            output.push_back( *pLastPoint - direction );
            output.push_back( *pCurPoint  + direction );
            pLastPoint = pCurPoint;
        }
    }
}