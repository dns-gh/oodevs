// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawingTypes.h"
#include "DrawingCategory.h"
#include <svgl/TextRenderer.h>
#include <xeumeuleu/xml.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawingTypes constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingTypes::DrawingTypes( kernel::Controller& controller )
    : controller_( controller )
    , renderer_( new svg::TextRenderer() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingTypes destructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingTypes::~DrawingTypes()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: DrawingTypes::Load
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
void DrawingTypes::Load( const std::string& filename )
{
    xml::xifstream input( filename );
    input >> xml::start( "templates" )
            >> xml::list( "category", *this, &DrawingTypes::ReadCategory )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: DrawingTypes::Purge
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
void DrawingTypes::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: DrawingTypes::ReadCategory
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
void DrawingTypes::ReadCategory( xml::xistream& xis )
{
    std::auto_ptr< DrawingCategory > category( new DrawingCategory( xis, *renderer_, controller_ ) );
    Register( category->GetName(), *category );
    category.release();
}
