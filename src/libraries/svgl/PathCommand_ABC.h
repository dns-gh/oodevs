// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PathCommand_ABC_h_
#define __PathCommand_ABC_h_

#include "PathContext.h"

class Tesselator_ABC;

namespace svg
{

// =============================================================================
/** @class  PathCommand_ABC
    @brief  PathCommand_ABC
*/
// Created: AGE 2006-08-10
// =============================================================================
class PathCommand_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PathCommand_ABC();
    virtual ~PathCommand_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void DrawLine  ( float expectedPrecision ) = 0;
    virtual void DrawArea  ( Tesselator_ABC& tesselator, float expectedPrecision ) = 0; 
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PathCommand_ABC( const PathCommand_ABC& );            //!< Copy constructor
    PathCommand_ABC& operator=( const PathCommand_ABC& ); //!< Assignement operator
    //@}
};

}

#endif // __PathCommand_ABC_h_
