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

#include "ter_pch.h"
#include "TER_ObjectManager.h"
#include "pathfind/SpatialContainerTraits.h"
#include "TER_DynaObject_ABC.h"

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
// Name: TER_ObjectManager::GetListDynaObjectsAt
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_ObjectManager::GetListDynaObjectsAt( const MT_Vector2D& vPos, T_DynaObjectVector& dynaObjectsSet ) const
{
    dynaObjectsSet.reserve( 10 );
    pathfind::PointIntersecter< MT_Float > intersecter( geometry::Point2< MT_Float >( vPos.rX_, vPos.rY_ ) );
    T_Objects::View view = objects_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_DynaObject_ABC* pObject = view.NextElement();
        if( pObject && pObject->IsInside( vPos ) )
            dynaObjectsSet.push_back( pObject );
    };
}
    
// -----------------------------------------------------------------------------
// Name: TER_ObjectManager::GetListDynaObjectWithinCircle
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_ObjectManager::GetListDynaObjectWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, T_DynaObjectVector& dynaObjectsSet ) const
{
    dynaObjectsSet.reserve( 10 );
    pathfind::PointIntersecter< MT_Float > intersecter( geometry::Point2< MT_Float >( vCenter.rX_, vCenter.rY_ ), rRadius );
    T_Objects::View view = objects_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_DynaObject_ABC* pObject = view.NextElement();
        if( pObject && pObject->Intersect2DWithCircle( vCenter, rRadius ) )
            dynaObjectsSet.push_back( pObject );
    };
}

// -----------------------------------------------------------------------------
// Name: TER_ObjectManager::UpdateObjectPosition
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_ObjectPositionHint TER_ObjectManager::UpdateObjectPosition( TER_DynaObject_ABC& object, const TER_ObjectPositionHint& hint )
{
    objects_.Erase( &object, hint );
    return objects_.Insert( &object );
}

// -----------------------------------------------------------------------------
// Name: TER_ObjectManager::RemoveObject
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
bool TER_ObjectManager::RemoveObject( TER_DynaObject_ABC& object, const TER_ObjectPositionHint& hint )
{
    return objects_.Erase( &object, hint );
}
