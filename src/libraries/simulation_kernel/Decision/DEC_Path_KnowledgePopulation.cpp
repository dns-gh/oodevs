// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Path_KnowledgePopulation.h"
#include "DEC_Agent_PathClass_ABC.h"
#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Knowledge/DEC_Knowledge_Population.h"

namespace
{
    class PopulationPathInserter : public MIL_EntityVisitor_ABC< MIL_PopulationElement_ABC >
    {
    public:
        PopulationPathInserter( DEC_Path_KnowledgePopulation& container )
            : container_ ( container ) {}

        void Visit( const MIL_PopulationElement_ABC& element )
        {
            if( ! element.IsDead() ) // only keep alive elements
                container_.AddElement( element );
        }
    private:
        DEC_Path_KnowledgePopulation& container_;
    };
};

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgePopulation constructor
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
DEC_Path_KnowledgePopulation::DEC_Path_KnowledgePopulation( const DEC_Knowledge_Population& knowledge, const DEC_Agent_PathClass_ABC& pathClass, bool avoidPolicy )
    : pPathClass_  ( &pathClass )
    , bAvoidPolicy_( avoidPolicy )
{
    elements_.reserve( 10 );
    PopulationPathInserter pathInserter( *this );
    knowledge.GetPopulationKnown().Apply( pathInserter );
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
// Name: DEC_Path_KnowledgePopulation::AddElement
// Created: SBO 2006-02-24
// -----------------------------------------------------------------------------
void DEC_Path_KnowledgePopulation::AddElement( const MIL_PopulationElement_ABC& element )
{
    elements_.push_back( sPopulationElement( element ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgePopulation::ComputeCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgePopulation::ComputeCost( const MT_Vector2D& /*from*/, const MT_Vector2D& to, const TerrainData& /*nToTerrainType*/, const TerrainData& /*nLinkTerrainType*/ ) const
{
    const double rMaxRange = pPathClass_->GetPopulationSecurityRange();
    const sPopulationElement* pClosestElement = 0;
    const double rDistance = ComputeClosestElementInRange( to, rMaxRange, pClosestElement );
    if( bAvoidPolicy_ ) // avoiding policy (non-terrorist)
    {
        if( !pClosestElement )
            return 0;
        const double rElementCost = pPathClass_->GetPopulationAttitudeCost( pClosestElement->pAttitude_->GetID() ) * pClosestElement->rDensity_;
        return rElementCost * ( rMaxRange - rDistance ) / rMaxRange;
    }
    else // "loving" policy (terrorist)
    {
        if( !pClosestElement )
            return pPathClass_->GetCostOutsideOfPopulation();
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgePopulation::ComputeClosestElementInRange
// Created: SBO 2006-02-24
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgePopulation::ComputeClosestElementInRange( const MT_Vector2D& position, double rMaxRange, sPopulationElement const*& pResult ) const
{
    double rMinDistance = std::numeric_limits< double >::max();
    for( CIT_PopulationElements it = elements_.begin(); it != elements_.end(); ++it )
    {
        MT_Vector2D nearestPoint;
        if( !it->location_.ComputeNearestPoint( position, nearestPoint ) )
            continue;
        double rDistance = position.Distance( nearestPoint );
        if( rDistance < rMaxRange && rDistance < rMinDistance )
        {
            rMinDistance = rDistance;
            pResult = &*it;
        }
    }
    return rMinDistance;
}
