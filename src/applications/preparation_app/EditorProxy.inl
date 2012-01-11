// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel/OptionVariant.h"

// -----------------------------------------------------------------------------
// Name: EditorProxy constructor
// Created: LGY 2012-01-03
// -----------------------------------------------------------------------------
template< typename T >
EditorProxy< T >::EditorProxy( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                               gui::View_ABC& view, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter )
    : T( controllers, tools, strategy, view, profile, filter )
    , livingAreaEditor_( false )
{
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: EditorProxy constructor
// Created: LGY 2012-01-03
// -----------------------------------------------------------------------------
template< typename T >
EditorProxy< T >::EditorProxy( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                               gui::View_ABC& view, const kernel::Profile_ABC& profile, gui::TerrainPicker& picker, const gui::LayerFilter_ABC& filter )
    : T( controllers, tools, strategy, view, profile, picker, filter )
    , livingAreaEditor_( false )
{
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: EditorProxy constructor
// Created: LGY 2012-01-03
// -----------------------------------------------------------------------------
template< typename T >
EditorProxy< T >::EditorProxy( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                               gui::ParametersLayer& parameters, gui::View_ABC& view, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter )
    : T( controllers, tools, strategy, parameters, view, profile, filter )
    , livingAreaEditor_( false )
{
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: EditorProxy destructor
// Created: LGY 2012-01-03
// -----------------------------------------------------------------------------
template< typename T >
EditorProxy< T >::~EditorProxy()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EditorProxy::ShouldDisplay
// Created: LGY 2012-01-03
// -----------------------------------------------------------------------------
template< typename T >
bool EditorProxy< T >::ShouldDisplay( const kernel::Entity_ABC& entity )
{
    if( livingAreaEditor_ )
        return false;
    return T::ShouldDisplay( entity );
}

// -----------------------------------------------------------------------------
// Name: EditorProxy::OptionChanged
// Created: LGY 2012-01-03
// -----------------------------------------------------------------------------
template< typename T >
void EditorProxy< T >::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "LivingAreaEditor" )
        livingAreaEditor_ = value.To< bool >();
}
