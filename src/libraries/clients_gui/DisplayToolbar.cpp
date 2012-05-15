// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::DisplayToolbar */

#include "clients_gui_pch.h"
#include "DisplayToolbar.h"
#include "BooleanOptionButton.h"
#include "resources.h"
#include "Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "tools/GeneralConfig.h"

using namespace gui;

namespace
{
    QPixmap MakeIcon( const std::string& file )
    {
        return QPixmap( tools::GeneralConfig::BuildResourceChildFile( file ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: DisplayToolbar constructor
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
DisplayToolbar::DisplayToolbar( QMainWindow* pParent, kernel::Controllers& controllers )
    : RichToolBar( controllers, pParent, "displaytoolbar", tools::translate( "gui::DisplayToolbar", "Display toolbar" ) )
{
    addWidget( new BooleanOptionButton( MAKE_ICON( threed ), tools::translate( "gui::DisplayToolbar", "3D" ), this, controllers.options_, "3D" ) );
    addWidget( new BooleanOptionButton( MakeIcon( "images/gui/fogofwar.png" ), tools::translate( "gui::DisplayToolbar", "Fog of war" ), this, controllers.options_, "FogOfWar" ) );
}

// -----------------------------------------------------------------------------
// Name: DisplayToolbar destructor
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
DisplayToolbar::~DisplayToolbar()
{
    // NOTHING
}
