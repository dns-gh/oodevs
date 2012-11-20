// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "DisasterPrototype.h"
#include "preparation/DisasterAttribute.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/LoadableTimeEdit.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DisasterPrototype constructor
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
DisasterPrototype::DisasterPrototype( QWidget* parent, const tools::GeneralConfig& config, kernel::Object_ABC*& creation )
    : DisasterPrototype_ABC( parent, config )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype destructor
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
DisasterPrototype::~DisasterPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype::Commit
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
void DisasterPrototype::Commit( const kernel::Team_ABC& )
{
    if( creation_ )
    {
        PropertiesDictionary& dictionary = creation_->Get< PropertiesDictionary >();
        QTime time;
        if( checkbox_->isChecked() )
            time = time_->time();
        creation_->Attach( *new DisasterAttribute( dictionary, propagationFiles_->currentText(), time ) );
    }
}
