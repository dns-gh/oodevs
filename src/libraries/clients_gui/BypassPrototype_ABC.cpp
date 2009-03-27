// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "BypassPrototype_ABC.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_kernel/Iterator.h"
#include "Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: BypassPrototype_ABC constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
BypassPrototype_ABC::BypassPrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "BypassPrototype_ABC", "Bypass parameters" ) )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: BypassPrototype_ABC destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
BypassPrototype_ABC::~BypassPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassPrototype_ABC::showEvent
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void BypassPrototype_ABC::showEvent( QShowEvent* e )
{
    QGroupBox::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: BypassPrototype_ABC::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool BypassPrototype_ABC::CheckValidity() const
{
    return true;
}
