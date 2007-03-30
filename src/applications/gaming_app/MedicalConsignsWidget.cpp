// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MedicalConsignsWidget.h"
#include "gaming/LogMedicalConsign.h"
#include "gaming/MedicalStates.h"

// -----------------------------------------------------------------------------
// Name: MedicalConsignsWidget constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MedicalConsignsWidget::MedicalConsignsWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : LogisticConsignsWidget_ABC< MedicalConsignsWidget, LogMedicalConsign, LogMedicalConsigns >( parent, controllers, factory )
{
    AddConsignColumn( tr( "Injury:" ) );
    AddConsignColumn( tr( "Mentally injured:" ) );
    AddConsignColumn( tr( "NBC contaminated:" ) );
    AddConsignColumn( tr( "State:" ) );
}

// -----------------------------------------------------------------------------
// Name: MedicalConsignsWidget destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MedicalConsignsWidget::~MedicalConsignsWidget()
{
    // NOTHING
}
