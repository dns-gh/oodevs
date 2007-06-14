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
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Viewport_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: FormationLayer constructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
FormationLayer::FormationLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy )
    : controllers_( controllers )
    , tools_      ( tools )
    , strategy_   ( strategy )
    , selectedFormation_( controllers )
    , selectedAutomat_  ( controllers )
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
    viewport.SetHotpoint( geometry::Point2f( std::numeric_limits< float >::infinity(),
                                             std::numeric_limits< float >::infinity() ) );
    if( selectedFormation_ )
    {
        strategy_.SelectColor( *selectedFormation_ );
        selectedFormation_->Draw( geometry::Point2f(), viewport, tools_ );
    }
    if( selectedAutomat_ )
    {
        strategy_.SelectColor( *selectedAutomat_ );
        selectedAutomat_->Draw( geometry::Point2f(), viewport, tools_ );
    }
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::BeforeSelection
// Created: AGE 2007-06-14
// -----------------------------------------------------------------------------
void FormationLayer::BeforeSelection()
{
    selectedFormation_ = 0;
    selectedAutomat_   = 0;
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::Select
// Created: AGE 2007-06-14
// -----------------------------------------------------------------------------
void FormationLayer::Select( const kernel::Formation_ABC& element )
{
    selectedFormation_ = &element;
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::Select
// Created: AGE 2007-06-14
// -----------------------------------------------------------------------------
void FormationLayer::Select( const kernel::Automat_ABC& element )
{
    selectedAutomat_ = &element;
}

// -----------------------------------------------------------------------------
// Name: FormationLayer::AfterSelection
// Created: AGE 2007-06-14
// -----------------------------------------------------------------------------
void FormationLayer::AfterSelection()
{
    // NOTHING
}
