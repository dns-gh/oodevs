// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PolyLine_h_
#define __PolyLine_h_

#include "Path.h"

namespace svg
{

// =============================================================================
/** @class  PolyLine
    @brief  PolyLine
*/
// Created: AGE 2006-08-16
// =============================================================================
class PolyLine : public Path
{

public:
    //! @name Constructors/Destructor
    //@{
             PolyLine( xml::xistream& input, References_ABC& references, const PropertyFactory& factory );
    virtual ~PolyLine();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PolyLine( const PolyLine& );
    PolyLine& operator=( const PolyLine& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    static std::string CreatePath( xml::xistream& input );
    //@}
};

}

#endif // __PolyLine_h_
