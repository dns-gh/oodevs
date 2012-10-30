// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ASCExtractor.h"

#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <numeric>
namespace bfs = boost::filesystem;

using namespace kernel;

namespace
{
    OGRCoordinateTransformation* CreateCoordinateTransformation( const std::string& projection )
    {
        OGRSpatialReference oSourceSRS, oTargetSRS;
        oTargetSRS.SetFromUserInput( "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs" );
        oSourceSRS.SetFromUserInput( projection.c_str() );
        return OGRCreateCoordinateTransformation( &oSourceSRS, &oTargetSRS );
    }

    short ConvertData( double value, double max )
    {
        if( value == 0 )
            return 0;
        return static_cast< short >( value * std::numeric_limits< short >::max() / max );
    }

    std::string ReadProjectionfile( const bfs::path& file )
    {
        std::string projection;
        std::ifstream stream( file.string().c_str() );
        while( std::getline( stream, projection ) ) {}
        return projection;
    }
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor constructor
// Created: LGY 2012-10-03
// -----------------------------------------------------------------------------
ASCExtractor::ASCExtractor( const std::string& file )
    : pDataset_       ( 0 )
    , pBand_          ( 0 )
    , pTransformation_( 0 )
    , ncols_          ( 0 )
    , nrows_          ( 0 )
    , max_            ( 1. )
    , noValueData_    ( 0. )
{
    GDALAllRegister();
    pDataset_ = ( GDALDataset* ) GDALOpen( file.c_str(), GA_ReadOnly );
    if( pDataset_ == NULL )
        throw std::runtime_error( "Unable to open file : " + file );

    if( pDataset_->GetRasterCount() != 1 )
        throw std::runtime_error( "Format not supported : " + file );

    pTransformation_ = CreateCoordinateTransformation( pDataset_->GetProjectionRef() );

    if( !pTransformation_ )
        throw std::runtime_error( "Invalid projection" );

    ExtractData();
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor constructor
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
ASCExtractor::ASCExtractor( const std::string& file, const std::string& projection )
    : pDataset_       ( 0 )
    , pBand_          ( 0 )
    , pTransformation_( 0 )
    , ncols_          ( 0 )
    , nrows_          ( 0 )
    , max_            ( 1. )
    , noValueData_    ( 0. )
{
    GDALAllRegister();
    pDataset_ = ( GDALDataset* ) GDALOpen( file.c_str(), GA_ReadOnly );
    if( pDataset_ == NULL )
        throw std::runtime_error( "Unable to open file : " + file );

    if( pDataset_->GetRasterCount() != 1 )
        throw std::runtime_error( "Format not supported : " + file );

    pTransformation_ = CreateCoordinateTransformation( ReadProjectionfile( bfs::path( projection ) ) );

    if( !pTransformation_ )
        throw std::runtime_error( "Invalid projection" );

    ExtractData();
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor destructor
// Created: LGY 2012-10-03
// -----------------------------------------------------------------------------
ASCExtractor::~ASCExtractor()
{
    if( pDataset_ )
    {
        GDALClose( pDataset_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor::ExtractData
// Created: LGY 2012-10-16
// -----------------------------------------------------------------------------
void ASCExtractor::ExtractData()
{
    ncols_ = pDataset_->GetRasterXSize();
    nrows_ = pDataset_->GetRasterYSize();
    pBand_ = pDataset_->GetRasterBand( 1 );
    noValueData_ = pBand_->GetNoDataValue();

    double adfGeoTransform[ 6 ];
    if( pDataset_->GetGeoTransform( adfGeoTransform ) == CE_None )
    {
        origin_ = geometry::Point2d( adfGeoTransform[ 0 ], adfGeoTransform[ 3 ] );
        pixelSize_ = geometry::Point2d( adfGeoTransform[ 1 ], adfGeoTransform[ 5 ] );
    }

    extent_ = GenerateExtent( origin_.X() + ( ncols_ * pixelSize_.X() ),
                              origin_.Y() + ( nrows_ * pixelSize_.Y() ),
                              origin_.X(), origin_.Y() );

    int size = ncols_ * nrows_;
    std::vector< float > data( size );
    values_.resize( size );

    pBand_->RasterIO( GF_Read, 0, 0, ncols_, nrows_, &data[ 0 ], ncols_, nrows_, GDT_Float32, 0, 0 );

    for( int i = 0; i < size; ++i )
    {
        float value = data[ i ];
        if( value == noValueData_ )
            data[ i ] = 0;
        else
            max_ = std::max< double >( max_, value );
    }

    for( int i = 0; i < size; ++i )
        values_[ i ] = ConvertData( data[ i ], max_ );
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor::GenerateExtent
// Created: LGY 2012-10-10
// -----------------------------------------------------------------------------
geometry::Rectangle2d ASCExtractor::GenerateExtent( double left, double bottom, double right, double top )
{
    geometry::Rectangle2d extent( left, bottom, right, top );
    double lx,ly,rx,ry;
    Project( extent.BottomLeft(), ly, lx );
    Project( extent.TopRight(), ry, rx );
    return geometry::Rectangle2d( lx, ly, rx, ry );
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor::GenerateTile
// Created: LGY 2012-10-09
// -----------------------------------------------------------------------------
geometry::Rectangle2d ASCExtractor::GenerateTile( int x, int y, int sizeX, int sizeY )
{
    double heighttop = y * pixelSize_.Y();
    double heightbottom = ( y + sizeY ) * pixelSize_.Y();
    double widthmin = x * pixelSize_.X();
    double widthmax = ( x + sizeX ) * pixelSize_.X();
    return GenerateExtent( origin_.X() + widthmin, origin_.Y() + heightbottom, origin_.X() + widthmax, origin_.Y() + heighttop );
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor::Project
// Created: LGY 2012-10-10
// -----------------------------------------------------------------------------
void ASCExtractor::Project( const geometry::Point2d& point, double& rLatitudeInDegrees, double& rLongitudeInDegrees )
{
    rLongitudeInDegrees = point.X();
    rLatitudeInDegrees = point.Y();
    pTransformation_->Transform( 1, &rLongitudeInDegrees, &rLatitudeInDegrees );
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor::GetExtent
// Created: LGY 2012-10-08
// -----------------------------------------------------------------------------
const geometry::Rectangle2d& ASCExtractor::GetExtent() const
{
    return extent_;
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor::GetCols
// Created: LGY 2012-10-03
// -----------------------------------------------------------------------------
int ASCExtractor::GetCols() const
{
    return ncols_;
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor::GetRows
// Created: LGY 2012-10-03
// -----------------------------------------------------------------------------
int ASCExtractor::GetRows() const
{
    return nrows_;
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor::GetNoValueData
// Created: LGY 2012-10-03
// -----------------------------------------------------------------------------
double ASCExtractor::GetNoValueData() const
{
    return noValueData_;
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor::GetOrigin
// Created: LGY 2012-10-03
// -----------------------------------------------------------------------------
const geometry::Point2d& ASCExtractor::GetOrigin() const
{
    return origin_;
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor::GetPixelSize
// Created: LGY 2012-10-03
// -----------------------------------------------------------------------------
const geometry::Point2d& ASCExtractor::GetPixelSize() const
{
    return pixelSize_;
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor::GetMaximumValue
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
double ASCExtractor::GetMaximumValue() const
{
    return max_;
}

// -----------------------------------------------------------------------------
// Name: ASCExtractor::GetValues
// Created: LGY 2012-10-24
// -----------------------------------------------------------------------------
const ASCExtractor::T_Values& ASCExtractor::GetValues() const
{
    return values_;
}
