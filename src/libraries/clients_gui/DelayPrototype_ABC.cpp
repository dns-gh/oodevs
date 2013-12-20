// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DelayPrototype_ABC.h"
#include "SubObjectName.h"
#include "LoadableTimeEdit.h"

#include "clients_kernel/Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DelayPrototype_ABC constructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayPrototype_ABC::DelayPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, "DelayPrototype_ABC", tools::translate( "gui::DelayPrototype_ABC", "Delay time" ) )
{
    SubObjectName subObject( "DelayPrototype_ABC" );
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    layout->addWidget( new QLabel( tools::translate( "gui::DelayPrototype_ABC", "Delay time:" ) ) );
    delayTime_ = new LoadableTimeEdit( "delayTime", this );
    layout->addWidget( delayTime_ );
}

// -----------------------------------------------------------------------------
// Name: DelayPrototype_ABC destructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayPrototype_ABC::~DelayPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayPrototype_ABC::CheckValidity
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
bool DelayPrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: DelayPrototype_ABC::SetLoader
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
void DelayPrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    delayTime_->SetLoader( loader );
}
