// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectIcons_h_
#define __ObjectIcons_h_

namespace kernel
{
    class Viewport_ABC;
    class GlTools_ABC;

// =============================================================================
/** @class  ObjectIcons
    @brief  ObjectIcons
*/
// Created: AGE 2008-03-14
// =============================================================================
class ObjectIcons
{

public:
    //! @name Operations
    //@{
    static void Draw( unsigned long type, const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools );
    //@}

    //! @name Helpers
    //@{
    static const char** TypeIcon( unsigned long id );
    //@}
};

}

#endif // __ObjectIcons_h_
