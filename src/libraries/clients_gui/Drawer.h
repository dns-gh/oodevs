// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef CLIENTS_GUI_DRAWER_H__
#define CLIENTS_GUI_DRAWER_H__

#include <vector>

namespace kernel
{
    class Extension_ABC;
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace gui
{
    class Drawable_ABC;

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
    void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
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
