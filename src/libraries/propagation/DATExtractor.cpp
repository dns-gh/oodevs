// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "DATExtractor.h"
#include <tools/Path.h>
#include <tools/StdFileWrapper.h>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <boost/lexical_cast.hpp>
#include <geocoord/UTM.h>
#include <geocoord/Geodetic.h>
#include <set>

// -----------------------------------------------------------------------------
// Name: DATExtractor constructor
// Created: JSR 2014-04-10
// -----------------------------------------------------------------------------
DATExtractor::DATExtractor( const tools::Path& file, short timeZone )
    : utm_( new geocoord::UTM( timeZone > 0 ? geocoord::eNorth : geocoord::eSouth, std::abs( timeZone ), 0, 0 ) )
    , geodetic_( new geocoord::Geodetic() )
{
    double xUTMMin = std::numeric_limits< double >::max();
    double xUTMMax = std::numeric_limits< double >::min();
    double yUTMMin = std::numeric_limits< double >::max();
    double yUTMMax = std::numeric_limits< double >::min();

    T_Values values;
    std::set< double > xSet;
    std::set< double > ySet;

    tools::Ifstream stream( file );
    std::string line;
    while( std::getline( stream, line ) )
    {
        boost::algorithm::trim( line );
        std::vector< std::string > inputs;
        boost::algorithm::split( inputs, line, boost::algorithm::is_any_of( " \t" ), boost::algorithm::token_compress_on );
        const double xUTM = boost::lexical_cast< double >( inputs[ 0 ] ) * 1000; // convert from km to m
        const double yUTM = boost::lexical_cast< double >( inputs[ 1 ] ) * 1000;
        xSet.insert( xUTM );
        ySet.insert( yUTM );

        // Height in meters = isomass(g.m^-2) / 1060(kg.m^-3) / 1000 (g->kg)
        values.push_back( boost::lexical_cast< float >( inputs[ 2 ] ) / 1060000 );
        xUTMMin = std::min( xUTMMin, xUTM );
        xUTMMax = std::max( xUTMMax, xUTM );
        yUTMMin = std::min( yUTMMin, yUTM );
        yUTMMax = std::max( yUTMMax, yUTM );
    }
    origin_ = geometry::Point2d( xUTMMin, yUTMMin );
    ncols_ = static_cast< int >( xSet.size() );
    nrows_ = static_cast< int >( ySet.size() );
    pixelSize_.Set( *std::next( xSet.begin() ) - *xSet.begin(), *std::next( ySet.begin() ) - *ySet.begin() );

    values_.resize( nrows_ * ncols_ );
    for( int i = 0; i < nrows_; ++i )
    {
        const auto src = values.begin() + i * ncols_;
        auto dst = values_.begin() + ( nrows_ - i - 1 ) * ncols_;
        std::copy( src, src + ncols_, dst );
    }

    extent_.Set( Project( xUTMMin, yUTMMin ), Project( xUTMMax, yUTMMax ) );
}

// -----------------------------------------------------------------------------
// Name: DATExtractor destructor
// Created: JSR 2014-04-10
// -----------------------------------------------------------------------------
DATExtractor::~DATExtractor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor::Fill
// Created: LGY 2012-11-08
// -----------------------------------------------------------------------------
void DATExtractor::Fill( std::vector< geometry::Point2d >& points ) const
{
    for( int i = 0; i < nrows_; ++i )
        for( int j = 0; j < ncols_; ++j )
        {
            const float value = values_[ ncols_ * i + j ];
            if( value > 0.f )
            {
                double heighttop = ( nrows_ - i - 1 ) * pixelSize_.Y();
                double heightbottom = ( nrows_ - i ) * pixelSize_.Y();
                double widthmin = ( j - 1 ) * pixelSize_.X();
                double widthmax = j * pixelSize_.X();
                const geometry::Rectangle2d r( origin_.X() + widthmin, origin_.Y() + heightbottom, origin_.X() + widthmax, origin_.Y() + heighttop );
                const geometry::Point2d tmp( r.Center() );
                points.push_back( Project( tmp.X(), tmp.Y() ) );
            }
      }
}

// -----------------------------------------------------------------------------
// Name: DATExtractor::Project
// Created: JSR 2014-04-16
// -----------------------------------------------------------------------------
geometry::Point2d DATExtractor::Project( double xUTM, double yUTM ) const
{
    utm_->SetEasting( xUTM );
    utm_->SetNorthing( yUTM );
    geodetic_->SetCoordinates( *utm_ );
    static const double toDegrees = 180. / std::acos( -1. );
    return geometry::Point2d( geodetic_->GetLongitude() * toDegrees, geodetic_->GetLatitude() * toDegrees );
}
