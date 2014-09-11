// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __WeaponRangeLayer_h_
#define __WeaponRangeLayer_h_

#include "clients_gui/EntityLayer.h"

// =============================================================================
/** @class  WeaponRangeLayer
    @brief  WeaponRangeLayer
*/
// Created: SBO 2008-04-14
// =============================================================================
class WeaponRangeLayer : public gui::EntityLayerBase
                       , public tools::ElementObserver_ABC< kernel::Agent_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             WeaponRangeLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                       gui::View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~WeaponRangeLayer();
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

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ColorStrategy_ABC& strategy_;
    //@}
};

#endif // __WeaponRangeLayer_h_
