// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __VisionConesLayer_h_
#define __VisionConesLayer_h_

#include "clients_gui/EntityLayer.h"

// =============================================================================
/** @class  VisionConesLayer
    @brief  Vision cones layer
*/
// Created: MCO 2014-07-10
// =============================================================================
class VisionConesLayer : public gui::EntityLayerBase
                       , public tools::ElementObserver_ABC< kernel::Agent_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             VisionConesLayer( kernel::Controllers& controllers, gui::GLView_ABC& view,
                 gui::ColorStrategy_ABC& strategy, const kernel::Profile_ABC& profile );
    virtual ~VisionConesLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const kernel::Entity_ABC& entity, gui::Viewport_ABC& viewport, bool pickingMode );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Agent_ABC& entity );
    virtual void NotifyDeleted( const kernel::Agent_ABC& entity );
    //@}
};

#endif // __VisionConesLayer_h_
