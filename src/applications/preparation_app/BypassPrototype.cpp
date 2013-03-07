// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "BypassPrototype.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_gui/LoadableSpinBox.h"
#include "preparation/BypassAttribute.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: BypassPrototype constructor
// Created: NPT 2012-09-05
// -----------------------------------------------------------------------------
BypassPrototype::BypassPrototype( QWidget* parent, Object_ABC*& creation )
    : BypassPrototype_ABC( parent )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassPrototype destructor
// Created: NPT 2012-09-05
// -----------------------------------------------------------------------------
BypassPrototype::~BypassPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassPrototype::Commit
// Created: NPT 2012-09-05
// -----------------------------------------------------------------------------
void BypassPrototype::Commit( const kernel::Team_ABC& )
{
    if( creation_ )
    {
        kernel::PropertiesDictionary& dico = creation_->Get< kernel::PropertiesDictionary >();
        BypassAttribute* attribute = new BypassAttribute( dico, *creation_ );
        attribute->SetBypassConstruction( bypassConstructionPercentage_->value() );
        creation_->Attach( *attribute );
    }
}
