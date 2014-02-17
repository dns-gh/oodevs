// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Node_ABC_h_
#define __Node_ABC_h_

namespace svg
{
    class RenderingContext_ABC;
    class References_ABC;

// =============================================================================
/** @class  Node_ABC
    @brief  Node_ABC
*/
// Created: AGE 2006-08-22
// =============================================================================
class Node_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Node_ABC() {};
    virtual ~Node_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual Node_ABC& Compile( RenderingContext_ABC& context, References_ABC& references ) const = 0;
    virtual void      Draw   ( RenderingContext_ABC& context, References_ABC& references ) const = 0;
    //@}
};

}

#endif // __Node_ABC_h_
