// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Circle_h_
#define __Circle_h_

#include "Ellipse.h"

namespace svg
{

// =============================================================================
/** @class  Circle
    @brief  Circle
*/
// Created: AGE 2006-08-15
// =============================================================================
class Circle : public Ellipse
{

public:
    //! @name Constructors/Destructor
    //@{
             Circle( xml::xistream& input, References_ABC& references, const PropertyFactory& factory );
    virtual ~Circle();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Circle( const Circle& );            //!< Copy constructor
    Circle& operator=( const Circle& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    static geometry::Point ReadCenter( xml::xistream& input );
    static float             ReadRadius( xml::xistream& input );
    //@}
};

}

#endif // __Circle_h_
