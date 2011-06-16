// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_PopulationFlowTraits.h"
#include "TER_PopulationFlow_ABC.h"
#include "TER_Localisation.h"
#include "MT_Tools/MT_Rect.h"
#include <spatialcontainer/SpatialContainerTraits.h>

namespace
{
    spatialcontainer::SegmentIntersecter< double > Intersector( const MT_Rect& boundingBox )
    {
        return spatialcontainer::SegmentIntersecter< double >(
            geometry::Point2< double >( boundingBox.GetLeft(), boundingBox.GetBottom() ),
            geometry::Point2< double >( boundingBox.GetRight(), boundingBox.GetTop() ) );
    };
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowTraits::CompareOnX
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
int TER_PopulationFlowTraits::CompareOnX( double rValue, const T_Value& pFlow ) const
{
    return Intersector( pFlow->GetLocation().GetBoundingBox() ).CompareOnX( rValue );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowTraits::CompareOnY
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
int TER_PopulationFlowTraits::CompareOnY( double rValue, const T_Value& pFlow ) const
{
    return Intersector( pFlow->GetLocation().GetBoundingBox() ).CompareOnY( rValue );
}
