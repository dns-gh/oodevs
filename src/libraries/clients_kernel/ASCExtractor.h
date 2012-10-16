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

#include <gdal/gdal_priv.h>
#include <gdal/ogrsf_frmts.h>
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
    //! @name Constructors/Destructor
    //@{
             ASCExtractor( const std::string& file, unsigned int sizeFactor = 1u );
             ASCExtractor( const std::string& file, const std::string& projection, unsigned int sizeFactor = 1u );
    virtual ~ASCExtractor();
    //@}

    //! @name Types
    //@{
    typedef std::pair< geometry::Rectangle2d, double > T_Tile;
    typedef std::vector< T_Tile >                      T_Tiles;
    //@}

    //! @name Operations
    //@{
    int GetCols() const;
    int GetRows() const;
    double GetNoValueData() const;
    double GetMinimumValue() const;
    double GetMaximumValue() const;

    const geometry::Point2d& GetOrigin() const;
    const geometry::Point2d& GetPixelSize() const;
    const geometry::Rectangle2d& GetExtent() const;
    const T_Tiles& GetTiles() const;
    //@}

private:
    //! @name Helpers
    //@{
    geometry::Rectangle2d GenerateTile( int x, int y, int sizeX, int sizeY );
    geometry::Rectangle2d GenerateExtent( double left, double bottom, double right, double top );
    void Project( const geometry::Point2d& point, double& rLatitudeInDegrees, double& rLongitudeInDegrees );
    void ExtractData( unsigned int sizeFactor );
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
    double min_;
    double max_;
    T_Tiles tiles_;
    geometry::Rectangle2d extent_;
    geometry::Point2d origin_;
    geometry::Point2d pixelSize_;
    //@}
};

}

#endif // ASCExtractor_h
