// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Path_KnowledgePopulation.h"

#include "DEC_PathClass.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "TER/TER_Localisation.h"

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgePopulation constructor
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
DEC_Path_KnowledgePopulation::DEC_Path_KnowledgePopulation( const DEC_PathClass& pathClass, const DEC_Knowledge_Population& knowledge )
    : knowledge_ ( &knowledge )
    , pathClass_ ( &pathClass )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgePopulation destructor
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
DEC_Path_KnowledgePopulation::~DEC_Path_KnowledgePopulation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgePopulation::ComputeCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
MT_Float DEC_Path_KnowledgePopulation::ComputeCost( const MT_Vector2D& /*from*/, const MT_Vector2D& to, const TerrainData& /*nToTerrainType*/, const TerrainData& /*nLinkTerrainType*/ ) const
{
    MIL_PopulationElement_ABC* pClosestElement = 0;
    MT_Float                   rDistance       = 0.0f;
    knowledge_->GetPopulationKnown().ComputeClosestAliveElement( to, pClosestElement, rDistance );
    const MT_Float rMaxRange = pathClass_->GetPopulationSecurityRange();
    if( !pClosestElement || rDistance > rMaxRange )
        return 0.0f;
    const MT_Float rElementCost = pathClass_->GetPopulationAttitudeCost( pClosestElement->GetAttitude() ) * pClosestElement->GetDensity();
    return rElementCost * ( rMaxRange - rDistance ) / rMaxRange;
}
