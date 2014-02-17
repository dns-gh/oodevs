// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "TesselatorBase.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>


namespace
{

void CALLBACK combineDataCallback( GLdouble coords[3], void * /*vertex_data*/[4], GLfloat /*weight*/[4], unsigned int *dataOut, TesselatorBase* pTesselatorBase )
{
    const geometry::Point p( static_cast< float >( coords[0] ), static_cast< float >( coords[1] ) );
    pTesselatorBase->Combine( p, *dataOut );
}

void CALLBACK vertexDataCallback( unsigned int nIndex, TesselatorBase* pTesselatorBase )
{
    pTesselatorBase->Vertex( nIndex );
}

void CALLBACK beginDataCallback( GLenum type, TesselatorBase* pTesselatorBase )
{
    pTesselatorBase->Begin( type );
}

void CALLBACK endDataCallback( TesselatorBase* pTesselatorBase )
{
    pTesselatorBase->End();
}

};

// -----------------------------------------------------------------------------
// Name: TesselatorBase constructor
// Created: AGE 2005-01-10
// -----------------------------------------------------------------------------
TesselatorBase::TesselatorBase( unsigned int windingRule )
    : pTesselatorBase_( 0 )
    , started_( false )
{
    pTesselatorBase_ = gluNewTess();
    gluTessCallback( pTesselatorBase_, GLU_TESS_VERTEX_DATA,  (void (CALLBACK *)()) vertexDataCallback );
    gluTessCallback( pTesselatorBase_, GLU_TESS_COMBINE_DATA, (void (CALLBACK *)()) combineDataCallback );
    gluTessCallback( pTesselatorBase_, GLU_TESS_BEGIN_DATA,   (void (CALLBACK *)()) beginDataCallback );
    gluTessCallback( pTesselatorBase_, GLU_TESS_END_DATA,     (void (CALLBACK *)()) endDataCallback );
    gluTessProperty( pTesselatorBase_, GLU_TESS_WINDING_RULE, windingRule );
    gluTessBeginPolygon( pTesselatorBase_, this );
}

// -----------------------------------------------------------------------------
// Name: TesselatorBase destructor
// Created: AGE 2005-01-10
// -----------------------------------------------------------------------------
TesselatorBase::~TesselatorBase()
{
    gluDeleteTess( pTesselatorBase_ );
}

// -----------------------------------------------------------------------------
// Name: TesselatorBase::StartPath
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void TesselatorBase::StartPath()
{
    if( started_ )
        EndPath( false );
    started_ = true;
    gluTessBeginContour( pTesselatorBase_ );
}

// -----------------------------------------------------------------------------
// Name: TesselatorBase::AddPoints
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void TesselatorBase::AddPoints( const T_Points& points )
{
    points_.reserve( points_.size() + points.size() );
    for( T_Points::const_iterator it = points.begin(); it != points.end(); ++it )
        TesselatorBase::AddPoint( *it );
}

// -----------------------------------------------------------------------------
// Name: TesselatorBase::AddPoint
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void TesselatorBase::AddPoint( const geometry::Point& point )
{
    points_.push_back( point );
    double p[3] = { point.X(), point.Y(), 0 };
    unsigned int intIndex = static_cast< unsigned int >( points_.size() - 1 );
    gluTessVertex( pTesselatorBase_, p, reinterpret_cast< void* >( intIndex ) );
}

// -----------------------------------------------------------------------------
// Name: TesselatorBase::EndPath
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void TesselatorBase::EndPath( bool /*close*/ /*= true*/ )
{
    gluTessEndContour( pTesselatorBase_ );
    started_ = false;
}

// -----------------------------------------------------------------------------
// Name: TesselatorBase::Combine
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void TesselatorBase::Combine( const geometry::Point& point, unsigned int& index )
{
    points_.push_back( point );
    index = static_cast< unsigned int >( points_.size() - 1 );
}

// -----------------------------------------------------------------------------
// Name: TesselatorBase::Vertex
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void TesselatorBase::Vertex( unsigned int index )
{
    Vertex( points_.at( index ), index );
}

// -----------------------------------------------------------------------------
// Name: TesselatorBase::Commit
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void TesselatorBase::Commit()
{
    if( started_ )
        EndPath( false );
    gluTessEndPolygon( pTesselatorBase_ );
}

// -----------------------------------------------------------------------------
// Name: TesselatorBase::GetPoints
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
const TesselatorBase::T_Points& TesselatorBase::GetPoints() const
{
    return points_;
}
