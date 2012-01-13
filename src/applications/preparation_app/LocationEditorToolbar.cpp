// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LocationEditorToolbar.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar constructor
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
LocationEditorToolbar::LocationEditorToolbar( QMainWindow* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter,
                                              gui::View_ABC& view, gui::LocationsLayer& layer )
    : gui::LocationEditorToolbar( parent, controllers, converter, view, layer )
    , controllers_( controllers )
    , livingAreaEditor_( false )
{
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar destructor
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
LocationEditorToolbar::~LocationEditorToolbar()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::StartEdit
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void LocationEditorToolbar::StartEdit( gui::ParametersLayer& parameters )
{
    if( !livingAreaEditor_ )
        gui::LocationEditorToolbar::StartEdit( parameters );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::NotifyContextMenu
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void LocationEditorToolbar::NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu )
{
    if( !livingAreaEditor_ )
        gui::LocationEditorToolbar::NotifyContextMenu( point, menu );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::OptionChanged
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void LocationEditorToolbar::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "LivingAreaEditor" )
        livingAreaEditor_ = value.To< bool >();
}
