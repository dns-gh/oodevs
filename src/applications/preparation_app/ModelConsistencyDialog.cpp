// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ModelConsistencyDialog.h"

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog constructor
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
ModelConsistencyDialog::ModelConsistencyDialog( QWidget* parent, Model& model, const StaticModel& staticModel )
    : QDialog ( parent, "ModelConsistencyDialog" )
    , checker_( model, staticModel )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog destructor
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
ModelConsistencyDialog::~ModelConsistencyDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::CheckConsistency
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::CheckConsistency( unsigned int /*filters*/ /*= ModelConsistencyChecker::eAllChecks*/ )
{
    //checker_.CheckConsistency( filters );
    //show();
    //UpdateErrorListView();
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::UpdateErrorListView
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::UpdateErrorListView()
{
    const ModelConsistencyChecker::T_ConsistencyErrors& errors = checker_.GetConsistencyErrors();
    for( ModelConsistencyChecker::CIT_ConsistencyErrors it = errors.begin(); it != errors.end(); ++it )
    {

    }
}
