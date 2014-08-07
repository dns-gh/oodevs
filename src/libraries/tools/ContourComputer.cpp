// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "ContourComputer.h"

#include <boost/bind.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <deque>

#define XSECT( p1, p2 ) ( h[ p2 ] * xh[ p1 ] - h[ p1 ] * xh[ p2 ] ) / ( h[ p2 ] - h[ p1 ] )
#define YSECT( p1, p2 ) ( h[ p2 ] * yh[ p1 ] - h[ p1 ] * yh[ p2 ] ) / ( h[ p2 ] - h[ p1 ] )

namespace tools
{

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

bool ComputeContour( const unsigned int width,  const unsigned int height,
    const int linesHeight, const float cellSize,
    const std::function< short(int, int) >& data,
    const std::function< void(short) >& progress,
    const std::function< bool() >& valid,
    const std::function< void( boost::shared_ptr< T_PointVector >, int, bool )>& loop,
    const std::function< bool() >& checkStop,
    const int nc )
{
    // Adapted from http://paulbourke.net/papers/conrec/
    int sh[ 5 ];
    float h[ 5 ];
    float xh[ 5 ];
    float yh[ 5 ];
    float x1;
    float x2;
    float y1;
    float y2;

    std::vector< boost::shared_ptr< T_PointVector > > points;
    for( int i = 0; i < nc; ++i )
        points.push_back( boost::make_shared< T_PointVector >() );

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
        if( !valid() )
            return false;

        progress( static_cast< short >( 10.f * ( height - j ) / height ) );

        for( register unsigned int i = 0; i <= width - 2; ++i )
        {
            short temp1;
            short temp2;
            temp1 = std::min( data( i    , j ), data( i    , j + 1 ) );
            temp2 = std::min( data( i + 1, j ), data( i + 1, j + 1 ) );
            float dmin = std::min( temp1, temp2 );
            temp1 = std::max( data( i    , j ), data( i    , j + 1 ) );
            temp2 = std::max( data( i + 1, j ), data( i + 1, j + 1 ) );
            float dmax = std::max( temp1, temp2 );
            if( dmax >= linesHeight + 0.1 && dmin <= nc * linesHeight  + 0.1 )
            {
                for( register int k = 0; k < nc; ++k )
                {
                    const float alt  = linesHeight * ( k + 1 ) + 0.1f;
                    if( alt >= dmin && alt <= dmax )
                    {
                        for( register int m = 4; m >=0; --m )
                        {
                            if( m > 0 )
                            {
                                h[ m ] = data( i + im [ m - 1 ], j + jm[ m - 1 ] ) - alt;
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
        if( !valid() )
            return false;
        progress( static_cast< short >( 10 + 90.f * k / nc ) );

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
                            if( !valid() )
                                return false;
                            if( checkStop() )
                                return false;
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
                        v->push_back( v->front() );
                        loop( boost::shared_ptr< T_PointVector >( v ), k, true );
                        contours.erase( contours.begin() + index );
                    }
                    break;
                }
            }
            if( !found )
                contours.push_back( boost::make_shared< sContour >( p1, p2 ) );
        }
        const std::size_t size = contours.size();
        for( register std::size_t nn = 0; nn < size; ++nn )
        {
            const sContour* contour = contours[ nn ].get();
            T_PointVector* v = new T_PointVector( contour->points_.begin(), contour->points_.end() );
            loop( boost::shared_ptr< T_PointVector >( v ), k, true );
        }
    }
    return true;
}

} // namespace tools
