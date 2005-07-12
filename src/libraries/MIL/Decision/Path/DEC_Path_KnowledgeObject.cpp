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

#include "MIL_pch.h"
#include "DEC_Path_KnowledgeObject.h"

#include "DEC_Path.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Entities/Objects/MIL_RealObjectType.h"

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject constructor
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObject::DEC_Path_KnowledgeObject( const DEC_Knowledge_Object& knowledge )
    : localisation_( knowledge.GetLocalisation() )
    , nObjectType_( knowledge.GetType().GetID() )
    , rCostIn_( 0 )
    , rCostOut_( 0 )
//    , avoidanceLocalisation_( knowledge.GetAvoidanceLocalisation() )
{ 
//    InitializeAgentStatus();
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
    : localisation_           ( copy.localisation_ )
    , nObjectType_            ( copy.nObjectType_ )
    , rCostOut_               ( copy.rCostOut_ )
    , rCostIn_                ( copy.rCostIn_ )
//    , avoidanceLocalisation_  ( copy.avoidanceLocalisation_ )
//    , vAgentPos_              ( copy.vAgentPos_ )
//    , bAgentStartsInObject_   ( copy.bAgentStartsInObject_ )
//    , bAgentStartsOnBorder_   ( copy.bAgentStartsOnBorder_ )
//    , bAgentStartsInAvoidance_( copy.bAgentStartsInAvoidance_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject::operator=
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObject& DEC_Path_KnowledgeObject::operator=( const DEC_Path_KnowledgeObject& copy )
{
//    vAgentPos_               = copy.vAgentPos_;
//    bAgentStartsInObject_    = copy.bAgentStartsInObject_;
//    bAgentStartsOnBorder_    = copy.bAgentStartsOnBorder_;
//    bAgentStartsInAvoidance_ = copy.bAgentStartsInAvoidance_;

    localisation_.Reset( copy.localisation_ );
    nObjectType_ = copy.nObjectType_;
    rCostIn_     = copy.rCostIn_;
    rCostOut_    = copy.rCostOut_;
//    avoidanceLocalisation_.Reset( copy.avoidanceLocalisation_ );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject::GetTypeID
// Created: AGE 2005-03-30
// -----------------------------------------------------------------------------
uint DEC_Path_KnowledgeObject::GetTypeID() const
{
    return nObjectType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject::SetCosts
// Created: AGE 2005-03-30
// -----------------------------------------------------------------------------
void DEC_Path_KnowledgeObject::SetCosts( MT_Float rCostIn, MT_Float rCostOut )
{
    rCostIn_  = rCostIn;
    rCostOut_ = rCostOut;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject::ComputeCost
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
MT_Float DEC_Path_KnowledgeObject::ComputeCost( const MT_Vector2D& /*from*/, const MT_Vector2D& to, const TerrainData&, const TerrainData& ) const
{
    if( localisation_.IsInside( to ) )
        return rCostIn_;
    return rCostOut_;
}
