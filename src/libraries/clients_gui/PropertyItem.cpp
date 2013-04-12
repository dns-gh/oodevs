// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PropertyItem.h"
#include "PropertyDisplayer.h"
#include "Property_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertyItem constructor
// Created: LGY 2012-08-13
// -----------------------------------------------------------------------------
PropertyItem::PropertyItem( Property_ABC& property, PropertyDisplayer& displayer )
    : displayer_( displayer )
    , property_ ( property )
{
    Update();
}

// -----------------------------------------------------------------------------
// Name: PropertyItem destructor
// Created: LGY 2012-08-13
// -----------------------------------------------------------------------------
PropertyItem::~PropertyItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyItem::Update
// Created: LGY 2012-08-14
// -----------------------------------------------------------------------------
void PropertyItem::Update( QWidget* widget )
{
    if( widget )
        property_.SetValueFromEditor( widget );
    displayer_.SetItem( this );
    property_.Display( displayer_ );
}
