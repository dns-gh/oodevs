// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef DATExtractor_h
#define DATExtractor_h

#include "Extractor_ABC.h"

namespace geocoord
{
    class Geodetic;
    class UTM;
}

namespace tools
{
    class Path;
}

// =============================================================================
/** @class  DATExtractor
    @brief  DAT extractor
*/
// Created: JSR 2014-04-10
// =============================================================================
class DATExtractor : public Extractor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DATExtractor( const tools::Path& file, short timeZone );
    virtual ~DATExtractor();
    //@}

    //! @name Operations
    //@{
    virtual void Fill( std::vector< geometry::Point2d >& points ) const;
    //@}

private:
    //! @name Helpers
    //@{
    geometry::Point2d Project( double xUTM, double yUTM ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< geocoord::UTM > utm_;
    std::auto_ptr< geocoord::Geodetic > geodetic_;
    geometry::Point2d origin_;
    //@}
};

#endif // DATExtractor_h
