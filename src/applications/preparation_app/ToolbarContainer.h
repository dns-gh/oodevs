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
    class ExclusiveEventStrategy;
    class GisToolbar;
    class LocationsLayer;
    class ParametersLayer;
    class TerrainProfiler;
    class GLView_ABC;
}

class FileToolbar;
class LocationEditorToolbar;
class RemoveBlocksDialog;
class StaticModel;
class UrbanModel;

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
             ToolbarContainer( QMainWindow* parent,
                               kernel::Controllers& controllers,
                               const StaticModel& staticModel,
                               gui::GLView_ABC& view,
                               const std::shared_ptr< gui::LocationsLayer >& layer,
                               const std::shared_ptr< gui::ParametersLayer >& paramLayer,
                               gui::ExclusiveEventStrategy& eventStrategy,
                               UrbanModel& urbanModel,
                               RemoveBlocksDialog& removeBlocksDialog,
                               gui::TerrainProfiler& terrainProfiler );
    virtual ~ToolbarContainer();
    //@}

    //! @name Accessors
    //@{
    FileToolbar& GetFileToolbar() const;
    LocationEditorToolbar& GetLocationEditorToolbar() const;
    //@}

private:
    //! @name Member data
    //@{
    FileToolbar*           fileToolbar_;
    LocationEditorToolbar* locationEditorToolbar_;
    //@}
};

#endif // __ToolbarContainer_h_
