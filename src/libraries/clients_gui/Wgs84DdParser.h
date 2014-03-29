// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Wgs84DdParser_h_
#define __Wgs84DdParser_h_

#include "LocationParser_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace gui
{
// =============================================================================
/** @class  Wgs84DdParser
    @brief  Wgs84DdParser
*/
// Created: AME 2010-03-04
// =============================================================================
class Wgs84DdParser : public LocationParser_ABC
                    , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Wgs84DdParser( const kernel::CoordinateConverter_ABC& converter );
    virtual ~Wgs84DdParser();
    //@}

    //! @name LocationParser_ABC methods
    //@{
    virtual const LocationParserDescriptor& GetDescriptor() const;
    virtual bool Parse( const QStringList& content, geometry::Point2f& result, QStringList& hint, bool small ) const;
    virtual QStringList Split( const QString& input ) const;
    virtual std::string GetStringPosition( const geometry::Point2f& position ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

}

#endif // __Wgs84DdParser_h_
