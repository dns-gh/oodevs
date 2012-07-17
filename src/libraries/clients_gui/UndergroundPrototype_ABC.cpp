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
#include "clients_gui/RichLabel.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ObjectExtensions.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UndergroundPrototype_ABC constructor
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
UndergroundPrototype_ABC::UndergroundPrototype_ABC( QWidget* parent, kernel::Controller& controller )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::UndergroundPrototype_ABC", "Underground" ) )
    , controller_( controller )
{
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    networkLabel_ = new RichLabel( tools::translate( "gui::UndergroundPrototype_ABC", "Network:" ) );
    layout->addWidget( networkLabel_ );
    network_ = new LoadableComboBox( true, 0 );
    layout->addWidget( network_ );
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
bool UndergroundPrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    if( network_->currentText().isEmpty() )
    {
        networkLabel_->Warn( 3000 );
        return false;
    }
    return true;
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
            if( network == combo->text( i ).toUtf8().constData() )
                return;
        }
        combo->insertItem( network.c_str() );
    }
}
