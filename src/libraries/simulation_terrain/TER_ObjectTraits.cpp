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
#include "TER_Object_ABC.h"
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
// Name: TER_ObjectTraits::CompareOnX
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
int TER_ObjectTraits::CompareOnX( double rValue, const T_Value& pObject ) const
{
    return Intersector( pObject->GetLocalisation().GetBoundingBox() ).CompareOnX( rValue );
}

// -----------------------------------------------------------------------------
// Name: TER_ObjectTraits::CompareOnY
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
int TER_ObjectTraits::CompareOnY( double rValue, const T_Value& pObject ) const
{
    return Intersector( pObject->GetLocalisation().GetBoundingBox() ).CompareOnY( rValue );
}
