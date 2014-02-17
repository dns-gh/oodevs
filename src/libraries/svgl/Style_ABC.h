// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Style_ABC_h_
#define __Style_ABC_h_

namespace svg
{
    class RenderingContext_ABC;
    class References_ABC;

// =============================================================================
/** @class  Style_ABC
    @brief  Style_ABC
*/
// Created: AGE 2007-10-30
// =============================================================================
class Style_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Style_ABC() {};
    virtual ~Style_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Push( RenderingContext_ABC& context, References_ABC& references ) const = 0;
    virtual void Pop ( RenderingContext_ABC& context, References_ABC& references ) const = 0;
    //@}
};

}

#endif // __Style_ABC_h_
