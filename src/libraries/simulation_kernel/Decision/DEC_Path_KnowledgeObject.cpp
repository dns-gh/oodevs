// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Path_KnowledgeObject.h"
#include "DEC_Agent_PathClass.h"
#include "DEC_Population_PathClass.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "entities/objects/MIL_Object_ABC.h"

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject constructor
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObject::DEC_Path_KnowledgeObject( const DEC_Agent_PathClass& pathClass, const DEC_Knowledge_Object& knowledge )
    : localisation_         ( knowledge.GetLocalisation() )
    , scaledLocalisation_   ( localisation_ )
    , realLocalisation_     ( knowledge.GetObjectKnown() ? knowledge.GetObjectKnown()->GetLocalisation() : localisation_ )
    , rCostIn_              ( 0 )
    , rCostOut_             ( 0 )
    , rObstructionThreshold_( pathClass.GetThreshold() )
    , rMaxTrafficability_   ( knowledge.GetMaxTrafficability() )
{
    const double rCost = pathClass.GetObjectCost( knowledge.GetType() );
    if( rCost > 0 )
        rCostIn_ = rCost;
    else
        rCostOut_ = -rCost;
    if( rCost != 0 )
        scaledLocalisation_.Scale( 100 ); // $$$ LDC arbitrary 100m precision (useful for making path very close to obstacle expensive)
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject constructor
// Created: CMA 2011-11-24
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObject::DEC_Path_KnowledgeObject( const DEC_Population_PathClass& pathClass, const DEC_Knowledge_Object& knowledge )
    : localisation_         ( knowledge.GetLocalisation() )
    , scaledLocalisation_   ( localisation_ )
    , realLocalisation_     ( knowledge.GetObjectKnown() ? knowledge.GetObjectKnown()->GetLocalisation() : localisation_ )
    , rCostIn_              ( 0 )
    , rCostOut_             ( 0 )
    , rObstructionThreshold_( pathClass.GetThreshold() )
    , rMaxTrafficability_   ( knowledge.GetMaxTrafficability() )
{
    const double rCost = pathClass.GetObjectCost( knowledge.GetType() );
    if( rCost > 0 )
        rCostIn_ = rCost;
    else
        rCostOut_ = -rCost;
    if( rCost != 0 )
        scaledLocalisation_.Scale( 100 ); // $$$ LDC arbitrary 100m precision (useful for making path very close to obstacle expensive)
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject destructor
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObject::~DEC_Path_KnowledgeObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject::ComputeCost
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObject::ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData&, const TerrainData&, double weight ) const
{
    static const double epsilon = 1e-4;
    const MT_Line line( from, to );
    bool isIntersectingWithReal = realLocalisation_.Intersect2D( line, epsilon );
    bool isToInsideReal = realLocalisation_.IsInside( to, epsilon );
    bool isFromInsideReal = realLocalisation_.IsInside( from, epsilon );
    if( isIntersectingWithReal || isToInsideReal || isFromInsideReal )
    {
        if( ( isIntersectingWithReal && localisation_.Intersect2D( line, epsilon ) ) ||
            ( isToInsideReal && localisation_.IsInside( to, epsilon ) ) ||
            ( isFromInsideReal && localisation_.IsInside( from, epsilon ) ) )
        {
            if( rMaxTrafficability_ != 0. && weight > rMaxTrafficability_ )
                return -1.; //$$$$ CMA in order to block the unit if there is a non-trafficable object
            else if( rCostIn_ >= rObstructionThreshold_ ) //$$$$ SLG put the value in pathfind xml
                return -1.;  //$$$$ SLG in order to block the unit if there is an object
            else
                return rCostIn_;
        }
        else if( ( isIntersectingWithReal && scaledLocalisation_.Intersect2D( line, epsilon ) ) ||
                 ( isToInsideReal && scaledLocalisation_.IsInside( to, epsilon ) ) ||
                 ( isFromInsideReal && scaledLocalisation_.IsInside( from, epsilon ) ) )
            return rCostIn_;
    }
    return std::numeric_limits< double >::min();
}
