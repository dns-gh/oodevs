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
#include "clients_kernel/Controller.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/DetectionMap.h"
#include <deque>

using namespace kernel;
using namespace gui;

// $$$$ JSR 2012-02-08: hack parce qu'on ne peux pas tuer le process
bool ContourLinesLayer::valid_ =  true;

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer constructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
ContourLinesLayer::ContourLinesLayer( Controllers& controllers, DetectionMap& map )
    : controllers_  ( controllers )
    , map_          ( map )
    , modelLoaded_  ( false )
    , enabled_      ( false )
    , color_        ( 245, 245, 220 )
    , callListId_   ( 0 )
    , linesHeight_  ( 20 )
    , stopThread_   ( false )
    , threadRunning_( false )
    , computed_     ( false )
{
    controllers_.Register( *this );
    thread_.reset( new tools::thread::ThreadPool( 1 ) );
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer destructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
ContourLinesLayer::~ContourLinesLayer()
{
    valid_ =  false;
    stopThread_ = true;
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
    Layer2D::SetAlpha( alpha );
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
    {
        CreateCallList();
        return;
    }

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
    boost::mutex::scoped_lock locker( mutex_ );
    modelLoaded_ = false;
    computed_ = false;
    if( threadRunning_ )
        stopThread_ = true;
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

        bool Insert( const geometry::Point2f& p1, const geometry::Point2f& p2, bool& ret )
        {
            const geometry::Point2f& front = points_.front();
            const geometry::Point2f& back = points_.back();
            if( p1 == front )
            {
                if( p2 == back )
                    loop_ = true;
                else
                    points_.push_front( p2 );
                ret = true;
                return true;
            }
            if( p1 == back )
            {
                if( p2 == front )
                    loop_ = true;
                else
                    points_.push_back( p2 );
                ret = false;
                return true;
            }
            if( p2 == front )
            {
                if( p1 == back )
                    loop_ = true;
                else
                    points_.push_front( p1 );
                ret = true;
                return true;
            }
            if( p2 == back )
            {
                if( p1 == front )
                    loop_ = true;
                else
                    points_.push_back( p1 );
                ret = false;
                return true;
            }
            return false;
        }

        bool Concatenate( const sContour& contour, bool ret )
        {
            bool found = false;
            if( ret )
            {
                const geometry::Point2f& p = points_.front();
                if( p == contour.points_.front() )
                {
                    points_.insert( points_.begin(), contour.points_.rbegin(), contour.points_.rend() - 1);
                    found = true;
                }
                else if( p == contour.points_.back() )
                {
                    points_.insert( points_.begin(), contour.points_.begin(), contour.points_.end() - 1 );
                    found = true;
                }
            }
            else
            {
                const geometry::Point2f& p = points_.back();
                if( p == contour.points_.front() )
                {
                    points_.insert( points_.end(), contour.points_.begin() + 1, contour.points_.end() );
                    found = true;
                }
                else if( p == contour.points_.back() )
                {
                    points_.insert( points_.end(), contour.points_.rbegin() + 1, contour.points_.rend() );
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
void ContourLinesLayer::Conrec()
{
    // Adapted from http://paulbourke.net/papers/conrec/
    const ElevationMap& elevation = map_.GetMap();
    const short* data  = elevation.Data();
    const unsigned int width = elevation.Width();
    const unsigned int height = elevation.Height();
    const float cellSize = static_cast< float >( elevation.GetCellSize() );

    int nc = elevation.MaximumElevation() / linesHeight_;
    std::vector< boost::shared_ptr< T_PointVector > > points;
    for( int i = 0; i < nc; ++i )
        points.push_back( boost::shared_ptr< T_PointVector >( new T_PointVector() ) );

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
        if( !valid_ )
            return;

        observer_.SetPercentage( static_cast< short >( 10.f * ( height - j ) / height ) );
        controllers_.controller_.Update( observer_ );

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
                                points[ k ].get()->push_back( geometry::Point2f( x1, y1 ) );
                                points[ k ].get()->push_back( geometry::Point2f( x2, y2 ) );
                            }
                        }
                    }
                }
            }
        }
    }

    for( int k = 0; k < nc; ++k )
    {
        if( !valid_ )
            return;
        observer_.SetPercentage( static_cast< short >( 10 + 90.f * k / nc ) );
        controllers_.controller_.Update( observer_ );
        bool large = ( k +1 ) % 5 == 0;
        std::deque< boost::shared_ptr< sContour > > contours;
        register std::size_t counter = 0;
        const T_PointVector* vector = points[ k ].get();
        const std::size_t vectorsize = vector->size();
        while( counter + 2 < vectorsize )
        {
            const geometry::Point2f& p1 = vector->at( counter++ );
            const geometry::Point2f& p2 = vector->at( counter++ );
            bool found = false;
            const size_t size = contours.size();
            for( register std::size_t index = 0; index < size; ++index )
            {
                sContour* contour = contours[ index ].get();
                bool result;
                if( contour->Insert( p1, p2, result ) )
                {
                    found = true;
                    if( !contour->loop_ )
                    {
                        for( register std::size_t index2 = index + 1; index2 < size; ++index2 )
                        {
                            if( !valid_ )
                                return;
                            if( stopThread_ )
                            {
                                boost::mutex::scoped_lock locker( mutex_ );
                                loops_[ 0 ].clear();
                                loops_[ 1 ].clear();
                                loops_[ 2 ].clear();
                                loops_[ 3 ].clear();
                                stopThread_ = false;
                                threadRunning_ = false;
                                observer_.SetPercentage( 0 );
                                controllers_.controller_.Update( observer_ );
                                return;
                            }
                            sContour* contour2 = contours[ index2 ].get();
                            if( contour->Concatenate( *contour2, result ) )
                            {
                                contours.erase( contours.begin() + index2 );
                                break;
                            }
                        }
                    }
                    if( contour->loop_ )
                    {
                        T_PointVector* v = new T_PointVector( contour->points_.begin(), contour->points_.end() );
                        loops_[ large ? 0 : 2 ].push_back( boost::shared_ptr< T_PointVector >( v ) );
                        contours.erase( contours.begin() + index );
                    }
                    break;
                }
            }
            if( !found )
                contours.push_back( boost::shared_ptr< sContour >( new sContour( p1, p2 ) ) );
        }
        const std::size_t size = contours.size();
        for( register std::size_t nn = 0; nn < size; ++nn )
        {
            const sContour* contour = contours[ nn ].get();
            T_PointVector* v = new T_PointVector( contour->points_.begin(), contour->points_.end() );
            loops_[ large ? 1 : 3 ].push_back( boost::shared_ptr< T_PointVector >( v ) );
        }
    }
    if( !valid_ )
        return;
    observer_.SetPercentage( 0 );
    controllers_.controller_.Update( observer_ );
    boost::mutex::scoped_lock locker( mutex_ );
    computed_ = true;
    threadRunning_ = false;
}

