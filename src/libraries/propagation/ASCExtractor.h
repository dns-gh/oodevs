// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef ASCExtractor_h
#define ASCExtractor_h

#pragma warning( push, 0 )
#include <gdal/gdal_priv.h>
#include <gdal/ogrsf_frmts.h>
#pragma warning( pop )
#include <boost/noncopyable.hpp>
#include <geometry/Types.h>
#include <vector>

namespace tools
{
    class Path;
}

// =============================================================================
/** @class  ASCExtractor
    @brief  ASC extractor
*/
// Created: LGY 2012-10-03
// =============================================================================
class ASCExtractor : private boost::noncopyable
{
public:
    typedef std::vector< float > T_Values;

    //! @name Constructors/Destructor
    //@{
    explicit ASCExtractor( const tools::Path& file );
             ASCExtractor( const tools::Path& file, const tools::Path& projection );
    virtual ~ASCExtractor();
    //@}

    //! @name Operations
    //@{
    int GetCols() const;
    int GetRows() const;
    double GetNoValueData() const;
    double GetMaximumValue() const;

    const geometry::Point2d& GetOrigin() const;
    const geometry::Point2d& GetPixelSize() const;
    const geometry::Rectangle2d& GetExtent() const;
    const T_Values& GetValues() const;
    geometry::Rectangle2d GenerateExtent( double left, double bottom, double right, double top ) const;
    void Fill( std::vector< geometry::Point2d >& points ) const;
    float GetValue( double latitude, double longitude ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void Project( const geometry::Point2d& point, double& rLatitudeInDegrees, double& rLongitudeInDegrees ) const;
    void ExtractData();
    //@}

private:
    //! @name Member data
    //@{
    GDALDataset* pDataset_;
    GDALRasterBand* pBand_;
    OGRCoordinateTransformation* pTransformation_;
    int ncols_;
    int nrows_;
    double noValueData_;
    double max_;
    geometry::Rectangle2d extent_;
    geometry::Point2d origin_;
    geometry::Point2d pixelSize_;
    T_Values values_;
    //@}
};

#endif // ASCExtractor_h