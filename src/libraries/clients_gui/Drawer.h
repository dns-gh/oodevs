// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef CLIENTS_GUI_DRAWER_H__
#define CLIENTS_GUI_DRAWER_H__

#include <vector>

namespace kernel
{
    class Extension_ABC;
}

namespace gui
{
    class Drawable_ABC;
    class GLView_ABC;
    class Viewport_ABC;
}

namespace gui
{
// =============================================================================
/** @class  Drawer
    @brief  Drawer implementation
*/
// Created: AGE 2006-08-10
// =============================================================================
class Drawer
{
public:
    //! @name Constructors/Destructor
    //@{
             Drawer();
    virtual ~Drawer();
    //@}

    //! @name Operations
    //@{
    void Register( const kernel::Extension_ABC& extension );
    void Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, GLView_ABC& tools ) const;
    void Pick( const geometry::Point2f& where, const Viewport_ABC& viewport, GLView_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Drawer( const Drawer& );            //!< Copy constructor
    Drawer& operator=( const Drawer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Add( const Drawable_ABC& extension );
    std::string Strip( const std::string& name );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const Drawable_ABC* > T_Drawables;
    //@}

private:
    //! @name Member data
    //@{
    T_Drawables extensions_;
    //@}
};

}

#endif // CLIENTS_GUI_DRAWER_H__
