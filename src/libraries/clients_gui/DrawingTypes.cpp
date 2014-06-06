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
#include "tools/ExerciseConfig.h"
#include "tools/PhyLoader.h"
#include <svgl/TextRenderer.h>
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawingTypes constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingTypes::DrawingTypes( kernel::Controller& controller )
    : controller_( controller )
    , renderer_  ( new svg::TextRenderer() )
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
void DrawingTypes::Load( const tools::ExerciseConfig& config )
{
    config.GetPhyLoader().LoadPhysicalFile( "drawing-templates", boost::bind( &DrawingTypes::Read, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: DrawingTypes::Read
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
void DrawingTypes::Read( xml::xistream& xis )
{
    xis >> xml::start( "templates" )
            >> xml::list( "category", *this, &DrawingTypes::ReadCategory )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: DrawingTypes::Purge
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
void DrawingTypes::Purge()
{
    DeleteAll();
    renderer_.reset(  new svg::TextRenderer() );
}

// -----------------------------------------------------------------------------
// Name: DrawingTypes::ReadCategory
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
void DrawingTypes::ReadCategory( xml::xistream& xis )
{
    std::unique_ptr< DrawingCategory > category( new DrawingCategory( xis, *renderer_, controller_ ) );
    Register( category->GetName(), *category );
    category.release();
}
