// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ObjectPrototypeAttributes_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeAttributes_ABC constructor
// Created: AGE 2006-08-08
// -----------------------------------------------------------------------------
ObjectPrototypeAttributes_ABC::ObjectPrototypeAttributes_ABC( QWidget* parent, const QString& name )
    : QGroupBox( 2, Qt::Horizontal, name, parent )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ObjectPrototypeAttributes_ABC destructor
// Created: AGE 2006-08-08
// -----------------------------------------------------------------------------
ObjectPrototypeAttributes_ABC::~ObjectPrototypeAttributes_ABC()
{
    // NOTHING
}
