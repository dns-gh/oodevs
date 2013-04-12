// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __GlTooltip_ABC_h_
#define __GlTooltip_ABC_h_

namespace kernel
{
    class Displayer_ABC;
}

namespace gui
{
// =============================================================================
/** @class  GlTooltip_ABC
    @brief  GlTooltip_ABC
*/
// Created: AGE 2007-05-30
// =============================================================================
class GlTooltip_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             GlTooltip_ABC() {};
    virtual ~GlTooltip_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual operator kernel::Displayer_ABC& () = 0;
    virtual void Draw( const geometry::Point2f& where, int width = 0, int height = 0, float factor = 1.f ) = 0;
    virtual void Hide() = 0;
    //@}
};

}

#endif // __GlTooltip_ABC_h_
