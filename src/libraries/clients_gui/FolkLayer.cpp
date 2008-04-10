// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "FolkLayer.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/WorldParameters.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "graphics/extensions.h"
#include <shapelib/shapefil.h>
#include <boost/shared_ptr.hpp>

using namespace gui;
using namespace kernel;
using namespace geometry;

// -----------------------------------------------------------------------------
// Name: FolkLayer constructor
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
FolkLayer::FolkLayer( Controller& controller, const kernel::CoordinateConverter_ABC& converter )
    : controller_( controller )
    , converter_( converter )
    , updateGradient_( true )
    , gradientTexture_( 0 )
{
    gradient_.AddColor( 0, Qt::green );
    gradient_.AddColor( 1, Qt::red );
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: FolkLayer destructor
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
FolkLayer::~FolkLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FolkLayer::SetGradient
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void FolkLayer::SetGradient( const Gradient& gradient )
{
    gradient_ = gradient;
    updateGradient_ = true;
}

// -----------------------------------------------------------------------------
// Name: FolkLayer::SetOccupation
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void FolkLayer::SetOccupation( const std::vector< float >& coordinates )
{
    if( coordinates.empty() )
        return;
    
    std::vector< float > transformed( coordinates );
    CIT_Edges itEdge = edges_.begin();
    std::vector< float >::iterator it = transformed.begin();
    float max = 0;
    for( ; it != transformed.end() && itEdge != edges_.end(); ++it, ++itEdge )
    {
        *it = *it * itEdge->ratio;
        max = std::max( max, *it );
    }
    if( max == 0 )
        max = 1;

    const float textureMin   = 0.5f / float( gradient_.Length() );
    const float textureRatio = ( gradient_.UsedRatio() - 2 * textureMin ) / max;
    itEdge = edges_.begin();
    it = transformed.begin();
    for( ; it != transformed.end() && itEdge != edges_.end(); ++it, ++itEdge  )
    {
        const float coordinate = *it * textureRatio + textureMin;
        assert( coordinate >= textureMin && coordinate <= 1 - textureMin );
        for( unsigned i = itEdge->start; i != itEdge->end; ++i )
            coordinates_.at( i ) = coordinate;
    }
}

// -----------------------------------------------------------------------------
// Name: FolkLayer::UpdateGradient
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void FolkLayer::UpdateGradient()
{
    if( ! gradientTexture_ )
        glGenTextures( 1, &gradientTexture_ );
    gl::Initialize();
    gl::glActiveTexture( gl::GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_1D, gradientTexture_ );
    gradient_.MakeGlTexture( 1 );
    updateGradient_ = false;
}

// -----------------------------------------------------------------------------
// Name: FolkLayer::Paint
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void FolkLayer::Paint( const Rectangle2f& viewport )
{
    if( graph_.empty() && !graphFile_.empty() )
        LoadGraph( graphFile_ );
    if( graph_.empty() )
        return;
    if( updateGradient_ )
        UpdateGradient();

    if( ! box_.Intersect( viewport ).IsEmpty() )
    {
        glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT );
        glLineWidth( 1 );
        gl::glActiveTexture( gl::GL_TEXTURE1 );
        glBindTexture( GL_TEXTURE_1D, 0 );
        glDisable( GL_TEXTURE_1D );
        glDisable( GL_TEXTURE_2D );
        gl::glActiveTexture( gl::GL_TEXTURE0 );
        glDisable( GL_TEXTURE_GEN_S );
        glDisable( GL_TEXTURE_GEN_T );
        glEnable( GL_TEXTURE_1D );
        glDisable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_1D, gradientTexture_ );
        glColor4f( 1, 1, 1, GetAlpha() );
        glVertexPointer( 2, GL_FLOAT, 0, (const void*)(&graph_.front()) );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glTexCoordPointer( 1, GL_FLOAT, 0, (const void*)(&coordinates_.front()) );
        glDrawArrays( GL_LINES, 0, graph_.size() );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        glPopAttrib();
        glDisable( GL_TEXTURE_1D );
    }
}

// -----------------------------------------------------------------------------
// Name: FolkLayer::NotifyUpdated
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void FolkLayer::NotifyUpdated( const ModelLoaded& modelLoaded )
{
    WorldParameters parameters( modelLoaded.config_ );
    graphFile_ = parameters.populationGraph_;
}

namespace
{
    void ShpClose( SHPHandle shp ) { if( shp ) SHPClose( shp ); }

    template< typename Edge >
    void DoAddPoints( SHPObject& object, T_PointVector& points, std::vector< Edge >& edges, const CoordinateConverter_ABC& converter )
    {
        for( int part = 0; part < object.nParts; ++part )
        {
            const int start = object.panPartStart[ part ];
            const int end   = part < object.nParts - 1 ? object.panPartStart[ part+1 ] : object.nVertices;
            Edge edge;
            edge.start = points.size();
            float length = 0;
            points.reserve( points.size() + 2 * ( end - start ) );
            for( int i = start; i < end; ++i )
            {
                const Point2d geodetic( object.padfX[i], object.padfY[i] );
                const Point2f point = converter.ConvertFromGeo( geodetic );
                if( i > start )
                    length += points.back().Distance( point );
                points.push_back( point );
                if( i > start && i < end-1 )
                    points.push_back( point );
            }
            edge.end = points.size();
            edge.ratio = 1.f / length;
            edges.push_back( edge );
        }
    }

    template< typename Edge >
    void AddPoints( SHPObject& object, T_PointVector& points, std::vector< Edge >& edges, const CoordinateConverter_ABC& converter )
    {
        switch( object.nSHPType )
        {
        default:
        case SHPT_NULL: return;
        case SHPT_ARC:
        case SHPT_ARCZ:
        case SHPT_ARCM:
            return DoAddPoints( object, points, edges, converter );
        };
    }
}

// -----------------------------------------------------------------------------
// Name: FolkLayer::LoadGraph
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void FolkLayer::LoadGraph( const std::string& graph )
{
    boost::shared_ptr< SHPInfo > shp( SHPOpen( graph.c_str() ,"rb" ), &ShpClose );
    if( ! shp ) return;
    const Point2f bottomLeft = converter_.ConvertFromGeo( Point2d( shp->adBoundsMin[0], shp->adBoundsMin[1] ) );
    const Point2f topRight   = converter_.ConvertFromGeo( Point2d( shp->adBoundsMax[0], shp->adBoundsMax[1] ) );
    box_ = Rectangle2f( bottomLeft, topRight );
    edges_.reserve( shp->nRecords );
    graph_.reserve( shp->nRecords * 4 );
    for( int i = 0; i < shp->nRecords; ++i )
    {
        boost::shared_ptr< SHPObject > object( SHPReadObject( shp.get(), i ), &SHPDestroyObject );
        if( object )
            AddPoints( *object, graph_, edges_, converter_ );
    }
    coordinates_.resize( graph_.size() );
}
