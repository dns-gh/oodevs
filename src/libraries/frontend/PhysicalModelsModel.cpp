// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "PhysicalModelsModel.h"
#include "PhysicalModelFactory_ABC.h"
#include "PhysicalModel.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: PhysicalModelsModel constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
PhysicalModelsModel::PhysicalModelsModel( PhysicalModelFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PhysicalModelsModel destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
PhysicalModelsModel::~PhysicalModelsModel()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: PhysicalModelsModel::CreatePhysicalModel
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
void PhysicalModelsModel::CreatePhysicalModel( const ASN1T_MsgPhysicalModelCreation& message )
{
    std::auto_ptr< PhysicalModel > physicalModel( factory_.Create( message ) );
    Register( physicalModel->GetName(), *physicalModel.release() );
}
