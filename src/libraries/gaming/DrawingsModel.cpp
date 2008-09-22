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
DrawingsModel::DrawingsModel( kernel::Controllers& controllers, const DrawingFactory& factory )
    : gui::DrawerModel( controllers, factory )
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
void DrawingsModel::Create( const ASN1T_MsgShapeCreation& asn )
{
    factory_.CreateShape( asn );
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::Update
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void DrawingsModel::Update( const ASN1T_MsgShapeUpdate& asn )
{
    gui::Drawing_ABC* drawing = Find( asn.oid );
    if( drawing )
        drawing->Update( asn );
}

// -----------------------------------------------------------------------------
// Name: DrawingsModel::Delete
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void DrawingsModel::Delete( const ASN1T_MsgShapeDestruction& asn )
{
    gui::DrawerModel::Delete( asn.oid );
}
