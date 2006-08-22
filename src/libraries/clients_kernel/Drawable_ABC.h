// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Drawable_ABC_h_
#define __Drawable_ABC_h_

namespace kernel
{
    class GlTools_ABC;

// =============================================================================
/** @class  Drawable_ABC
    @brief  Drawable extension definition
*/
// Created: AGE 2006-03-16
// =============================================================================
class Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Drawable_ABC() {};
    virtual ~Drawable_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const = 0;
    //@}
};

}

#endif // __Drawable_ABC_h_
