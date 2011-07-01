// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DrawingsModel.h"
#include "DrawingFactory.h"
#include "clients_gui/Drawing_ABC.h"

// -----------------------------------------------------------------------------
// Name: DrawingsModel constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingsModel::DrawingsModel( kernel::Controllers& controllers, const DrawingFactory& factory, const kernel::EntityResolver_ABC& resolver )
    : gui::DrawerModel( controllers, factory, resolver )
    , factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel destructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingsModel::~DrawingsModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::Create
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void DrawingsModel::Create( const sword::ShapeCreation& message )
{
    factory_.CreateShape( message );
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::Update
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void DrawingsModel::Update( const sword::ShapeUpdate& message )
{
    gui::Drawing_ABC* drawing = Find( message.id().id() );
    if( drawing )
        drawing->Update( message );
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::Delete
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void DrawingsModel::Delete( const sword::ShapeDestruction& message )
{
    gui::DrawerModel::Delete( message.id().id() );
}
