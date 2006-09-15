// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LogisticRoutePrototype.h"
#include "clients_kernel/Object_ABC.h"
#include "preparation/LogisticRouteAttributes.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LogisticRoutePrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
LogisticRoutePrototype::LogisticRoutePrototype( QWidget* parent, Object_ABC*& creation )
    : LogisticRoutePrototype_ABC( parent )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticRoutePrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
LogisticRoutePrototype::~LogisticRoutePrototype()
{
    Clean();
}

// -----------------------------------------------------------------------------
// Name: LogisticRoutePrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void LogisticRoutePrototype::Commit()
{
    if( !creation_ )
        return;

    LogisticRouteAttributes& attr = static_cast< LogisticRouteAttributes& >( creation_->Get< LogisticRouteAttributes_ABC >() );
    attr.SetFlow( flow_->value() );
    attr.SetWidth( width_->value() );
    attr.SetLength( length_->value() );
    attr.SetMaxWeight( maxWeight_->value() );
    attr.SetEquipped( equipped_->isOn() );
}

// -----------------------------------------------------------------------------
// Name: LogisticRoutePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void LogisticRoutePrototype::Clean()
{
    // NOTHING
}
