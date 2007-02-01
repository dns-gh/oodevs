// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "PhysicalModel.h"
#include "clients_kernel/Controller.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: PhysicalModel constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
PhysicalModel::PhysicalModel( const ASN1T_MsgPhysicalModelCreation& message, kernel::Controller& controller
                            , const kernel::StringResolver< Dataset >& datasetResolver )
    : controller_( controller )
    , name_( message.name )
    , dataset_( datasetResolver.Get( message.dataset ) )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: PhysicalModel destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
PhysicalModel::~PhysicalModel()
{
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: PhysicalModel::GetName
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
QString PhysicalModel::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: PhysicalModel::GetDataset
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
const Dataset& PhysicalModel::GetDataset() const
{
    return dataset_;
}
