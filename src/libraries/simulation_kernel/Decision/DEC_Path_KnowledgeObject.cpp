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
#include "DEC_Population_PathClass.h"
#include "MIL_Singletons.h"
#include "MIL_Time_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "entities/Objects/MIL_Object_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include <map>

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject constructor
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObject::DEC_Path_KnowledgeObject( const DEC_Knowledge_Object& knowledge, double rCost, double rObstructionThreshold )
    : localisation_         ( knowledge.GetLocalisation() )
    , scaledLocalisation_   ( localisation_ )
    , realLocalisation_     ( knowledge.GetObjectKnown() ? knowledge.GetObjectKnown()->GetLocalisation() : knowledge.GetLocalisation() )
    , rMaxTrafficability_   ( knowledge.GetMaxTrafficability() )
    , rThreshold_           ( rObstructionThreshold )
    , rCost_                ( rCost )
{
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
    , realLocalisation_     ( knowledge.GetObjectKnown() ? knowledge.GetObjectKnown()->GetLocalisation() : knowledge.GetLocalisation() )
    , rMaxTrafficability_   ( knowledge.GetMaxTrafficability() )
    , rThreshold_           ( pathClass.GetThreshold() )
    , rCost_                ( pathClass.GetObjectCost( knowledge.GetType() ) )
{
    if( rCost_ != 0 )
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
    if( isIntersectingWithReal )
    {
        if( isIntersectingWithReal && localisation_.Intersect2D( line, epsilon ) ||
            isToInsideReal && localisation_.IsInside( to, epsilon ) ||
            isFromInsideReal && localisation_.IsInside( from, epsilon ) )
            return ComputeCost( weight );
        if( isIntersectingWithReal && scaledLocalisation_.Intersect2D( line, epsilon ) ||
            isToInsideReal && scaledLocalisation_.IsInside( to, epsilon ) ||
            isFromInsideReal && scaledLocalisation_.IsInside( from, epsilon ) )
            return rCost_;
    }
    if( isToInsideReal )
    {
        if( isIntersectingWithReal && localisation_.Intersect2D( line, epsilon ) ||
            isToInsideReal && localisation_.IsInside( to, epsilon ) ||
            isFromInsideReal && localisation_.IsInside( from, epsilon ) )
            return ComputeCost( weight );
        if( isIntersectingWithReal && scaledLocalisation_.Intersect2D( line, epsilon ) ||
            isToInsideReal && scaledLocalisation_.IsInside( to, epsilon ) ||
            isFromInsideReal && scaledLocalisation_.IsInside( from, epsilon ) )
            return rCost_;
    }
    if( isFromInsideReal )
    {
        if( isIntersectingWithReal && localisation_.Intersect2D( line, epsilon ) ||
            isToInsideReal && localisation_.IsInside( to, epsilon ) ||
            isFromInsideReal && localisation_.IsInside( from, epsilon ) )
            return ComputeCost( weight );
        if( isIntersectingWithReal && scaledLocalisation_.Intersect2D( line, epsilon ) ||
            isToInsideReal && scaledLocalisation_.IsInside( to, epsilon ) ||
            isFromInsideReal && scaledLocalisation_.IsInside( from, epsilon ) )
            return rCost_;
    }
    return std::numeric_limits< double >::min();
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject::ComputeCost
// Created: MCO 2012-11-22
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObject::ComputeCost( double weight ) const
{
    if( rMaxTrafficability_ != 0. && weight > rMaxTrafficability_ )
        return -1.; //$$$$ CMA in order to block the unit if there is a non-trafficable object
    if( rCost_ >= rThreshold_ )
        return -1;  //$$$$ SLG in order to block the unit if there is an object
    return std::max( 0., rCost_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject::GetCostOut
// Created: NLD 2007-02-09
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObject::GetCostOut() const
{
    return std::max( 0., - rCost_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject::GetMaxTrafficability
// Created: CMA 2011-09-09
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObject::GetMaxTrafficability() const
{
    return rMaxTrafficability_;
}
