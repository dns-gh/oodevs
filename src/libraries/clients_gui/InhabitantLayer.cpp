// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
#include "clients_gui_pch.h"
#include "InhabitantLayer.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::InhabitantLayer
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
InhabitantLayer::InhabitantLayer( kernel::Controllers& controllers, const GlTools_ABC& tools,
                                  ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile )
    : EntityLayer< kernel::Inhabitant_ABC >( controllers, tools, strategy, view, profile, tr( "Inhabitants" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantLayer::~InhabitantLayer
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
InhabitantLayer::~InhabitantLayer()
{
    // NOTHING
}
