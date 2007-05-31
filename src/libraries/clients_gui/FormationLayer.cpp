// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "FormationLayer.h"
#include "ColorStrategy_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: FormationLayer constructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
FormationLayer::FormationLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy )
    : controllers_( controllers )
    , tools_      ( tools )
    , strategy_   ( strategy )
    , selected_   ( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer destructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
FormationLayer::~FormationLayer()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::Paint
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void FormationLayer::Paint( kernel::Viewport_ABC& viewport )
{
    if( selected_ )
    {
        strategy_.SelectColor( *selected_ );
        selected_->Draw( geometry::Point2f(), viewport, tools_ );
    }
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::NotifySelected
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void FormationLayer::NotifySelected( const kernel::Formation_ABC* element )
{
    selected_ = element;
}
