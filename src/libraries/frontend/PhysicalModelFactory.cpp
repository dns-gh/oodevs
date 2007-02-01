// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "PhysicalModelFactory.h"
#include "PhysicalModel.h"
#include "Model.h"
#include "DatasetsModel.h"
#include "Dataset.h"
#include "clients_kernel/Controllers.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: PhysicalModelFactory constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
PhysicalModelFactory::PhysicalModelFactory( kernel::Controllers& controllers, Model& model )
    : controllers_( controllers )
    , model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PhysicalModelFactory destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
PhysicalModelFactory::~PhysicalModelFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PhysicalModelFactory::Create
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
PhysicalModel* PhysicalModelFactory::Create( const ASN1T_MsgPhysicalModelCreation& message )
{
    std::auto_ptr< PhysicalModel > physicalModel( new PhysicalModel( message, controllers_.controller_, model_.datasets_ ) );
    model_.datasets_.Get( message.dataset ).AddPhysicalModel( *physicalModel );
    return physicalModel.release();
}
