// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ShapeHandler_ABC_h_
#define __ShapeHandler_ABC_h_

namespace kernel
{
    class Location_ABC;
    class GlTools_ABC;
}

namespace gui
{

// =============================================================================
/** @class  ShapeHandler_ABC
    @brief  ShapeHandler_ABC // $$$$ AGE 2006-03-24: renommer
*/
// Created: AGE 2006-03-24
// =============================================================================
class ShapeHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ShapeHandler_ABC();
    virtual ~ShapeHandler_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Handle( kernel::Location_ABC& location ) = 0;
    virtual void Draw( const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}
};

}

#endif // __ShapeHandler_ABC_h_
