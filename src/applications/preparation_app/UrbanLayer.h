// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanLayer_h_
#define __UrbanLayer_h_

#include "clients_gui/UrbanLayer.h"

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;
    class LayerFilter_ABC;
}

class UrbanModel;

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
             UrbanLayer( kernel::Controllers& controllers,
                         gui::GLView_ABC& view,
                         gui::ColorStrategy_ABC& strategy,
                         UrbanModel& model,
                         const kernel::Profile_ABC& profile );
    virtual ~UrbanLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool IsInside( const kernel::Entity_ABC& entity, const geometry::Rectangle2f& rectangle ) const;
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleKeyPress( QKeyEvent* key );
    //@}

private:
    //! @name Member data
    //@{
    UrbanModel& model_;
    //@}
};

#endif // __UrbanLayer_h_
