// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LogisticPrototype.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "preparation/LogisticAttribute.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LogisticPrototype::LogisticPrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
LogisticPrototype::LogisticPrototype( QWidget* parent, Controllers& controllers, Object_ABC*& creation )
    : LogisticPrototype_ABC( parent, controllers )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype::~LogisticPrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
LogisticPrototype::~LogisticPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void LogisticPrototype::Commit( const kernel::Team_ABC& team )
{
    if( creation_ && selected_ != 0 && CheckValidity( team ) )
    {
        PropertiesDictionary& dictionary = creation_->Get< PropertiesDictionary >();
        LogisticAttribute* attribute = new LogisticAttribute( dictionary, controllers_, *creation_ );
        attribute->SetLogisticBase( *selected_ );
        creation_->Attach( *attribute );
    }
}
