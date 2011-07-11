// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::UndergroundPrototype_ABC */

#include "clients_gui_pch.h"
#include "UndergroundPrototype_ABC.h"
#include "Tools.h"
#include "LoadableComboBox.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ObjectExtensions.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UndergroundPrototype_ABC constructor
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
UndergroundPrototype_ABC::UndergroundPrototype_ABC( QWidget* parent, kernel::Controller& controller )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::UndergroundPrototype_ABC", "Underground network" ) )
    , controller_( controller )
{
    new QLabel( tools::translate( "gui::UndergroundPrototype_ABC", "Underground network:" ), this );
    network_ = new LoadableComboBox( true, this );
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UndergroundPrototype_ABC destructor
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
UndergroundPrototype_ABC::~UndergroundPrototype_ABC()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UndergroundPrototype_ABC::CheckValidity
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
bool UndergroundPrototype_ABC::CheckValidity() const
{
    return !network_->currentText().isEmpty();
}

// -----------------------------------------------------------------------------
// Name: UndergroundPrototype_ABC::SetLoader
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
void UndergroundPrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    network_->SetLoader( loader );
}

// -----------------------------------------------------------------------------
// Name: UndergroundPrototype_ABC::NotifyUpdated
// Created: JSR 2011-07-08
// -----------------------------------------------------------------------------
void UndergroundPrototype_ABC::NotifyUpdated( const kernel::UndergroundAttribute_ABC& attribute )
{
    QComboBox* combo = network_->GetDefaultValueWidget();
    if( combo )
    {
        const std::string& network = attribute.GetNetwork();
        for( int i = 0; i < combo->count(); ++i )
        {
            if( network == combo->text( i ).ascii() )
                return;
        }
        combo->insertItem( network.c_str() );
    }
}
