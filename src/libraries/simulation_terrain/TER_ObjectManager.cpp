// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_ObjectManager.h"
#include "TER_Object_ABC.h"
#include "TER_ObjectVisitor_ABC.h"
#include <spatialcontainer/SpatialContainerTraits.h>

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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_ObjectManager::GetListAt
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_ObjectManager::GetListAt( const MT_Vector2D& vPos, T_ObjectVector& objects ) const
{
    objects.reserve( 10 );
    spatialcontainer::PointIntersecter< double > intersecter( geometry::Point2< double >( vPos.rX_, vPos.rY_ ) );
    T_Objects::View view = objects_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_Object_ABC* pObject = view.NextElement();
        if( pObject && pObject->IsInside( vPos ) )
            objects.push_back( pObject );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_ObjectManager::GetListWithinCircle
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_ObjectManager::GetListWithinCircle( const MT_Vector2D& vCenter, double rRadius, T_ObjectVector& objects ) const
{
    objects.reserve( 10 );
    spatialcontainer::PointIntersecter< double > intersecter( geometry::Point2< double >( vCenter.rX_, vCenter.rY_ ), rRadius );
    T_Objects::View view = objects_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_Object_ABC* pObject = view.NextElement();
        if( pObject && pObject->GetLocalisation().GetArea() && pObject->Intersect2DWithCircle( vCenter, rRadius ) )
            objects.push_back( pObject );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_ObjectManager::GetListWithinCircle
// Created: SLG 2010-06-15
// -----------------------------------------------------------------------------
void TER_ObjectManager::GetListWithinCircle2( const MT_Vector2D& vCenter, double rRadius, std::vector< const TER_Object_ABC* >& objects ) const
{
    objects.reserve( 10 );
    spatialcontainer::PointIntersecter< double > intersecter( geometry::Point2< double >( vCenter.rX_, vCenter.rY_ ), rRadius );
    T_Objects::View view = objects_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        const TER_Object_ABC* pObject = view.NextElement();
        if( pObject && pObject->GetLocalisation().GetArea() && pObject->Intersect2DWithCircle( vCenter, rRadius ) )
            objects.push_back( pObject );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_ObjectManager::GetListWithinLocalisation
// Created: NLD 2005-11-09
// -----------------------------------------------------------------------------
void TER_ObjectManager::GetListWithinLocalisation( const TER_Localisation& localisation, T_ObjectVector& objects ) const
{
    const MT_Rect& boundingBox = localisation.GetBoundingBox();
    spatialcontainer::SegmentIntersecter< double > intersecter( geometry::Point2<double>( boundingBox.GetLeft(), boundingBox.GetBottom() )
                                                                , geometry::Point2<double>( boundingBox.GetRight(), boundingBox.GetTop() ) );

    T_Objects::View view = objects_.CreateView( intersecter );
    while( view.HasMoreElements() )
    {
        TER_Object_ABC* pObject = view.NextElement();
        if( pObject && pObject->Intersect2DWithLocalisation( localisation ) )
            objects.push_back( pObject );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_ObjectManager::GetAllObjects
// Created: JSR 2011-01-06
// -----------------------------------------------------------------------------
void TER_ObjectManager::GetAllObjects( T_ObjectVector& objects ) const
{
    T_Objects::View view = objects_.CreateView();
    while( view.HasMoreElements() )
    {
        TER_Object_ABC* pObject = view.NextElement();
        if( pObject )
            objects.push_back( pObject );
    }
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

// -----------------------------------------------------------------------------
// Name: TER_ObjectManager::Accept
// Created: JSR 2011-05-05
// -----------------------------------------------------------------------------
void TER_ObjectManager::Accept( TER_ObjectVisitor_ABC& visitor ) const
{
    T_Objects::View view = objects_.CreateView();
    while( view.HasMoreElements() )
    {
        TER_Object_ABC* pObject = view.NextElement();
        if( pObject )
            visitor.Visit( *pObject );
    }
}
