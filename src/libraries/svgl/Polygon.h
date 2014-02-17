// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Polygon_h_
#define __Polygon_h_

#include "Path.h"

namespace svg
{

// =============================================================================
/** @class  Polygon
    @brief  Polygon
*/
// Created: AGE 2006-08-16
// =============================================================================
class Polygon : public Path
{

public:
    //! @name Constructors/Destructor
    //@{
             Polygon( xml::xistream& input, References_ABC& references, const PropertyFactory& factory );
    virtual ~Polygon();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Polygon( const Polygon& );
    Polygon& operator=( const Polygon& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    static std::string CreatePath( xml::xistream& input );
    //@}
};

}

#endif // __Polygon_h_
