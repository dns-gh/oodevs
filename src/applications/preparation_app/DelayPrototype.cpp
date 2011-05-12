// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "DelayPrototype.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/LoadableTimeEdit.h"
#include "preparation/DelayAttribute.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DelayPrototype constructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayPrototype::DelayPrototype( QWidget* parent, kernel::Object_ABC*& creation )
    : DelayPrototype_ABC( parent )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayPrototype destructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayPrototype::~DelayPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayPrototype::Commit
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayPrototype::Commit()
{
    if( creation_ )
    {
        PropertiesDictionary& dico = creation_->Get< PropertiesDictionary >();
        DelayAttribute* attribute = new DelayAttribute( dico );
        attribute->SetDelay( 3600 * delayTime_->time().hour() +
                               60 * delayTime_->time().minute() +
                                    delayTime_->time().second() );
        creation_->Attach( *attribute );
    }
}
