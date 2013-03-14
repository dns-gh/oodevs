// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ImageWrapper.h"
#include "tools/Path.h"

namespace gui
{

// -----------------------------------------------------------------------------
// Pixmap
// -----------------------------------------------------------------------------
Pixmap::Pixmap( const tools::Path& file )
    : QPixmap( QString::fromWCharArray( file.ToUnicode().c_str() ) )
{
    // NOTHING
}

Pixmap::~Pixmap()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Image
// -----------------------------------------------------------------------------
Image::Image( const tools::Path& file )
    : QImage( QString::fromWCharArray( file.ToUnicode().c_str() ) )
{
    // NOTHING
}

Image::~Image()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// QIcon
// -----------------------------------------------------------------------------
Icon::Icon( const tools::Path& file )
    : QIcon( QString::fromWCharArray( file.ToUnicode().c_str() ) )
{
    // NOTHING
}

Icon::~Icon()
{
    // NOTHING
}

}
