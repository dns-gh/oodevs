// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef kernel_ASCExtractor_h
#define kernel_ASCExtractor_h

#pragma warning( push, 0 )
#include <gdal/gdal_priv.h>
#include <gdal/ogrsf_frmts.h>
#pragma warning( pop )
#include <boost/noncopyable.hpp>
#include <geometry/Types.h>
#include <vector>

namespace kernel
{
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
    explicit ASCExtractor( const std::string& file );
             ASCExtractor( const std::string& file, const std::string& projection );
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
    geometry::Rectangle2d GenerateExtent( double left, double bottom, double right, double top );
    //@}

private:
    //! @name Helpers
    //@{
    geometry::Rectangle2d GenerateTile( int x, int y, int sizeX, int sizeY );
    void Project( const geometry::Point2d& point, double& rLatitudeInDegrees, double& rLongitudeInDegrees );
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

}

#endif // ASCExtractor_h
