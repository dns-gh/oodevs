// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlTooltip_h_
#define __GlTooltip_h_

#include "GlTooltip_ABC.h"
#include "Tooltip.h"

namespace gui
{
    class TooltipsLayer_ABC;

// =============================================================================
/** @class  GlTooltip
    @brief  GlTooltip
*/
// Created: AGE 2006-06-29
// =============================================================================
class GlTooltip : public GlTooltip_ABC
                , public Tooltip
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit GlTooltip( TooltipsLayer_ABC& layer );
    virtual ~GlTooltip();
    //@}

    //! @name Operations
    //@{
    virtual operator kernel::Displayer_ABC&();
    virtual void Draw( const geometry::Point2f& position, int width = 0, int height = 0, float factor = 1.f );
    //@}

private:
    virtual void DirtyImage();
    virtual void Hide();

private:
    //! @name Member data
    //@{
    TooltipsLayer_ABC& layer_;
    QImage image_;
    //@}
};

}

#endif // __GlTooltip_h_
