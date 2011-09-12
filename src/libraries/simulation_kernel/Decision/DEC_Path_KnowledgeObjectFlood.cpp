// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Path_KnowledgeObjectFlood.h"
#include "Entities/Objects/FloodAttribute.h"
#include "MT_Tools/MT_Scipio_enum.h"

using namespace geometry;

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectFlood constructor
// Created: JSR 2010-12-20
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObjectFlood::DEC_Path_KnowledgeObjectFlood( E_CrossingHeight crossingHeight, const DEC_Knowledge_Object& knowledge )
    : crossingHeight_   ( crossingHeight )
    , maxTrafficability_( knowledge.GetMaxTrafficability() )
{
    const FloodAttribute* attribute = knowledge.RetrieveAttribute< FloodAttribute >();
    if( attribute )
    {
        localisation_ = attribute->GetLocalisation();

        // conversion to MT instead of geometry, more efficient. To optimize.
        const std::vector< Polygon2f* >& deepAreas = attribute->GetDeepAreas();
        const std::vector< Polygon2f* >& lowAreas = attribute->GetLowAreas();
        std::vector< Polygon2f* >::const_iterator it;
        T_PointVector vector;
        for( it = deepAreas.begin(); it != deepAreas.end(); ++it )
        {
            vector.clear();
            const Polygon2f::T_Vertices& vertices = ( *it )->Vertices();
            for( Polygon2f::CIT_Vertices vertexIt = vertices.begin(); vertexIt != vertices.end(); ++vertexIt )
                vector.push_back( MT_Vector2D( vertexIt->X(), vertexIt->Y() ) );
            deepAreas_.push_back( TER_Polygon() );
            deepAreas_.back().Reset( vector );
        }
        for( it = lowAreas.begin(); it != lowAreas.end(); ++it )
        {
            vector.clear();
            const Polygon2f::T_Vertices& vertices = ( *it )->Vertices();
            for( Polygon2f::CIT_Vertices vertexIt = vertices.begin(); vertexIt != vertices.end(); ++vertexIt )
                vector.push_back( MT_Vector2D( vertexIt->X(), vertexIt->Y() ) );
            lowAreas_.push_back( TER_Polygon() );
            lowAreas_.back().Reset( vector );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectFlood destructor
// Created: JSR 2010-12-20
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObjectFlood::~DEC_Path_KnowledgeObjectFlood()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectFlood::ComputeCost
// Created: JSR 2010-12-20
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObjectFlood::ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& /*nToTerrainType*/, const TerrainData& /*nLinkTerrainType*/, double weight ) const
{
    const MT_Line line( from, to );
    if( crossingHeight_ != eCrossingHeightAlways )
        if( localisation_.Intersect2D( line ) || localisation_.IsInside( to ) )
        {
            if( ( maxTrafficability_ != 0. ) && ( weight > maxTrafficability_ ) )
                return -1.f;
            std::vector< TER_Polygon >::const_iterator it;
            for( it = deepAreas_.begin(); it != deepAreas_.end(); ++it )
                if( it->Intersect2D( line, 0 ) )
                    return -1;
            if( crossingHeight_ == eCrossingHeightNever )
                for( it = lowAreas_.begin(); it != lowAreas_.end(); ++it )
                    if( it->Intersect2D( line, 0 ) )
                        return -1;
        }
    return std::numeric_limits< double >::min();
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectFlood::GetCostOut
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObjectFlood::GetCostOut() const
{
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectFlood::GetMaxTrafficability
// Created: CMA 2011-09-09
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObjectFlood::GetMaxTrafficability() const
{
    return maxTrafficability_;
}
