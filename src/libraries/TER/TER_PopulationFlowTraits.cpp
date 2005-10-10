// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/ter/src/TER_PopulationFlowTraits.cpp $
// $Author: Age $
// $Modtime: 31/01/05 18:53 $
// $Revision: 1 $
// $Workfile: TER_PopulationFlowTraits.cpp $
//
// *****************************************************************************

#include "ter_pch.h"
#include "TER_PopulationFlowTraits.h"
#include "TER_PopulationFlow_ABC.h"
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
// Name: TER_PopulationFlowTraits::CompareOnX
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
int TER_PopulationFlowTraits::CompareOnX( MT_Float rValue, const T_Value& pFlow ) const
{
    return Intersector( pFlow->GetLocation().GetBoundingBox() ).CompareOnX( rValue );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowTraits::CompareOnY
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
int TER_PopulationFlowTraits::CompareOnY( MT_Float rValue, const T_Value& pFlow ) const
{
    return Intersector( pFlow->GetLocation().GetBoundingBox() ).CompareOnY( rValue );
}
