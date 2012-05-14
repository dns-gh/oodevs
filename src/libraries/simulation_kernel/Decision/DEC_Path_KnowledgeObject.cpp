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
    : scaledLocalisation_   ( knowledge.GetLocalisation() )
    , realLocalisation_     ( knowledge.GetObjectKnown() ? knowledge.GetObjectKnown()->GetLocalisation() : knowledge.GetLocalisation() )
    , pathClass_            ( pathClass )
    , knowledge_            ( knowledge )
{
    const double rCost = pathClass.GetObjectCost( knowledge.GetType() );
    if( rCost != 0 )
        scaledLocalisation_.Scale( 100 ); // $$$ LDC arbitrary 100m precision 
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject constructor
// Created: CMA 2011-11-24
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObject::DEC_Path_KnowledgeObject( const DEC_Population_PathClass& pathClass, const DEC_Knowledge_Object& knowledge )
    : scaledLocalisation_   ( knowledge.GetLocalisation() )
    , realLocalisation_     ( knowledge.GetObjectKnown() ? knowledge.GetObjectKnown()->GetLocalisation() : knowledge.GetLocalisation() )
    , pathClass_            ( pathClass )
    , knowledge_            ( knowledge )
{
    // NOTHING
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
    const MT_Line line( from, to );
    if( realLocalisation_.Intersect2D( line ) || realLocalisation_.IsInside( to ) || realLocalisation_.IsInside( from ) )
    {
        if( scaledLocalisation_.Intersect2D( line ) || scaledLocalisation_.IsInside( to ) || scaledLocalisation_.IsInside( from ) )
        {
            double rCostIn = pathClass_.GetObjectCost( knowledge_.GetType() );
            if( rCostIn < 0. )
                return 0.;
            else if( rCostIn >= pathClass_.GetThreshold() )
                return -1;  //$$$$ SLG in order to block the unit if there is an object
            else
                return rCostIn;
        }
    }
    return std::numeric_limits< double >::min();
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject::GetCostOut
// Created: NLD 2007-02-09
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObject::GetCostOut() const
{
    double cost = pathClass_.GetObjectCost( knowledge_.GetType() );
    return cost > 0 ? 0. : -cost;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObject::GetMaxTrafficability
// Created: CMA 2011-09-09
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObject::GetMaxTrafficability() const
{
    return knowledge_.GetMaxTrafficability();
}