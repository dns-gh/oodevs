// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ActivityTimePrototype.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "preparation/ObjectAttributesContainer.h"
#include "preparation/ActivityTimeAttribute.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
ActivityTimePrototype::ActivityTimePrototype( QWidget* parent, Object_ABC*& creation )
    : ActivityTimePrototype_ABC( parent )
    , creation_( creation )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
ActivityTimePrototype::~ActivityTimePrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ActivityTimePrototype::Commit()
{
    if( creation_ )
    {
        PropertiesDictionary& dico = creation_->Get< PropertiesDictionary >();
        ActivityTimeAttribute*    attribute = new ActivityTimeAttribute( dico );
                                  attribute->SetActivityTime( 3600 * activityTime_->time().hour() + 
                                                              60 * activityTime_->time().minute() +
                                                              activityTime_->time().second() );
        creation_->Get< ObjectAttributesContainer >().Register( *attribute );
    }
}

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ActivityTimePrototype::Clean()
{
    // NOTHING
}
