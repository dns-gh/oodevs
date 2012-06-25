// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InhabitantLayer_h_
#define __InhabitantLayer_h_

#include "clients_gui/InhabitantLayer.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class Viewport_ABC;
    class Profile_ABC;
    class Inhabitant_ABC;
}

namespace gui
{
    class View_ABC;
    class ColorStrategy_ABC;
}

class LivingAreaEditor_ABC;

// =============================================================================
/** @class  InhabitantLayer
    @brief  Inhabitant layer
*/
// Created: LGY 2012-01-10
// =============================================================================
class InhabitantLayer : public gui::InhabitantLayer
{
public:
    //! @name Constructors/Destructor
    //@{
             InhabitantLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools,
                              gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile,
                              LivingAreaEditor_ABC& editor );
    virtual ~InhabitantLayer();
    //@}

    //! @name Operations
    //@{
    virtual void NotifySelectionChanged( const std::vector< const kernel::Inhabitant_ABC* >& elements );
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    //@}

private:
    //! @name Member Data
    //@{
    LivingAreaEditor_ABC& editor_;
    //@}
};

#endif // __InhabitantLayer_h_
