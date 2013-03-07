// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ObstaclePrototype.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/LoadableCheckBox.h"
#include "preparation/ObstacleAttribute.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype::ObstaclePrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
ObstaclePrototype::ObstaclePrototype( QWidget* parent, Object_ABC*& creation )
    : ObstaclePrototype_ABC( parent )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype::~ObstaclePrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
ObstaclePrototype::~ObstaclePrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObstaclePrototype::Commit( const kernel::Team_ABC& )
{
    if( creation_ )
    {
        PropertiesDictionary& dictionary = creation_->Get< PropertiesDictionary >();
        ObstacleAttribute* attribute = new ObstacleAttribute( dictionary, types_->GetValue(), *creation_ );
        unsigned int activationTime = GetActivationTime();
        attribute->SetActivationTime( activationTime );
        attribute->SetActivityTime( GetActivityTime() );
        attribute->Activate( types_->GetValue() == eDemolitionTargetType_Preliminary && activationTime == 0 );
        creation_->Attach( *attribute );
    }
}
