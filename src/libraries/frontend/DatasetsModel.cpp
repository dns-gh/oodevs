// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "DatasetsModel.h"
#include "Dataset.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: DatasetsModel constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
DatasetsModel::DatasetsModel( DatasetFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DatasetsModel destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
DatasetsModel::~DatasetsModel()
{
    DeleteAll();
}
