// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::DelayPrototype_ABC */

#include "clients_gui_pch.h"
#include "DelayPrototype_ABC.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DelayPrototype_ABC constructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayPrototype_ABC::DelayPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "gui::DelayPrototype_ABC", "Delay time" ) )
{
    new QLabel( tools::translate( "gui::DelayPrototype_ABC", "Delay time:" ), this );
    delayTime_ = new QTimeEdit( this );
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
bool DelayPrototype_ABC::CheckValidity() const
{
    return true;
}
