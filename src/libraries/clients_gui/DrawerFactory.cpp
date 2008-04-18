// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerFactory.h"
#include "DrawerCategory.h"
#include "clients_kernel/Controllers.h"
#include "svgl/TextRenderer.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerFactory constructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerFactory::DrawerFactory( QWidget* parent, kernel::GlTools_ABC& tools, kernel::Controllers& controllers )
    : parent_( parent )
    , controllers_( controllers )
    , tools_( tools )
    , renderer_( *new svg::TextRenderer() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerFactory destructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerFactory::~DrawerFactory()
{
    delete &renderer_; 
}

// -----------------------------------------------------------------------------
// Name: DrawerFactory::CreateCategory
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerCategory* DrawerFactory::CreateCategory( xml::xistream& xis ) const
{
    return new DrawerCategory( parent_, tools_, xis, renderer_, controllers_.controller_ );
}
