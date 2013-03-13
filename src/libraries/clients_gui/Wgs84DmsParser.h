// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Wgs84DmsParser_h_
#define __Wgs84DmsParser_h_

#include "LocationParser_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace gui
{
// =============================================================================
/** @class  Wgs84DmsParser
    @brief  Wgs84DmsParser
*/
// Created: AME 2010-03-04
// =============================================================================
class Wgs84DmsParser : public LocationParser_ABC
                     , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Wgs84DmsParser( const kernel::CoordinateConverter_ABC& converter );
    virtual ~Wgs84DmsParser();
    //@}

    //! @name Operations
    //@{
    virtual bool Parse( const QString& content, geometry::Point2f& result, QStringList& hint ) const;
    virtual int GetNumberOfParameters() const;
    //@}

private:
    //! @name Helpers
    //@{
    bool FormatDmsCoordinate( const QString& content, bool longitude, QString& hint ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

}

#endif // __Wgs84DmsParser_h_
