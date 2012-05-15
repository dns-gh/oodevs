// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanLayer_h_
#define __UrbanLayer_h_

#include "clients_gui/UrbanLayer.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class Profile_ABC;
}


namespace gui
{
    class ColorStrategy_ABC;
    class View_ABC;
    class LayerFilter_ABC;
    class TerrainObjectProxy;
}

// =============================================================================
/** @class  UrbanLayer
    @brief  Urban layer
*/
// Created: LGY 2012-01-06
// =============================================================================
class UrbanLayer : public gui::UrbanLayer
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                         gui::View_ABC& view, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter );
    virtual ~UrbanLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool IsInSelection( const kernel::Entity_ABC& entity, const geometry::Point2f& point ) const;
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    //@}
};

#endif // __UrbanLayer_h_
