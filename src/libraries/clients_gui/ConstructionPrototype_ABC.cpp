// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ConstructionPrototype_ABC.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ConstructionPrototype_ABC constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
ConstructionPrototype_ABC::ConstructionPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "ConstructionPrototype_ABC", "Construction" ) )    
{
    // new QLabel( tools::translate( "ConstructionPrototype_ABC", "Construction:" ), this );
}

// -----------------------------------------------------------------------------
// Name: ConstructionPrototype_ABC destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
ConstructionPrototype_ABC::~ConstructionPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionPrototype_ABC::showEvent
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void ConstructionPrototype_ABC::showEvent( QShowEvent* e )
{    
    QGroupBox::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: ConstructionPrototype_ABC::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool ConstructionPrototype_ABC::CheckValidity() const
{
    return true;
}