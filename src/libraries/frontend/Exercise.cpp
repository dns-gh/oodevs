// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Exercise.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Exercise constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
Exercise::Exercise( const ASN1T_MsgExerciseCreation& message, kernel::Controllers& controllers, const kernel::StringResolver< Terrain >& terrainResolver
                  , const kernel::StringResolver< Dataset >& datasetResolver, const kernel::StringResolver< PhysicalModel >& physicalResolver )
    : controller_      ( controllers.controller_ )
    , terrainResolver_ ( terrainResolver )
    , datasetResolver_ ( datasetResolver )
    , physicalResolver_( physicalResolver )
    , name_            ( message.name )
    , terrain_         ( controllers )
    , dataset_         ( controllers )
    , physical_        ( controllers )
{
    Update( message );
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
Exercise::~Exercise()
{
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::DoUpdate
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void Exercise::DoUpdate( const ASN1T_MsgExerciseUpdate& message )
{
    Update( message );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::Update
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void Exercise::Update( const ASN1T_Exercise& message )
{
    name_     = message.name;
    terrain_  = &terrainResolver_ .Get( message.terrain );
    dataset_  = &datasetResolver_ .Get( message.dataset );
    physical_ = &physicalResolver_.Get( message.physical );
}

// -----------------------------------------------------------------------------
// Name: Exercise::GetName
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
QString Exercise::GetName() const
{
    return name_;
}
