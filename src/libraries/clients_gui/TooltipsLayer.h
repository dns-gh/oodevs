// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TooltipsLayer_h_
#define __TooltipsLayer_h_

#include "TooltipsLayer_ABC.h"
#include "Layer.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
// =============================================================================
/** @class  TooltipsLayer
    @brief  Tooltips layer
*/
// Created: SBO 2008-04-11
// =============================================================================
class TooltipsLayer : public TooltipsLayer_ABC, public Layer
{
public:
    explicit TooltipsLayer( kernel::Controllers& controllers, GLView_ABC& tools );
    virtual ~TooltipsLayer();

    virtual void Paint( Viewport_ABC& viewport );
    virtual void Draw( const geometry::Point2f& position, const Tooltip& tooltip );

private:
    std::set< std::pair< geometry::Point2f, Tooltip > > tooltips_;
    std::map< Tooltip, QImage > cache_;
    float alpha_;
};

}

#endif // __TooltipsLayer_h_
