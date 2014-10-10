// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************_

#include "clients_gui_pch.h"
#include "Tesselator.h"
#include <GL/gl.h>
#include <GL/glu.h>

using namespace gui;

namespace
{
    void CALLBACK vertexDataCallback( GLvoid* data, gui::Tesselator* pTesselatorBase )
    {
        if( pTesselatorBase )
        {
            const GLdouble* coord = static_cast< GLdouble* >( data );
            const geometry::Point2d point( coord[ 0 ], coord[ 1 ] );
            pTesselatorBase->Vertex( point );
        }
    }

    void CALLBACK beginDataCallback( GLenum type, gui::Tesselator* pTesselatorBase )
    {
        if( pTesselatorBase )
            pTesselatorBase->Begin( type );
    }
};

Tesselator::Tesselator()
    : pTesselatorBase_( gluNewTess() )
{
    gluTessCallback( pTesselatorBase_, GLU_TESS_BEGIN_DATA,   (void (CALLBACK *)()) beginDataCallback );
    gluTessCallback( pTesselatorBase_, GLU_TESS_VERTEX_DATA,  (void (CALLBACK *)()) vertexDataCallback );
}

Tesselator::~Tesselator()
{
    gluDeleteTess( pTesselatorBase_ );
}

void Tesselator::Begin( unsigned int type )
{
    dataOut_.push_back( std::make_pair( type, std::vector< geometry::Point2d >() ) );
}

void Tesselator::Vertex( const geometry::Point2d& point )
{
    if( !dataOut_.empty() )
        dataOut_.back().second.push_back( point );
}

kernel::T_TessellatedPolygon Tesselator::Tessellate( const geometry::Polygon2f& polygon )
{
    dataOut_.clear();
    const auto& vertices = polygon.Vertices();
    std::vector< GLdouble > coords;
    for( size_t i = 0; i < vertices.size(); ++i )
    {
        const auto& point = vertices[ i ];
        coords.push_back( point.X() );
        coords.push_back( point.Y() );
        coords.push_back( 0 );
    }

    gluTessBeginPolygon( pTesselatorBase_, this );
    gluTessBeginContour( pTesselatorBase_ );

    for( size_t i = 0; i < vertices.size(); ++i )
        gluTessVertex( pTesselatorBase_, &coords[ i * 3 ], &coords[ i * 3 ] );

    gluTessEndContour( pTesselatorBase_ );
    gluTessEndPolygon( pTesselatorBase_ );
    return dataOut_;
}
