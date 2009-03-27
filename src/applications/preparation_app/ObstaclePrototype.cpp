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
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "preparation/ObstacleAttribute.h"
#include "preparation/ObjectAttributesContainer.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype::ObstaclePrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
ObstaclePrototype::ObstaclePrototype( QWidget* parent, Object_ABC*& creation )
    : ObstaclePrototype_ABC( parent )
    , creation_ ( creation )
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
void ObstaclePrototype::Commit()
{
    if( creation_ )
    {
        PropertiesDictionary& dico = creation_->Get< PropertiesDictionary >();
        ObstacleAttribute* attribute = new ObstacleAttribute( dico, types_->GetValue() );
                           attribute->Activate( bActivated_->isChecked() );
        creation_->Get< ObjectAttributesContainer >().Register( *attribute );
    }
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ObstaclePrototype::Clean()
{
    // NOTHING
}
