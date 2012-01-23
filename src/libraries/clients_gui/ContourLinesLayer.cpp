// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ContourLinesLayer.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/DetectionMap.h"
#include <deque>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer constructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
ContourLinesLayer::ContourLinesLayer( Controllers& controllers, DetectionMap& map )
    : controllers_( controllers )
    , map_        ( map )
    , modelLoaded_( false )
    , enabled_    ( false )
    , color_      ( 245, 245, 220 )
    , callListId_ ( 0 )
    , linesHeight_( 20 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer destructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
ContourLinesLayer::~ContourLinesLayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::NotifyUpdated
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void ContourLinesLayer::NotifyUpdated( const ModelLoaded& /*modelLoaded*/ )
{
    modelLoaded_ = true;
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::SetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void ContourLinesLayer::SetAlpha( float alpha )
{
    Layer2d_ABC::SetAlpha( alpha );
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::OptionChanged
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void ContourLinesLayer::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "ContourLinesColor" )
        color_.setNamedColor( value.To< QString >() );
    else if( name == "ContourLinesEnabled" )
        enabled_ = value.To< bool >();
    else if( name == "ContourLinesHeight" )
    {
        int h = value.To< int >();
        if( h > 0 && h != linesHeight_ )
        {
            linesHeight_ = h;
            bool modelLoaded = modelLoaded_;
            Reset();
            modelLoaded_ = modelLoaded;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void ContourLinesLayer::Paint( const geometry::Rectangle2f& /*viewport*/ )
{
    if( !ShouldDrawPass() || GetAlpha() == 0 || !enabled_  )
        return;
    if( !modelLoaded_ )
        return;
    if( callListId_ == 0 )
        CreateCallList();

    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_STENCIL_BUFFER_BIT );

    glColor4f( color_.red() / 256.f, color_.green() / 256.f, color_.blue() / 256.f, GetAlpha() );
    glEnable( GL_LINE_SMOOTH );
    glCallList( callListId_ );

    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::Reset
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void ContourLinesLayer::Reset()
{
    modelLoaded_ = false;
    if( callListId_ )
    {
        glDeleteLists( callListId_, 1 );
        callListId_ = 0;
    }
}

#define DATA( x, y ) ( data[ ( y )  * width + ( x ) ] )
#define XSECT( p1, p2 ) ( h[ p2 ] * xh[ p1 ] - h[ p1 ] * xh[ p2 ] ) / ( h[ p2 ] - h[ p1 ] )
#define YSECT( p1, p2 ) ( h[ p2 ] * yh[ p1 ] - h[ p1 ] * yh[ p2 ] ) / ( h[ p2 ] - h[ p1 ] )

namespace
{
    struct sContour
    {
        sContour( const geometry::Point2f& p1, const geometry::Point2f& p2 ) : loop_( false )
        {
            points_.push_back( p1 );
            points_.push_back( p2 );
        }

        int Insert( const geometry::Point2f& p1, const geometry::Point2f& p2 )
        {
            if( p1 == points_.front() )
            {
                if( p2 == points_.back() )
                    loop_ = true;
                else
                    points_.insert( points_.begin(), p2 );
                return 1;
            }
            if( p1 == points_.back() )
            {
                if( p2 == points_.front() )
                    loop_ = true;
                else
                    points_.push_back( p2 );
                return 2;
            }
            if( p2 == points_.front() )
            {
                if( p1 == points_.back() )
                    loop_ = true;
                else
                    points_.insert( points_.begin(), p1 );
                return 3;
            }
            if( p2 == points_.back() )
            {
                if( p1 == points_.front() )
                    loop_ = true;
                else
                    points_.push_back( p1 );
                return 4;
            }
            return 0;
        }

        bool Concatenate( sContour& contour, int ret )
        {
            bool found = false;
            if( ret == 1 || ret == 3)
            {
                const geometry::Point2f& p = points_.front();
                if( p == contour.points_.front() )
                {
                    std::reverse( contour.points_.begin(), contour.points_.end() );
                    points_.insert( points_.begin(), contour.points_.begin(), contour.points_.end() );
                    found = true;
                }
                else if( p == contour.points_.back() )
                {
                    points_.insert( points_.begin(), contour.points_.begin(), contour.points_.end() );
                    found = true;
                }
            }
            else
            {
                const geometry::Point2f& p = points_.back();
                if( p == contour.points_.front() )
                {
                    points_.insert( points_.end(), contour.points_.begin(), contour.points_.end() );
                    found = true;
                }
                else if( p == contour.points_.back() )
                {
                    std::reverse( contour.points_.begin(), contour.points_.end() );
                    points_.insert( points_.end(), contour.points_.begin(), contour.points_.end() );
                    found = true;
                }
            }
            if( found && points_.front() == points_.back() )
            {
                loop_ = true;
                points_.pop_back();
            }
            return found;
        }

        std::deque< geometry::Point2f > points_;
        bool loop_;
    };
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::Conrec
// Created: JSR 2012-01-19
// -----------------------------------------------------------------------------
void ContourLinesLayer::Conrec() const
{
    // Adapted from http://paulbourke.net/papers/conrec/

    std::map< int, std::deque< sContour > > allContours;

    const ElevationMap& elevation = map_.GetMap();
    const short* data  = elevation.Data();
    const unsigned int width = elevation.Width();
    const unsigned int height = elevation.Height();
    const float cellSize = static_cast< float >( elevation.GetCellSize() );

    int nc = elevation.MaximumElevation() / linesHeight_;
    std::vector< std::vector< geometry::Point2f > > points;
    for( int i = 0; i < nc; ++i )
        points.push_back( std::vector< geometry::Point2f >() );

    int sh[ 5 ];
    float h[ 5 ];
    float xh[ 5 ];
    float yh[ 5 ];
    float x1;
    float x2;
    float y1;
    float y2;

    static const int im[ 4 ] = { 0, 1, 1, 0 };
    static const int jm[ 4 ] = { 0, 0, 1, 1 };
    static const int castab[ 3 ][ 3 ][ 3 ] =
    {
        {
            { 0, 0 ,8 }, { 0, 2, 5 }, { 7, 6, 9 }
        },
        {
            { 0, 3, 4 }, { 1, 3, 1 }, { 4, 3, 0 }
        },
        {
            { 9, 6, 7 }, { 5, 2, 0 }, { 8, 0, 0 }
        }
    };

    for( register int j = height - 2; j >= 0 ; --j )
    {
        for( register unsigned int i = 0; i <= width - 2; ++i )
        {
            short temp1;
            short temp2;
            temp1 = std::min( DATA( i    , j ), DATA( i    , j + 1 ) );
            temp2 = std::min( DATA( i + 1, j ), DATA( i + 1, j + 1 ) );
            float dmin = std::min( temp1, temp2 );
            temp1 = std::max( DATA( i    , j ), DATA( i    , j + 1 ) );
            temp2 = std::max( DATA( i + 1, j ), DATA( i + 1, j + 1 ) );
            float dmax = std::max( temp1, temp2 );
            if( dmax >= linesHeight_ + 0.1 && dmin <= nc * linesHeight_  + 0.1 )
            {
                for( register int k = 0; k < nc; ++k )
                {
                    const float alt  = linesHeight_ * ( k + 1 ) + 0.1f;
                    if( alt >= dmin && alt <= dmax )
                    {
                        for( register int m = 4; m >=0; --m )
                        {
                            if( m > 0 )
                            {
                                h[ m ] = DATA( i + im [ m - 1 ], j + jm[ m - 1 ] ) - alt;
                                xh[ m ] = ( i + im[ m - 1 ] ) * cellSize;
                                yh[ m ] = ( j + jm[ m - 1 ] ) * cellSize;
                            }
                            else
                            {
                                h[ 0 ] = 0.25f * ( h[ 1 ] + h[ 2 ] + h[ 3 ] + h[ 4 ] );
                                xh[ 0 ] = ( i + 0.5f ) * cellSize;
                                yh[ 0 ] = ( j + 0.5f ) * cellSize;
                            }
                            if( h[ m ] > 0 )
                                sh[ m ] = 1;
                            else if( h[ m ] < 0)
                                sh[m] = -1;
                            else
                                sh[m] = 0;
                        }
                        for( register int m = 1; m <= 4; ++m )
                        {
                            int m1 = m;
                            int m2 = 0;
                            int m3 = ( m == 4 ) ? 1 : ( m + 1 );
                            int case_value = castab[ sh[ m1 ] + 1 ][ sh[ m2 ] + 1 ][ sh[ m3 ] + 1 ];
                            if( case_value != 0 )
                            {
                                switch( case_value )
                                {
                                case 1:
                                    x1 = xh[ m1 ];
                                    y1 = yh[ m1 ];
                                    x2 = xh[ m2 ];
                                    y2 = yh[ m2 ];
                                    break;
                                case 2:
                                    x1 = xh[ m2 ];
                                    y1 = yh[ m2 ];
                                    x2 = xh[ m3 ];
                                    y2 = yh[ m3 ];
                                    break;
                                case 3:
                                    x1 = xh[ m3 ];
                                    y1 = yh[ m3 ];
                                    x2 = xh[ m1 ];
                                    y2 = yh[ m1 ];
                                    break;
                                case 4:
                                    x1 = xh[ m1 ];
                                    y1 = yh[ m1 ];
                                    x2 = XSECT( m2, m3 );
                                    y2 = YSECT( m2, m3 );
                                    break;
                                case 5:
                                    x1 = xh[ m2 ];
                                    y1 = yh[ m2 ];
                                    x2 = XSECT( m3, m1 );
                                    y2 = YSECT( m3, m1 );
                                    break;
                                case 6:
                                    x1 = xh[ m3 ];
                                    y1 = yh[ m3 ];
                                    x2 = XSECT( m1, m2 );
                                    y2 = YSECT( m1, m2 );
                                    break;
                                case 7:
                                    x1 = XSECT( m1, m2 );
                                    y1 = YSECT( m1, m2 );
                                    x2 = XSECT( m2, m3 );
                                    y2 = YSECT( m2, m3 );
                                    break;
                                case 8:
                                    x1 = XSECT( m2, m3 );
                                    y1 = YSECT( m2, m3 );
                                    x2 = XSECT( m3, m1 );
                                    y2 = YSECT( m3, m1 );
                                    break;
                                case 9:
                                    x1 = XSECT( m3, m1 );
                                    y1 = YSECT( m3, m1 );
                                    x2 = XSECT( m1, m2 );
                                    y2 = YSECT( m1, m2 );
                                    break;
                                default:
                                    assert( false );
                                    break;
                                }
                                points[ k ].push_back( geometry::Point2f( x1, y1 ) );
                                points[ k ].push_back( geometry::Point2f( x2, y2 ) );
                            }
                        }
                    }
                }
            }
        }
    }

    for( int k = 0; k < nc; ++k )
    {
        glLineWidth( ( k +1 ) % 5 != 0 ? 1.f : 2.f );
        std::deque< sContour > contours;
        int counter = 0;
        while( counter + 2 < points[ k ].size() )
        {
            const geometry::Point2f& p1 = points[ k ][ counter++ ];
            const geometry::Point2f& p2 = points[ k ][ counter++ ];
            bool found = false;
            for( register std::size_t index = 0; index < contours.size(); ++index )
            {
                sContour& contour = contours[ index ];
                int result = contour.Insert( p1, p2 );
                if( result )
                {
                    found = true;
                    if( !contour.loop_ )
                    {
                        for( register std::size_t index2 = 0; index2 < contours.size(); ++index2 )
                        {
                            if( index != index2 )
                            {
                                sContour& contour2 = contours[ index2 ];
                                if( contour.Concatenate( contour2, result ) )
                                {
                                    contours.erase( contours.begin() + index2 );
                                    break;
                                }
                            }
                        }
                    }
                    if( contour.loop_ )
                    {
                        std::vector< geometry::Point2f > lineTmp( contour.points_.begin(), contour.points_.end() );
                        glVertexPointer( 2, GL_FLOAT, 0, &lineTmp[ 0 ] );
                        glDrawArrays( GL_LINE_LOOP, 0, static_cast< GLsizei >( lineTmp.size() ) );
                        contours.erase( contours.begin() + index );
                    }
                    break;
                }
            }
            if( !found )
                contours.push_back( sContour( p1, p2 ) );
        }
        for( register std::size_t i = 0; i < contours.size(); ++i )
        {
            const sContour& contour = contours[ i ];
            std::vector< geometry::Point2f > lineTmp( contour.points_.begin(), contour.points_.end() );
            glVertexPointer( 2, GL_FLOAT, 0, &lineTmp[ 0 ] );
            glDrawArrays( GL_LINE_STRIP, 0, static_cast< GLsizei >( lineTmp.size() ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::CreateCallList
// Created: JSR 2012-01-18
// -----------------------------------------------------------------------------
void ContourLinesLayer::CreateCallList()
{
    callListId_ = glGenLists( 1 );
    glNewList( callListId_, GL_COMPILE );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnableClientState( GL_VERTEX_ARRAY );
    Conrec();
    glDisableClientState( GL_VERTEX_ARRAY );
    glEndList();
}
