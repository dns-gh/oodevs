// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef CLIENTS_GUI_DRAWABLE_ABC_H__
#define CLIENTS_GUI_DRAWABLE_ABC_H__

namespace gui
{
    class GLView_ABC;
    class Viewport_ABC;
}

namespace gui
{
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
    virtual void Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, GLView_ABC& tools ) const = 0;
    virtual void Pick( const geometry::Point2f& /*where*/, const Viewport_ABC& /*viewport*/, GLView_ABC& /*tools*/ ) const {}
    //@}
};

}

#endif // CLIENTS_GUI_DRAWABLE_ABC_H__
