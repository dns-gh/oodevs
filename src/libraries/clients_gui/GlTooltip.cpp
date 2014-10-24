// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GlTooltip.h"
#include "TooltipsLayer_ABC.h"

using namespace gui;

GlTooltip::GlTooltip( TooltipsLayer_ABC& layer )
    : layer_( layer )
{
    // NOTHING
}

GlTooltip::~GlTooltip()
{
    // NOTHING
}

void GlTooltip::DirtyImage()
{
    // NOTHING
}

void GlTooltip::Draw( const geometry::Point2f& position )
{
    layer_.Draw( position, GetTooltip() );
}

void GlTooltip::Hide()
{
    // NOTHING
}

GlTooltip::operator kernel::Displayer_ABC&()
{
    return *this;
}
