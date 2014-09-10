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
        attribute->Apply( [&]( const TER_Polygon& polygon, bool deep ) 
            {
                if( deep )
                    deepAreas_.push_back( polygon );
                else
                    lowAreas_.push_back( polygon );
            });
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
        if( localisation_.IsInside( to ) || localisation_.Intersect2D( line ) )
        {
            if( ( maxTrafficability_ != 0. ) && ( weight > maxTrafficability_ ) )
                return -1.f;
            for( auto it = deepAreas_.begin(); it != deepAreas_.end(); ++it )
                if( it->Intersect2D( line, 0 ) )
                    return -1;
            if( crossingHeight_ == eCrossingHeightNever )
                for( auto it = lowAreas_.begin(); it != lowAreas_.end(); ++it )
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

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectFlood::GetAgentMaxSpeedMultiplier
// Created: JSR 2013-01-02
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObjectFlood::GetAgentMaxSpeedMultiplier() const
{
    return 1.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectFlood::HasAgentMaxSpeedMultiplier
// Created: JSR 2013-01-02
// -----------------------------------------------------------------------------
bool DEC_Path_KnowledgeObjectFlood::HasAgentMaxSpeedMultiplier() const
{
    return false;
}