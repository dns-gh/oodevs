// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationEditorToolbar_h_
#define __LocationEditorToolbar_h_

#include "clients_gui/LocationEditorToolbar.h"

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
}

namespace gui
{
    class View_ABC;
    class LocationsLayer;
}

// =============================================================================
/** @class  LocationEditorToolbar
    @brief  Location editor toolbar
*/
// Created: LGY 2012-01-11
// =============================================================================
class LocationEditorToolbar : public gui::LocationEditorToolbar

{
public:
    //! @name Constructors/Destructor
    //@{
             LocationEditorToolbar( QMainWindow* parent,
                                    kernel::Controllers& controllers,
                                    const kernel::CoordinateConverter_ABC& converter,
                                    gui::View_ABC& view,
                                    const std::shared_ptr< gui::LocationsLayer >& layer );
    virtual ~LocationEditorToolbar();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __LocationEditorToolbar_h_
