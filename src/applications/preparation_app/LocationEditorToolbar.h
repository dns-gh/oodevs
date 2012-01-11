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
#include "clients_kernel/OptionsObserver_ABC.h"

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
                            , public kernel::OptionsObserver_ABC

{
public:
    //! @name Constructors/Destructor
    //@{
             LocationEditorToolbar( QMainWindow* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter,
                                    gui::View_ABC& view, gui::LocationsLayer& layer );
    virtual ~LocationEditorToolbar();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LocationEditorToolbar( const LocationEditorToolbar& );            //!< Copy constructor
    LocationEditorToolbar& operator=( const LocationEditorToolbar& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void StartEdit( gui::ParametersLayer& parameters );
    virtual void NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    bool livingAreaEditor_;
    //@}
};

#endif // __LocationEditorToolbar_h_
