// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ObjectAttributePrototype_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototype_ABC constructor
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
ObjectAttributePrototype_ABC::ObjectAttributePrototype_ABC( QWidget* parent, const QString& name )
    : QGroupBox( 2, Qt::Horizontal, name, parent )
{
    hide();
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototype_ABC destructor
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
ObjectAttributePrototype_ABC::~ObjectAttributePrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototype_ABC::SetLoader
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
void ObjectAttributePrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* /*loader*/ )
{
    //NOTHING
}
