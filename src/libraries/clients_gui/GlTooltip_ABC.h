// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlTooltip_ABC_h_
#define __GlTooltip_ABC_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Displayer_ABC;
}

namespace gui
{
// =============================================================================
/** @class  GlTooltip_ABC
    @brief  Tooltip definition
*/
// Created: AGE 2007-05-30
// =============================================================================
class GlTooltip_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             GlTooltip_ABC() {}
    virtual ~GlTooltip_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual operator kernel::Displayer_ABC&() = 0;
    virtual void Draw( const geometry::Point2f& where ) = 0;
    //@}
};

}

#endif // __GlTooltip_ABC_h_
