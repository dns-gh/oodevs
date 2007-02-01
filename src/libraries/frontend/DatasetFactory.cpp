// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "DatasetFactory.h"
#include "Dataset.h"
#include "clients_kernel/Controllers.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: DatasetFactory constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
DatasetFactory::DatasetFactory( kernel::Controllers& controllers )
    : controllers_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DatasetFactory destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
DatasetFactory::~DatasetFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DatasetFactory::Create
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
Dataset* DatasetFactory::Create( const ASN1T_MsgDatasetCreation& message )
{
    return new Dataset( message, controllers_.controller_ );
}
