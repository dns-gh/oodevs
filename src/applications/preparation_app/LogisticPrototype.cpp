// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LogisticPrototype.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "preparation/LogisticAttribute.h"
#include "preparation/ObjectAttributesContainer.h"

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
void LogisticPrototype::Commit()
{
    if( creation_ )
    {
        PropertiesDictionary& dico = creation_->Get< PropertiesDictionary >();
        LogisticAttribute* attribute = new LogisticAttribute( dico );
                           attribute->SetTC2( *selected_ );           
        creation_->Get< ObjectAttributesContainer >().Register( *attribute );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void LogisticPrototype::Clean()
{
    // NOTHING
}
