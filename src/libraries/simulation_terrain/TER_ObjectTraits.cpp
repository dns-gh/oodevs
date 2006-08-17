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
// $Archive: /MVW_v10/Build/SDK/ter/src/TER_ObjectTraits.cpp $
// $Author: Age $
// $Modtime: 31/01/05 18:53 $
// $Revision: 1 $
// $Workfile: TER_ObjectTraits.cpp $
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_ObjectTraits.h"
#include "pathfind/SpatialContainerTraits.h"
#include "MT_Tools/MT_Rect.h"
#include "TER_Object_ABC.h"
#include "TER_Localisation.h"

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
// Name: TER_ObjectTraits::CompareOnX
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
int TER_ObjectTraits::CompareOnX( MT_Float rValue, const T_Value& pObject ) const
{
    return Intersector( pObject->GetLocalisation().GetBoundingBox() ).CompareOnX( rValue );
}

// -----------------------------------------------------------------------------
// Name: TER_ObjectTraits::CompareOnY
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
int TER_ObjectTraits::CompareOnY( MT_Float rValue, const T_Value& pObject ) const
{
    return Intersector( pObject->GetLocalisation().GetBoundingBox() ).CompareOnY( rValue );
}
