// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "CampPrototype.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "preparation/ObjectsModel.h"
#include "preparation/CampAttributes.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: CampPrototype::CampPrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CampPrototype::CampPrototype( QWidget* parent, Controllers& controllers, ObjectsModel& model, Object_ABC*& creation )
    : CampPrototype_ABC( parent, controllers )
    , model_( model )
    , creation_( creation )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: CampPrototype::~CampPrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CampPrototype::~CampPrototype()
{
    Clean();
}

// -----------------------------------------------------------------------------
// Name: CampPrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CampPrototype::Commit()
{
    if( creation_ )
        static_cast< CampAttributes& >( creation_->Get< CampAttributes_ABC >() ).SetTC2( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: CampPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void CampPrototype::Clean()
{
    // NOTHING
}
