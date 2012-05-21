// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ToolbarContainer_h_
#define __ToolbarContainer_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class View_ABC;
    class LocationsLayer;
    class GisToolbar;
}

class FileToolbar;
class LocationEditorToolbar;
class StaticModel;

// =============================================================================
/** @class  ToolbarContainer
    @brief  ToolbarContainer
*/
// Created: ABR 2012-05-16
// =============================================================================
class ToolbarContainer : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             ToolbarContainer( QMainWindow* parent, kernel::Controllers& controllers, const StaticModel& staticModel, gui::View_ABC& view, gui::LocationsLayer& layer );
    virtual ~ToolbarContainer();
    //@}

    //! @name Accessors
    //@{
    FileToolbar& GetFileToolbar() const;
    LocationEditorToolbar& GetLocationEditorToolbar() const;
    gui::GisToolbar& GetGisToolbar() const;
    //@}

private:
    //! @name Member data
    //@{
    FileToolbar*           fileToolbar_;
    LocationEditorToolbar* locationEditorToolbar_;
    gui::GisToolbar*       gisToolbar_;
    //@}
};

#endif // __ToolbarContainer_h_
