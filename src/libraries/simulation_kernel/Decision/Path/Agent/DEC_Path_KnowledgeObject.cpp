// *****************************************************************************
//
// $Created: NLD 2004-04-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path_KnowledgeObject.cpp $
// $Author: Age $
// $Modtime: 10/06/05 14:40 $
// $Revision: 10 $
// $Workfile: DEC_Path_KnowledgeObject.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Path_KnowledgeObject.h"

#include "DEC_Agent_PathClass.h"
#include "Knowledge/DEC_Knowledge_Object.h"

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject constructor
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObject::DEC_Path_KnowledgeObject( const DEC_Agent_PathClass& pathClass, const DEC_Knowledge_Object& knowledge )
    : localisation_( knowledge.GetLocalisation() )
    , rCostIn_     ( 0 )
    , rCostOut_    ( 0 )
{ 
    const MT_Float rCost = pathClass.GetObjectCost( knowledge.GetType() );

    if( rCost > 0 )
        rCostIn_  = rCost;
    else
        rCostOut_ = -rCost;
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
// Name: DEC_Path_KnowledgeObject constructor
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObject::DEC_Path_KnowledgeObject( const DEC_Path_KnowledgeObject& copy )
    : localisation_( copy.localisation_ )
    , rCostOut_    ( copy.rCostOut_ )
    , rCostIn_     ( copy.rCostIn_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject::operator=
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObject& DEC_Path_KnowledgeObject::operator=( const DEC_Path_KnowledgeObject& copy )
{
    localisation_.Reset( copy.localisation_ );
    rCostIn_     = copy.rCostIn_;
    rCostOut_    = copy.rCostOut_;
    return *this;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject::ComputeCost
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
MT_Float DEC_Path_KnowledgeObject::ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData&, const TerrainData& ) const
{
    const MT_Line line( from, to );
    if( localisation_.Intersect2D( line ) || localisation_.IsInside( to ) )
        return rCostIn_;
    return std::numeric_limits< MT_Float >::min();
}
