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
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Wgs84DdParser( const kernel::CoordinateConverter_ABC& converter );
    virtual ~Wgs84DdParser();
    //@}

    //! @name Operations
    //@{
    virtual bool Parse( QString content, geometry::Point2f& result, QStringList& hint ) const;
    virtual int GetNumberOfParameters() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Wgs84DdParser( const Wgs84DdParser& );            //!< Copy constructor
    Wgs84DdParser& operator=( const Wgs84DdParser& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    int numParameters_;
    //@}
};

}

#endif // __Wgs84DdParser_h_
