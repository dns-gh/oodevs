// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Dataset.h"
#include "PhysicalModel.h"
#include "clients_kernel/Controller.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Dataset constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
Dataset::Dataset( const ASN1T_MsgDatasetCreation& message, kernel::Controller& controller )
    : controller_( controller )
    , name_( message.name )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Dataset destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
Dataset::~Dataset()
{
    Clear();
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: Dataset::AddPhysicalModel
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
void Dataset::AddPhysicalModel( PhysicalModel& model )
{
    Register( model.GetName(), model );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Dataset::RemovePhysicalModel
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
void Dataset::RemovePhysicalModel( const PhysicalModel& model )
{
    Remove( model.GetName() );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Dataset::GetName
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
QString Dataset::GetName() const
{
    return name_;
}
