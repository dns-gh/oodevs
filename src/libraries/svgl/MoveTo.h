// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MoveTo_h_
#define __MoveTo_h_

#include "PathCommand_ABC.h"

namespace svg 
{
class PathParser;

namespace path
{
class LineTo;

// =============================================================================
/** @class  MoveTo
    @brief  MoveTo
*/
// Created: AGE 2006-08-10
// =============================================================================
class MoveTo : public PathCommand_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MoveTo( PathParser& data, PathContext& context );
    virtual ~MoveTo();
    //@}

    //! @name Operations
    //@{
    virtual void DrawLine( float expectedPrecision );
    virtual void DrawArea( Tesselator_ABC& tesselator, float expectedPrecision );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MoveTo( const MoveTo& );            //!< Copy constructor
    MoveTo& operator=( const MoveTo& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point where_;
    LineTo* line_;
    //@}
};

}
}

#endif // __MoveTo_h_