namespace
{
    void CreateGLArrays( GLenum mode, const std::vector< boost::shared_ptr< T_PointVector > >& contours )
    {
        const std::size_t size = contours.size();
        for( register std::size_t i = 0; i < size; ++i )
        {
            const T_PointVector* l = contours[ i ].get();
            glVertexPointer( 2, GL_FLOAT, 0, &( *l )[ 0 ] );
            glDrawArrays( mode, 0, static_cast< GLsizei >( l->size() ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::CreateCallList
// Created: JSR 2012-01-18
// -----------------------------------------------------------------------------
void ContourLinesLayer::CreateCallList()
{
    {
        boost::mutex::scoped_lock locker( mutex_ );
        if( !computed_ )
        {
            if( !threadRunning_ )
            {
                threadRunning_  = true;
                loops_[ 0 ].clear();
                loops_[ 1 ].clear();
                loops_[ 2 ].clear();
                loops_[ 3 ].clear();
                thread_->Enqueue( boost::bind( &ContourLinesLayer::Conrec, this ) );
            }
            return;
        }
    }
    
    callListId_ = glGenLists( 1 );
    glNewList( callListId_, GL_COMPILE );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnableClientState( GL_VERTEX_ARRAY );
    glLineWidth( 2.f );
    CreateGLArrays( GL_LINE_LOOP, loops_[ 0 ] );
    CreateGLArrays( GL_LINE_STRIP, loops_[ 1 ] );
    glLineWidth( 1.f );
    CreateGLArrays( GL_LINE_LOOP, loops_[ 2 ] );
    CreateGLArrays( GL_LINE_STRIP, loops_[ 3 ] );
    glDisableClientState( GL_VERTEX_ARRAY );
    glEndList();
    loops_[ 0 ].clear();
    loops_[ 1 ].clear();
    loops_[ 2 ].clear();
    loops_[ 3 ].clear();
}
