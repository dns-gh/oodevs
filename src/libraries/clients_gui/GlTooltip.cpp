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
    image_ = QImage();
}

void GlTooltip::Draw( const geometry::Point2f& position )
{
    if( image_.isNull() )
        image_ = GenerateImage().mirror();
    layer_.Draw( position, image_ );
}

void GlTooltip::Hide()
{
    image_ = QImage();
}

GlTooltip::operator kernel::Displayer_ABC&()
{
    return *this;
}
