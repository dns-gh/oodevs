// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UtmParser_h_
#define __UtmParser_h_

#include "LocationParser_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace gui
{

// =============================================================================
/** @class  UtmParser
    @brief  UtmParser
*/
// Created: AGE 2008-05-29
// =============================================================================
class UtmParser : public LocationParser_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit UtmParser( const kernel::CoordinateConverter_ABC& converter );
    virtual ~UtmParser();
    //@}

    //! @name Operations
    //@{
    virtual bool Parse( const QString& content, geometry::Point2f& result, QString& hint ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UtmParser( const UtmParser& );            //!< Copy constructor
    UtmParser& operator=( const UtmParser& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

}

#endif // __UtmParser_h_
