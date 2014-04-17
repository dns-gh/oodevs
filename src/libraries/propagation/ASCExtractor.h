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

#include "Extractor_ABC.h"
#pragma warning( push, 0 )
#include <gdal/gdal_priv.h>
#include <gdal/ogrsf_frmts.h>
#pragma warning( pop )

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
class ASCExtractor : public Extractor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ASCExtractor( const tools::Path& file );
             ASCExtractor( const tools::Path& file, const tools::Path& projection );
    virtual ~ASCExtractor();
    //@}

    //! @name Operations
    //@{
    virtual void Fill( std::vector< geometry::Point2d >& points ) const;

    double GetNoValueData() const;
    double GetMaximumValue() const;

    const geometry::Point2d& GetOrigin() const;
    geometry::Rectangle2d GenerateExtent( double left, double bottom, double right, double top ) const;
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
    double noValueData_;
    double max_;
    geometry::Point2d origin_;
    //@}
};

#endif // ASCExtractor_h
