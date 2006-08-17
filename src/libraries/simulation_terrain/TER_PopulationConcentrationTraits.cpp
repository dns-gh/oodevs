// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ter_pch.h"
#include "TER_PopulationConcentrationTraits.h"
#include "TER_PopulationConcentration_ABC.h"
#include "TER_Localisation.h"
#include "MT_Tools/MT_Rect.h"
#include "pathfind/SpatialContainerTraits.h"

namespace
{
    pathfind::SegmentIntersecter< MT_Float > Intersector( const MT_Rect& boundingBox )
    {
        return pathfind::SegmentIntersecter< MT_Float >( 
            geometry::Point2< MT_Float >( boundingBox.GetLeft(), boundingBox.GetBottom() ),
            geometry::Point2< MT_Float >( boundingBox.GetRight(), boundingBox.GetTop() ) );
    };
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentrationTraits::CompareOnX
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
int TER_PopulationConcentrationTraits::CompareOnX( MT_Float rValue, const T_Value& pConcentration ) const
{
    return Intersector( pConcentration->GetLocation().GetBoundingBox() ).CompareOnX( rValue );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationConcentrationTraits::CompareOnY
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
int TER_PopulationConcentrationTraits::CompareOnY( MT_Float rValue, const T_Value& pConcentration ) const
{
    return Intersector( pConcentration->GetLocation().GetBoundingBox() ).CompareOnY( rValue );
}
