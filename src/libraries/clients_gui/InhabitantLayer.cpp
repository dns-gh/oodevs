// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "clients_gui_pch.h"
#include "InhabitantLayer.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::InhabitantLayer
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
InhabitantLayer::InhabitantLayer( kernel::Controllers& controllers,
                                  GLView_ABC& view,
                                  ColorStrategy_ABC& strategy,
                                  const kernel::Profile_ABC& profile )
    : EntityLayer< kernel::Inhabitant_ABC >( controllers, view, strategy, profile, eLayerTypes_Inhabitants )
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
