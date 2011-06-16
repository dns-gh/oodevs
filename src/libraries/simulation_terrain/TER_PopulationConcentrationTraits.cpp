// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_PopulationConcentrationTraits.h"
#include "TER_PopulationConcentration_ABC.h"
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
// Name: TER_PopulationConcentrationTraits::CompareOnX
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
int TER_PopulationConcentrationTraits::CompareOnX( double rValue, const T_Value& pConcentration ) const
{
    return Intersector( pConcentration->GetLocation().GetBoundingBox() ).CompareOnX( rValue );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentrationTraits::CompareOnY
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
int TER_PopulationConcentrationTraits::CompareOnY( double rValue, const T_Value& pConcentration ) const
{
    return Intersector( pConcentration->GetLocation().GetBoundingBox() ).CompareOnY( rValue );
}
