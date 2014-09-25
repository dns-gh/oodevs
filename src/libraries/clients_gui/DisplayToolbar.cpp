// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DisplayToolbar.h"
#include "BooleanOptionButton.h"
#include "ImageWrapper.h"
#include "resources.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/Tools.h"

#include "tools/GeneralConfig.h"

using namespace gui;

namespace
{
    QPixmap MakeIcon( const tools::Path& file )
    {
        return gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( file ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DisplayToolbar constructor
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
DisplayToolbar::DisplayToolbar( QMainWindow* parent, kernel::Controllers& controllers )
    : RichToolBar( controllers, parent, "displaytoolbar", tools::translate( "gui::DisplayToolbar", "Display toolbar" ) )
{
    addWidget( new BooleanOptionButton( "3DToolButton", MAKE_ICON( threed ), tools::translate( "gui::DisplayToolbar", "3D" ), this, controllers.options_, "3D", false ) );
    addWidget( new BooleanOptionButton( "FogOfWarButton", MakeIcon( "images/gui/fogofwar.png" ), tools::translate( "gui::DisplayToolbar", "Fog of war" ), this, controllers.options_, "FogOfWar" ) );

    // Raster
    QAction* rasterAction = new QAction( tools::translate( "gui::DisplayToolbar", "Add raster data" ), this );
    rasterAction->setIcon( Icon( "resources/images/gui/raster.png" ) );
    rasterAction->setToolTip( tools::translate( "gui::DisplayToolbar", "Add raster data" ) );
    connect( rasterAction, SIGNAL( triggered() ), parent, SLOT( OnAddRaster() ) );
    addAction( rasterAction );
    widgetForAction( rasterAction )->setObjectName( "rasterAction" );
}

// -----------------------------------------------------------------------------
// Name: DisplayToolbar destructor
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
DisplayToolbar::~DisplayToolbar()
{
    // NOTHING
}
