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
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_ObjectManager.cpp $
// $Author: Age $
// $Modtime: 4/03/05 14:46 $
// $Revision: 2 $
// $Workfile: TER_ObjectManager.cpp $
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_ObjectManager.h"
#include "pathfind/SpatialContainerTraits.h"
#include "TER_Object_ABC.h"

// -----------------------------------------------------------------------------
// Name: TER_ObjectManager constructor
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_ObjectManager::TER_ObjectManager( const MT_Rect& extent )
    : objects_( T_Objects::T_Rectangle( extent.GetLeft(), extent.GetBottom(), extent.GetRight(), extent.GetTop() ) )
{
    objects_.SetRefinementPolicy( 20 ); // $$$$ AGE 2005-02-02: profile something ?
}

// -----------------------------------------------------------------------------
// Name: TER_ObjectManager destructor
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_ObjectManager::~TER_ObjectManager()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_ObjectManager::GetListAt
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_ObjectManager::GetListAt( const MT_Vector2D& vPos, T_ObjectVector& objects ) const
{
    objects.reserve( 10 );
    pathfind::PointIntersecter< MT_Float > intersecter( geometry::Point2< MT_Float >( vPos.rX_, vPos.rY_ ) );
    T_Objects::View view = objects_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_Object_ABC* pObject = view.NextElement();
        if( pObject && pObject->IsInside( vPos ) )
            objects.push_back( pObject );
    };
}
    
// -----------------------------------------------------------------------------
// Name: TER_ObjectManager::GetListWithinCircle
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_ObjectManager::GetListWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, T_ObjectVector& objects ) const
{
    objects.reserve( 10 );
    pathfind::PointIntersecter< MT_Float > intersecter( geometry::Point2< MT_Float >( vCenter.rX_, vCenter.rY_ ), rRadius );
    T_Objects::View view = objects_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_Object_ABC* pObject = view.NextElement();
        if( pObject && pObject->Intersect2DWithCircle( vCenter, rRadius ) )
            objects.push_back( pObject );
    };
}

// -----------------------------------------------------------------------------
// Name: TER_ObjectManager::GetListWithinLocalisation
// Created: NLD 2005-11-09
// -----------------------------------------------------------------------------
void TER_ObjectManager::GetListWithinLocalisation( const TER_Localisation& localisation, T_ObjectVector& objects ) const
{
    const MT_Rect& boundingBox = localisation.GetBoundingBox();
    pathfind::SegmentIntersecter< MT_Float > intersecter( geometry::Point2<MT_Float>( boundingBox.GetLeft(), boundingBox.GetBottom() )
                                                        , geometry::Point2<MT_Float>( boundingBox.GetRight(), boundingBox.GetTop() ) );

    T_Objects::View view = objects_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_Object_ABC* pObject = view.NextElement();
        if( pObject && pObject->Intersect2DWithLocalisation( localisation ) )
            objects.push_back( pObject );
    };
}

// -----------------------------------------------------------------------------
// Name: TER_ObjectManager::UpdatePosition
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_Object_ABC::T_Hint TER_ObjectManager::UpdatePosition( TER_Object_ABC& object, const TER_Object_ABC::T_Hint& hint )
{
    objects_.Erase( &object, hint );
    return objects_.Insert( &object );
}

// -----------------------------------------------------------------------------
// Name: TER_ObjectManager::Remove
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
bool TER_ObjectManager::Remove( TER_Object_ABC& object, const TER_Object_ABC::T_Hint& hint )
{
    return objects_.Erase( &object, hint );
}
