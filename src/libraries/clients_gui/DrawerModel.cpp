// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerModel.h"
#include "DrawerFactory.h"
#include "DrawerCategory.h"
#include "xeumeuleu/xml.h"

using namespace gui;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: DrawerModel constructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerModel::DrawerModel( const DrawerFactory& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerModel destructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerModel::~DrawerModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::Load
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
void DrawerModel::Load( const std::string& filename )
{
    xml::xifstream input( filename );
    input >> xml::start( "templates" )
            >> xml::list( "category", *this, &DrawerModel::ReadCategory )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::ReadCategory
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
void DrawerModel::ReadCategory( xml::xistream& xis )
{
    std::auto_ptr< DrawerCategory > category( factory_.CreateCategory( xis ) );
    Register( category->GetName(), *category.release() );
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::Purge
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
void DrawerModel::Purge()
{
    Clear();
}
