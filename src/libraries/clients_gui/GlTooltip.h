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
#include "TooltipDisplayer.h"

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
                , public TooltipDisplayer
{
public:
    explicit GlTooltip( TooltipsLayer_ABC& layer );
    virtual ~GlTooltip();

    virtual operator kernel::Displayer_ABC&();
    virtual void Draw( const geometry::Point2f& position );

private:
    virtual void DirtyImage();
    virtual void Hide();

private:
    TooltipsLayer_ABC& layer_;
};

}

#endif // __GlTooltip_h_
